#include "HTML/parser.h"

typedef struct
{
    char *data;
    size_t length;
    size_t capacity;
} HtmlBuffer;

static int appendHTML(HtmlBuffer *buffer, const char *text, size_t length)
{
    if (!text || length == 0)
        return 1;

    if (length > (size_t)-1 - buffer->length - 1)
        return 0;

    size_t required = buffer->length + length + 1;
    if (required > buffer->capacity)
    {
        size_t capacity = buffer->capacity ? buffer->capacity : 1024;
        while (capacity < required)
        {
            if (capacity > (size_t)-1 / 2)
            {
                capacity = required;
                break;
            }
            capacity *= 2;
        }

        char *data = realloc(buffer->data, capacity);
        if (!data)
            return 0;
        buffer->data = data;
        buffer->capacity = capacity;
    }

    memcpy(buffer->data + buffer->length, text, length);
    buffer->length += length;
    buffer->data[buffer->length] = '\0';
    return 1;
}

static int appendHTMLString(HtmlBuffer *buffer, const char *text)
{
    return text ? appendHTML(buffer, text, strlen(text)) : 1;
}

static int appendTagStart(HtmlBuffer *buffer, const TagNode *tag)
{
    if (!appendHTMLString(buffer, "<") ||
        !appendHTMLString(buffer, tag->name))
        return 0;

    if (tag->attributes)
    {
        for (int i = 0; i < HASHMAP_SIZE; i++)
        {
            HashEntry *entry = tag->attributes->entries[i];
            while (entry)
            {
                if (!appendHTMLString(buffer, " ") ||
                    !appendHTMLString(buffer, entry->key) ||
                    !appendHTMLString(buffer, "=\"") ||
                    !appendHTMLString(buffer, entry->value) ||
                    !appendHTMLString(buffer, "\""))
                    return 0;
                entry = entry->next;
            }
        }
    }

    return appendHTMLString(buffer, ">");
}

static int appendParsedStyles(HtmlBuffer *buffer, const StyleNode *styles)
{
    size_t count = 0;
    for (const StyleNode *node = styles; node; node = node->next)
        count++;

    const StyleNode **ordered = NULL;
    if (count)
    {
        ordered = malloc(count * sizeof(*ordered));
        if (!ordered)
            return 0;

        size_t i = 0;
        for (const StyleNode *node = styles; node; node = node->next)
            ordered[i++] = node;
    }

    int ok = appendHTMLString(buffer, "<style>\n");
    for (size_t i = count; ok && i > 0; i--)
    {
        const TagNode *tag = ordered[i - 1]->node;
        if (!tag)
            continue;

        int isLinkedStylesheet = tag->name &&
                                 !strcasecmp(tag->name, "link");
        for (const TagNode *child = tag->child; child; child = child->next)
        {
            if ((child->isText || isLinkedStylesheet) && child->content &&
                !appendHTMLString(buffer, child->content))
            {
                ok = 0;
                break;
            }
        }

        if (ok)
            ok = appendHTMLString(buffer, "\n");
    }
    if (ok)
        ok = appendHTMLString(buffer, "</style>\n");

    free(ordered);
    return ok;
}

static int isStylesheetLink(const TagNode *tag)
{
    if (!tag || !tag->name || strcasecmp(tag->name, "link") ||
        !tag->attributes)
        return 0;

    char *rel = getFromHashmap(tag->attributes, "rel");
    return rel && !strcasecmp(rel, "stylesheet") &&
           tag->child && tag->child->content;
}

static int hasChildNamed(const TagNode *tag, const char *name)
{
    for (const TagNode *child = tag ? tag->child : NULL; child; child = child->next)
    {
        if (!child->isText && child->name && !strcasecmp(child->name, name))
            return 1;
    }
    return 0;
}

static int containsTagNamed(const TagNode *nodes, const char *name)
{
    for (const TagNode *node = nodes; node; node = node->next)
    {
        if (!node->isText && node->name && !strcasecmp(node->name, name))
            return 1;
        if (containsTagNamed(node->child, name))
            return 1;
    }
    return 0;
}

