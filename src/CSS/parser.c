#include "CSS/parser.h"
#include "HTML/parser.h"

void parseCSS(TagNode *DOM);
void parseStyle(TagNode *tag);
int letterToIntColor(char v);
int parseInt(char *str, int start, int end);
void parseColor(char *str, int start, int end, int *r, int *g, int *b, int *a);
void parseRGB(char *str, int start, int end, int *r, int *g, int *b);
CSSBlockNode *parseFromStyle(const char *source);
void applyCSOMtoDOM(TagNode *DOM, CSSBlockNode *CSOM);
CSSBlockNode *insertToCSS(CSSBlockNode *final, CSSBlockNode *current);

CSSBlockNode *parseFromStyle(const char *source)
{
    if (!source)
        return NULL;

    int i = 0;
    int skip = 0;
    int bracedepth = 0;
    size_t len = strlen(source);

    int currentWordLen = 10;
    int currentWordIndex = 0;
    char *currentWord = malloc(currentWordLen + 1);
    currentWord[currentWordIndex] = '\0';

    CSSBlockNode *list = NULL;
    CSSBlockNode *listTail = NULL;

    int newLine = 1;
    CSSBlockNode *line = NULL;
    CSSBlockNode *lineTail = NULL;

    int currentListLength = 0;
    int currentStart = 0;

    int isComment = 0;
    while (i <= len)
    {
        char cc = source[i];
        if (cc == '\n' || cc == '\t' || (cc == ' ' && source[i + 1] == ' ') || (cc == ' ' && source[i + 1] == '{') || (i > 0 && cc == ' ' && source[i - 1] == '\n') || (i > 0 && cc == ' ' && source[i + 1] == '\n'))
        {
            i++;
            continue;
        }

        if (isComment)
        {
            if (cc == '*' && source[i + 1] == '/')
            {
                isComment = 0;
                i += 2;
                continue;
            }
            i++;
            continue;
        }

        if (cc == '/' && source[i + 1] == '*')
        {
            isComment = 1;
            i += 2;
            continue;
        }

        if (!skip && (cc == ' ' || cc == '{'))
        {
            if (cc == ' ' && !currentWordIndex)
            {
                i++;
                continue;
            }
            CSSBlockNode *temp = calloc(1, sizeof(CSSBlockNode));
            currentWord[currentWordIndex] = '\0';
            temp->name = SDL_strdup(currentWord);

            currentWordIndex = 0;
            currentWord[0] = '\0';

            if (!newLine)
            {
                lineTail->child = temp;
                lineTail = temp;
            }
            else
            {
                lineTail = temp;

                if (listTail)
                    listTail->next = temp;
                listTail = temp;

                newLine = 0;
            }

            if (!list)
                list = temp;

            if (cc == '{')
            {
                if (!skip)
                {
                    lineTail->content = (char *)(source + i + 1);
                    skip = 1;
                    currentStart = i;
                    // printf("\n%d\n", strlen(source[i+1]));
                }
                else
                    bracedepth++;
            }

            i++;
            continue;
        }

        if (skip && cc == '{')
        {
            bracedepth++;
        }

        if (cc == '}')
        {
            if (skip)
            {
                if (bracedepth > 0)
                {
                    bracedepth--;
                }
                else
                {
                    skip = 0;
                    newLine = 1;
                    lineTail->length = i - currentStart;
                }
            }

            i++;
            continue;
        }

        if (!skip)
        {
            if (currentWordIndex >= currentWordLen)
            {
                currentWordLen *= 2;
                char *str = realloc(currentWord, currentWordLen + 1);
                currentWord = str;
            }
            currentWord[currentWordIndex++] = cc;
            currentWord[currentWordIndex] = '\0';
        }

        i++;
        continue;
    }

    // printParsedStyles(list);

    free(currentWord);

    return list;
}

CSSBlockNode *insertToCSS(CSSBlockNode *final, CSSBlockNode *current)
{

    CSSBlockNode *node = current;

    while (node)
    {

        CSSBlockNode *child = node;
        CSSBlockNode *listParent = NULL;
        CSSBlockNode *list = final;

        while (child)
        {
            if (child->name)
            {

                CSSBlockNode *temp = (CSSBlockNode *)malloc(sizeof(CSSBlockNode));
                temp->name = child->name;
                // temp->length = strlen(child->name);
                temp->length = child->length;
                if (child->content && child->length)
                    temp->content = child->content;
                temp->next = NULL;
                temp->child = NULL;

                if (!final)
                {
                    final = temp;
                    list = final;

                    listParent = list;
                    list = NULL;

                    child = child->child;
                    continue;
                }

                if (!list)
                {
                    listParent->child = temp;
                    list = temp;

                    listParent = list;
                    list = NULL;

                    child = child->child;
                    continue;
                }

                CSSBlockNode *i = list;

                int found = 0;
                while (i->next)
                {
                    if (!strcasecmp(i->name, child->name))
                    {
                        found = 1;
                        list = i;
                        break;
                    }
                    i = i->next;
                }

                if (!strcasecmp(i->name, child->name))
                {
                    found = 1;
                    list = i;
                }

                if (found)
                {
                    if (temp->content && temp->length)
                    {
                        list->content = temp->content;
                        list->length = temp->length;
                    }

                    free(temp);
                    listParent = list;
                    list = list->child;
                }
                else
                {
                    i->next = temp;
                    listParent = temp;
                    list = NULL;
                }
            }
            child = child->child;
        }
        node = node->next;
    }

    return final;
}

void applyCSOMtoDOM(TagNode *DOM, CSSBlockNode *CSSOM)
{
    if (!CSSOM)
        return;
    TagNode *node = DOM;

    while (node)
    {
        CSSBlockNode *cnode = CSSOM;
        ContentNode *tail = node->styleContentNodes;

        CSSBlockNode *level = CSSOM;

        while (cnode)
        {
            if (node->name && cnode->name && !strcasecmp(node->name, cnode->name))
            {
                // printf("%s : %.*s\n", node->name, cnode->length, cnode->content);
                ContentNode *temp = (ContentNode *)malloc(sizeof(ContentNode));
                temp->content = cnode->content;
                temp->len = cnode->length;
                temp->next = NULL;
                if (!tail)
                {
                    node->styleContentNodes = temp;
                    tail = temp;
                }
                else
                {
                    tail->next = temp;
                    tail = temp;
                }

                CSSBlockNode *child = cnode->child;
                CSSBlockNode *lvl = level;
                while (child)
                {
                    CSSBlockNode *l = (CSSBlockNode *)malloc(sizeof(CSSBlockNode));
                    l->child = child->child;
                    l->content = child->content;
                    l->length = child->length;
                    l->name = child->name;
                    l->next = lvl;
                    lvl = l;

                    child = child->next;
                }

                level = lvl;
            }

            ContentNode *class = node->classes;
            while (class)
            {
                if (class->len && cnode->name && cnode->name[0] == '.' && !strcasecmp(class->content, cnode->name + 1))
                {
                    // printf("%s _ %s: %.*s\n", node->name, cnode->name, cnode->length, cnode->content);
                    ContentNode *temp = (ContentNode *)malloc(sizeof(ContentNode));
                    temp->content = cnode->content;
                    temp->len = cnode->length;
                    temp->next = NULL;
                    if (!tail)
                    {
                        node->styleContentNodes = temp;
                        tail = temp;
                    }
                    else
                    {
                        tail->next = temp;
                        tail = temp;
                    }

                    CSSBlockNode *child = cnode->child;
                    CSSBlockNode *lvl = level;
                    while (child)
                    {
                        CSSBlockNode *l = (CSSBlockNode *)malloc(sizeof(CSSBlockNode));
                        l->child = child->child;
                        l->content = child->content;
                        l->length = child->length;
                        l->name = child->name;
                        l->next = lvl;
                        lvl = l;
                        child = child->next;
                    }

                    level = lvl;
                }

                class = class->next;
            }

            if (node->attr_id && cnode->name && !strcasecmp(node->attr_id, cnode->name + 1))
            {
                // printf("%s\n", node->name);
                ContentNode *temp = (ContentNode *)malloc(sizeof(ContentNode));
                temp->content = cnode->content;
                temp->len = cnode->length;
                temp->next = NULL;
                if (!tail)
                {
                    node->styleContentNodes = temp;
                    tail = temp;
                }
                else
                {
                    tail->next = temp;
                    tail = temp;
                }

                CSSBlockNode *child = cnode->child;
                CSSBlockNode *lvl = level;
                while (child)
                {
                    CSSBlockNode *l = (CSSBlockNode *)malloc(sizeof(CSSBlockNode));
                    l->child = child->child;
                    l->content = child->content;
                    l->length = child->length;
                    l->name = child->name;
                    l->next = lvl;
                    lvl = l;

                    child = child->next;
                }

                level = lvl;
            }

            cnode = cnode->next;
        }

        if (node->child)
        {
            applyCSOMtoDOM(node->child, level);
            // printf("%s Child: %s\n", node->name, node->child->name);
            // free(level);
        }

        node = node->next;
    }
}