static int appendParsedNode(HtmlBuffer *buffer, const TagNode *node,
                            const StyleNode *styles);

static int appendParsedElement(HtmlBuffer *buffer, const TagNode *node,
                               const StyleNode *styles)
{
    if (node->isText)
    {
        return appendHTMLString(buffer, node->content) &&
               appendParsedNode(buffer, node->child, styles);
    }

    if (!node->name)
        return 1;

    if (node->name[0] == '!')
        return appendParsedNode(buffer, node->child, styles);

    if (!strcasecmp(node->name, "style") || isStylesheetLink(node))
        return 1;

    if (!appendTagStart(buffer, node))
        return 0;

    if (!strcasecmp(node->name, "html") && !hasChildNamed(node, "head") &&
        (!appendHTMLString(buffer, "<head>\n") ||
         !appendParsedStyles(buffer, styles) ||
         !appendHTMLString(buffer, "</head>\n")))
        return 0;

    if (!strcasecmp(node->name, "head") &&
        !appendParsedStyles(buffer, styles))
        return 0;

    if (isVoidTag(node->name))
        return 1;

    if (!appendParsedNode(buffer, node->child, styles) ||
        !appendHTMLString(buffer, "</") ||
        !appendHTMLString(buffer, node->name) ||
        !appendHTMLString(buffer, ">"))
        return 0;

    return 1;
}

static int appendParsedNode(HtmlBuffer *buffer, const TagNode *node,
                            const StyleNode *styles)
{
    for (; node; node = node->next)
    {
        if (!appendParsedElement(buffer, node, styles))
            return 0;
    }
    return 1;
}

char *serializeParsedHTML(const Tab *tab)
{
    if (!tab || !tab->DOM)
        return NULL;

    HtmlBuffer buffer = {0};
    int hasHtml = 0;
    int hasHead = 0;
    int hasBody = 0;

    hasHtml = containsTagNamed(tab->DOM, "html");
    for (const TagNode *node = tab->DOM; node; node = node->next)
    {
        if (!node->isText && node->name)
        {
            hasHead |= !strcasecmp(node->name, "head");
            hasBody |= !strcasecmp(node->name, "body");
        }
    }

    int ok = appendHTMLString(&buffer, "<!doctype html>\n");
    if (ok && !hasHtml)
    {
        ok = appendHTMLString(&buffer, "<html>\n");
        if (ok && !hasHead)
            ok = appendHTMLString(&buffer, "<head>\n") &&
                 appendParsedStyles(&buffer, tab->stylenodes) &&
                 appendHTMLString(&buffer, "</head>\n");

        if (ok && hasHead)
        {
            for (const TagNode *node = tab->DOM; ok && node; node = node->next)
            {
                if (!node->isText && node->name &&
                    !strcasecmp(node->name, "head"))
                    ok = appendParsedElement(&buffer, node, tab->stylenodes);
            }
        }

        if (ok && hasBody)
        {
            for (const TagNode *node = tab->DOM; ok && node; node = node->next)
            {
                if (!node->isText && node->name &&
                    (!strcasecmp(node->name, "!doctype") ||
                     !strcasecmp(node->name, "head")))
                    continue;
                ok = appendParsedElement(&buffer, node, tab->stylenodes);
            }
        }
        else if (ok)
        {
            ok = appendHTMLString(&buffer, "<body>\n");

            for (const TagNode *node = tab->DOM; ok && node; node = node->next)
            {
                if (!node->isText && node->name &&
                    (!strcasecmp(node->name, "!doctype") ||
                     !strcasecmp(node->name, "head")))
                    continue;
                ok = appendParsedElement(&buffer, node, tab->stylenodes);
            }
        }

        if (ok && !hasBody)
            ok = appendHTMLString(&buffer, "</body>\n");
        if (ok)
            ok = appendHTMLString(&buffer, "</html>");
    }
    else if (ok)
    {
        ok = appendParsedNode(&buffer, tab->DOM, tab->stylenodes);
    }

    if (!ok)
    {
        free(buffer.data);
        return NULL;
    }

    return buffer.data;
}