void parseCSSProperties(TagNode *tag, char *content, int ind, int len)
{
    if (!tag || !content || !len)
        return;

    char key[100];
    int keyInd = 0;
    char value[1024];
    int valueInd = 0;
    int which = 0;

    // printf("%s %.*s\n", tag->name, len, content);
    int start = ind;
    while (ind < len + start && content[ind] != '\0')
    {

        char c = content[ind];
        if (c == ' ')
        {
            ind++;
            continue;
        }

        if (c == ':')
        {
            which = 1;
            valueInd = 0;
            value[0] = '\0';
        }
        else if (c == ';')
        {
            // printf("\n%s :: %s\n", key, value);

            int v = parseInt(value, 0, valueInd);
            if (strcmp(key, "width") == 0)
            {
                tag->style.width = v;
            }
            else if (strcmp(key, "height") == 0)
            {
                tag->style.height = v;
            }
            else if (strcmp(key, "max-width") == 0)
            {
                tag->style.maxHeight = v;
            }
            else if (strcmp(key, "max-height") == 0)
            {
                tag->style.maxWidth = v;
            }
            else if (strcmp(key, "display") == 0)
            {
                if (strcmp(value, "flex") == 0)
                {
                    tag->style.displayInner = DISPLAY_INNER_FLEX;
                    tag->style.displayOuter = DISPLAY_OUTER_BLOCK;
                    tag->style.flexDirection = 0;
                }
                if (strcmp(value, "block") == 0)
                {
                    tag->style.displayInner = DISPLAY_INNER_FLOW;
                    tag->style.displayOuter = DISPLAY_OUTER_BLOCK;
                    tag->style.flexDirection = 0;
                }

                if (strcmp(value, "inline") == 0)
                {
                    tag->style.displayInner = DISPLAY_INNER_FLOW;
                    tag->style.displayOuter = DISPLAY_OUTER_INLINE;
                    tag->style.flexDirection = 0;
                }
            }
            else if (strcmp(key, "gap") == 0)
            {
                tag->style.gap = v;
            }
            else if (strcmp(key, "flex-direction") == 0)
            {
                if (strcmp(value, "column") == 0)
                {
                    tag->style.flexDirection = 1;
                }
            }
            else if (strcmp(key, "align-items") == 0)
            {
                if (strcmp(value, "flex-start") == 0)
                    tag->style.alignItems = 1;

                if (strcmp(value, "center") == 0)
                    tag->style.alignItems = 2;
            }
            else if (strcmp(key, "line-height") == 0)
            {
                if (v)
                    tag->style.lineheight = v;
            }
            else if (strcmp(key, "font-size") == 0)
            {
                if (v)
                {
                    tag->style.fontsize = v;
                }
            }
            else if (strcmp(key, "font-weight") == 0)
            {
                if (!strcasecmp(value, "bold"))
                {
                    tag->style.fontweight = 400;
                }
            }
            else if (strcmp(key, "border-radius") == 0)
            {
                tag->style.borderRadius = v;
            }
            else if (strcmp(key, "margin-left") == 0)
            {
                tag->style.marginleft = v;
            }
            else if (strcmp(key, "margin-right") == 0)
            {
                tag->style.marginright = v;
            }
            else if (strcmp(key, "margin-top") == 0)
            {
                tag->style.margintop = v;
            }
            else if (strcmp(key, "margin-bottom") == 0)
            {
                tag->style.marginbottom = v;
            }
            else if (strcmp(key, "padding-left") == 0)
            {
                tag->style.paddingleft = v;
            }
            else if (strcmp(key, "padding-right") == 0)
            {
                tag->style.paddingright = v;
            }
            else if (strcmp(key, "padding-top") == 0)
            {
                tag->style.paddingtop = v;
            }
            else if (strcmp(key, "padding-bottom") == 0)
            {
                tag->style.paddingbottom = v;
            }
            else if (strcmp(key, "text-align") == 0)
            {
                if (strcasecmp(value, "center") == 0)
                {
                    tag->style.textalign = 1;
                }
                else if (strcasecmp(value, "right") == 0)
                {
                    tag->style.textalign = 2;
                }
            }
            else if (strcmp(key, "margin") == 0)
            {
                int ind1 = 0;
                int last = -1;
                int lastInd = 0;
                int i = 0;
                int values[4] = {0};
                while (i < valueInd)
                {
                    if (isdigit(value[i]))
                    {
                        ind1 = 0;
                    }
                    else
                    {
                        if (ind1 == 0)
                        {
                            last++;
                            // printf("--- %d %d\n", lastInd, i);
                            values[last] = parseInt(value, lastInd, i);
                        }
                        lastInd = i;
                        ind1 = 1;
                    }
                    i++;
                }
                if (last == 0)
                {
                    tag->style.margin = values[0];
                    tag->style.margintop = values[0];
                    tag->style.marginright = values[0];
                    tag->style.marginbottom = values[0];
                    tag->style.marginleft = values[0];
                }
                else if (last == 1)
                {
                    tag->style.margintop = values[0];
                    tag->style.marginright = values[1];
                    tag->style.marginbottom = values[0];
                    tag->style.marginleft = values[1];
                }
                else
                {
                    tag->style.margintop = values[0];
                    tag->style.marginright = values[1];
                    tag->style.marginbottom = values[2];
                    tag->style.marginleft = values[3];
                }
            }
            else if (strcmp(key, "padding") == 0)
            {
                int ind1 = 0;
                int last = -1;
                int lastInd = 0;
                int i = 0;
                int values[4] = {0};
                while (i < valueInd)
                {
                    if (isdigit(value[i]))
                    {
                        ind1 = 0;
                    }
                    else
                    {
                        if (ind1 == 0)
                        {
                            last++;
                            values[last] = parseInt(value, lastInd, i);
                        }
                        lastInd = i;
                        ind1 = 1;
                    }
                    i++;
                }
                if (last == 0)
                {
                    tag->style.padding = values[0];
                    tag->style.paddingtop = values[0];
                    tag->style.paddingright = values[0];
                    tag->style.paddingbottom = values[0];
                    tag->style.paddingleft = values[0];
                }
                else if (last == 1)
                {
                    tag->style.paddingtop = values[0];
                    tag->style.paddingright = values[1];
                    tag->style.paddingbottom = values[0];
                    tag->style.paddingleft = values[1];
                }
                else
                {
                    tag->style.paddingtop = values[0];
                    tag->style.paddingright = values[1];
                    tag->style.paddingbottom = values[2];
                    tag->style.paddingleft = values[3];
                }
            }
            else if (strcmp(key, "color") == 0)
            {
                int r, g, b, a;
                parseColor(value, 0, valueInd, &r, &g, &b, &a);
                tag->style.color = (SDL_Color){r, g, b, a};

                // printf("\nRGB : %d %d %d\n", r, g, b);
            }
            else if (strcmp(key, "background") == 0)
            {
                int r, g, b, a;
                parseColor(value, 0, valueInd, &r, &g, &b, &a);
                tag->style.background = (SDL_Color){r, g, b, a};
                // printf("\nRGB : %d %d %d\n", r, g, b);
            }
            else if (strcmp(key, "border") == 0)
            {
                tag->style.border = 1;
            }
            else if (strcmp(key, "border-top") == 0)
            {
                tag->style.bordertop = 1;
            }
            else if (strcmp(key, "border-right") == 0)
            {
                tag->style.borderright = 1;
            }
            else if (strcmp(key, "border-bottom") == 0)
            {
                tag->style.borderbottom = 1;
            }
            else if (strcmp(key, "border-left") == 0)
            {
                tag->style.borderleft = 1;
            }

            which = 0;
            keyInd = 0;
            key[0] = '\0';
            valueInd = 0;
            value[0] = '\0';
        }
        else
        {
            if (!which)
            {
                key[keyInd++] = c;
                key[keyInd] = '\0';
            }
            else
            {
                value[valueInd++] = c;
                value[valueInd] = '\0';
            }
        }

        ind++;
    }
}

void parseStyle(TagNode *tag)
{
    if (tag->parent)
    {
        tag->style.color = tag->parent->style.color;
        // tag->style.background = tag->parent->style.background;
        tag->style.fontsize = tag->parent->style.fontsize;
        tag->style.fontweight = tag->parent->style.fontweight;
        tag->style.textalign = tag->parent->style.textalign;
        tag->style.lineheight = tag->parent->style.lineheight;
    }
    else
    {
        tag->style.color = (SDL_Color){20, 20, 20, 255};
        // tag->style.background = (SDL_Color){255, 255, 255, 255};
        tag->style.fontsize = BORDER_ICON_H + 1;
        tag->style.textalign = 0;
    }

    tag->style.width = 0;

    tag->style.displayInner = DISPLAY_INNER_FLOW;
    tag->style.displayOuter = DISPLAY_OUTER_BLOCK;
    tag->style.flexDirection = 0;
    tag->style.marginleft = 0;
    tag->style.marginright = 0;
    tag->style.margintop = 0;
    tag->style.marginbottom = 0;

    tag->style.paddingleft = 0;
    tag->style.paddingright = 0;
    tag->style.paddingtop = 0;
    tag->style.paddingbottom = 0;

    // printf("%s|%s\n\n", tag->name, tag->content);

    // if (strcmp(tag->name, "div") == 0 || strcmp(tag->name, "h1") == 0 || strcmp(tag->name, "p") == 0 || strcmp(tag->name, "section") == 0 || strcmp(tag->name, "footer") == 0 || strcmp(tag->name, "nav") == 0 || strcmp(tag->name, "header") == 0)
    // {
    // tag->style.display = BLOCK;
    // }
    // else
    // {
    //     tag->type = 1;
    //     tag->style.display = INLINE;
    // }

    if (tag->name && strcasecmp(tag->name, "h1") == 0)
    {
        tag->style.fontsize = 25;
        tag->style.fontweight = 500;
    }
    if (tag->name && strcasecmp(tag->name, "h2") == 0)
    {
        tag->style.fontsize = 23;
        tag->style.fontweight = 400;
    }
    if (tag->name && strcasecmp(tag->name, "h3") == 0)
    {
        tag->style.fontsize = 21;
        tag->style.fontweight = 400;
    }
    if (tag->name && strcasecmp(tag->name, "h4") == 0)
    {
        tag->style.fontsize = 18;
        tag->style.fontweight = 400;
    }
    if (tag->name && strcasecmp(tag->name, "h5") == 0)
    {
        tag->style.fontsize = 16;
        tag->style.fontweight = 400;
    }
    if (tag->name && strcasecmp(tag->name, "h6") == 0)
    {
        tag->style.fontsize = 14;
        tag->style.fontweight = 400;
    }
    if (tag->name && strcasecmp(tag->name, "b") == 0)
    {
        tag->style.fontweight = 400;
    }
    if (tag->name && strcasecmp(tag->name, "p") == 0)
    {
        tag->style.margintop = 16;
        tag->style.marginbottom = 16;
    }
    if (tag->name && strcasecmp(tag->name, "button") == 0)
    {
        tag->style.border = 1;
        tag->style.paddingleft = 6;
        tag->style.paddingtop = 1;
        tag->style.paddingright = 6;
        tag->style.paddingbottom = 1;
        tag->style.background = (SDL_Color){239, 239, 239, 255};
        tag->style.displayOuter = DISPLAY_OUTER_INLINE;
        tag->style.displayInner = DISPLAY_INNER_FLOW;
    }
    if (tag->name && (!strcasecmp(tag->name, "span") || !strcasecmp(tag->name, "a") || !strcasecmp(tag->name, "b") || !strcasecmp(tag->name, "i") || !strcasecmp(tag->name, "u")))
    {
        tag->style.displayOuter = DISPLAY_OUTER_INLINE;
        tag->style.displayInner = DISPLAY_INNER_FLOW;
    }
    if (tag->name && strcasecmp(tag->name, "tr") == 0)
    {
        tag->style.displayOuter = DISPLAY_OUTER_BLOCK;
        tag->style.displayInner = DISPLAY_INNER_FLEX;
    }
    if (tag->name && strcasecmp(tag->name, "li") == 0)
    {
        tag->style.paddingtop = 10;
        tag->style.paddingbottom = 10;
        tag->style.paddingleft = 20;
    }
    // if (tag->parent)
    //     tag->style.width = tag->parent->style.width;
    // else
    //     tag->style.width = WINDOW_W;

    // Parse Style
    ContentNode *cnode = tag->styleContentNodes;
    // int i = 0;
    while (cnode)
    {
        // i++;
        // printf("%d. %s __ %.*s\n", i, tag->name, cnode->len, cnode->content);
        parseCSSProperties(tag, cnode->content, 0, cnode->len);
        cnode = cnode->next;
    }

    // Parse inline style
    int isInside = 0;
    int ind = 0;
    size_t ln = strlen(tag->content);
    while (!isInside && ind + 6 < ln)
    {
        if (isStyle(tag->content, ind))
        {
            isInside = 1;
            ind += 7;
            break;
        }
        ind++;
    }

    if (!isInside)
        return;

    int len = ind;
    while (tag->content[len] != '"' && tag->content[len] != '\0')
    {
        len++;
    }

    // printf("%s : %s\n", tag->name, tag->content + ind);
    parseCSSProperties(tag, tag->content, ind, len - ind);
}

void parseCSS(TagNode *DOM)
{
    if (!DOM)
        return;
    if (DOM->content)
        parseStyle(DOM);
    if (DOM->child)
        parseCSS(DOM->child);
    if (DOM->next)
        parseCSS(DOM->next);
}

void parseColor(char *str, int start, int end, int *r, int *g, int *b, int *a)
{
    while (start < end && str[start] != ';' && str[start] == ' ')
    {
        start++;
    }

    if (start >= end)
        return;

    if (!strncasecmp(str, "rgb(", 4))
    {
        parseRGB(str, start + 4, end, r, g, b);
        *a = 255;
        return;
    }
    else if (!strncasecmp(str, "rgba(", 5))
    {
        parseRGB(str, start + 4, end, r, g, b);
        return;
    }
    else if (str[start] == '#')
    {
        parseHex(str, start + 1, end, r, g, b, a);
    }
}

void parseRGB(char *str, int start, int end, int *r, int *g, int *b)
{
    int i = start;
    int values[10] = {0};
    int ind = -1;
    int dirty = 0;
    int lastInd = i;
    while (i < end && ind < 3)
    {
        if (!isdigit(str[i]))
        {
            if (dirty == 0)
            {
                ind++;
                values[ind] = parseInt(str, lastInd, i);
            }
            lastInd = i;
            dirty = 1;
        }
        else
        {
            dirty = 0;
        }
        i++;
    }

    *r = values[0];
    *g = values[1];
    *b = values[2];

    // printf("\nRGB : %d %d %d\n", r, g, b);
}

void parseRGBA(char *str, int start, int end, int *r, int *g, int *b, int *a)
{
    int i = start;
    int values[10] = {0};
    int ind = -1;
    int dirty = 0;
    int lastInd = i;
    while (i < end && ind < 3)
    {
        if (!isdigit(str[i]))
        {
            if (dirty == 0)
            {
                ind++;
                values[ind] = parseInt(str, lastInd, i);
            }
            lastInd = i;
            dirty = 1;
        }
        else
        {
            dirty = 0;
        }
        i++;
    }

    *r = values[0];
    *g = values[1];
    *b = values[2];
    *a = values[3];
    // printf("\nRGB : %d %d %d\n", r, g, b);
}

void parseHex(char *str, int start, int end, int *r, int *g, int *b, int *a)
{

    if (end - start != 4 || end - start != 8)
        *a = 255;

    if (end - start == 3 || end - start == 4)
    {
        int red = letterToIntColor(str[start]);
        *r = (red + 0.0) / 15 * 255;
        
        int green = letterToIntColor(str[start+1]);
        *g = (green + 0.0) / 15 * 255;

        int blue = letterToIntColor(str[start+2]);
        *b = (blue + 0.0) / 15 * 255;
    }

    if (end - start == 4)
    {
        int alpha = letterToIntColor(str[start+3]);
        *a = (alpha + 0.0) / 15 * 255;
    }

    if (end - start == 6 || end - start == 8)
    {
        int r1 = letterToIntColor(str[start]);
        int r2 = letterToIntColor(str[start+1]);
        *r = r1*16+r2;
        
        int g1 = letterToIntColor(str[start+2]);
        int g2 = letterToIntColor(str[start+3]);
        *g = g1*16+g2;

        int b1 = letterToIntColor(str[start+4]);
        int b2 = letterToIntColor(str[start+5]);
        *b = b1*16+b2;
    }

    if(end - start == 8){
        int a1 = letterToIntColor(str[start+6]);
        int a2 = letterToIntColor(str[start+7]);
        *a = a1*16+a2;
    }
}

int letterToIntColor(char v)
{
    int o = 0;
    if (v >= '0' && v <= '9')
    {
        o = (int)v - (int)'0';
    }
    else
    {
        o = (int)v - (int)'a' + 10;
    }

    return o;
}

int parseInt(char *str, int start, int end)
{
    int i = start;
    int num = 0;
    while (i < end)
    {
        if (isdigit(str[i]))
        {
            num *= 10;
            int digit = str[i] - '0';
            num += digit;
        }
        i++;
    }
    return num;
}
