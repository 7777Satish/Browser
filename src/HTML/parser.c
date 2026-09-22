#include "HTML/parser.h"

void parseTag(TagNode *tag);

void freeLISTItems(ItemNode *LIST)
{
    if (!LIST)
        return;
    ItemNode *node = LIST;

    if (node->content)
        SDL_free(node->content);
    if (node->name)
        free(node->name);

    if (node->next)
        freeLISTItems(node->next);
    free(node);
}

void createDOM(char *file_content, Tab **tab)
{
    if (!tab || !*tab) return;

    if (!file_content)
        return;

    int i = 0;
    // int y = 0;
    size_t length = strlen(file_content);
    ItemNode *LIST = NULL;
    ItemNode *lastLISTItem = NULL;

    int isTag = 0;
    int currentTextLen = 10;
    int currentTextIndex = 0;
    char *currentText = malloc(currentTextLen + 1);
    if (!currentText)
    {
        fprintf(stderr, "createDOM: failed to allocate currentText");
        return;
    }

    currentText[currentTextIndex] = '\0';

    int currentTagLen = 10;
    int currentTagIndex = 0;
    char *currentTagText = malloc(currentTagLen + 1);
    if (!currentTagText)
    {
        fprintf(stderr, "createDOM: failed to allocate currentTagText");
        free(currentText);
        return;
    }

    currentTagText[currentTagIndex] = '\0';

    int isTextOnly = 0;
    int isComment = 0;
    int isScript = 0;

    while (file_content[i] != '\0')
    {
        char cc = file_content[i];
        if (cc == '\n' || cc == '\t' || (cc == ' ' && file_content[i + 1] == ' ') || (i > 0 && cc == ' ' && file_content[i - 1] == '\n'))
        {
            i += 1;
            continue;
        }

        if (isComment)
        {
            if (i + 2 < length && cc == '-' && file_content[i + 1] == '-' && file_content[i + 2] == '>')
            {
                isComment = 0;
                i += 3;
                continue;
            }
            i++;
            continue;
        }

        if (i + 3 < length && cc == '<' && file_content[i + 1] == '!' && file_content[i + 2] == '-' && file_content[i + 3] == '-')
        {
            isComment = 1;
            i += 4;
            continue;
        }

        if (isScript)
        {
            if (i + 8 < length && cc == '<' && file_content[i + 1] == '/' && file_content[i + 2] == 's' && file_content[i + 3] == 'c' && file_content[i + 4] == 'r' && file_content[i + 5] == 'i' && file_content[i + 6] == 'p' && file_content[i + 7] == 't' && file_content[i + 8] == '>')
            {
                isScript = 0;
                i += 9;
                continue;
            }
            i++;
            continue;
        }

        // if (i + 7 < length && cc == '<' && file_content[i + 1] == 's' && file_content[i + 2] == 'c' && file_content[i + 3] == 'r' && file_content[i + 4] == 'i' && file_content[i + 5] == 'p' && file_content[i + 6] == 't' && file_content[i + 7] == '>')
        // {
        //     isScript = 1;
        //     i += 8;
        //     continue;
        // }

        if (cc == '<' && file_content[i + 1] != '<' && file_content[i + 1] != ' ')
        {
            isTag = 1;
            i += 1;
            if (strlen(currentText) > 1)
            {
                ItemNode *t = calloc(1, sizeof(*t));

                if (!t)
                {
                    fprintf(stderr, "createDOM: failed to allocate t");
                    free(currentText);
                    free(currentTagText);
                    freeLISTItems(LIST);
                    return;
                }

                t->isText = 1;

                currentText[currentTextIndex] = '\0';
                t->content = SDL_strdup(currentText);
                if (!t->content)
                {
                    fprintf(stderr, "createDOM: failed to allocate t->content");
                    free(currentText);
                    free(currentTagText);
                    freeLISTItems(LIST);
                    free(t);
                    return;
                }

                t->next = NULL;
                t->prev = NULL;

                if (!LIST)
                {
                    LIST = t;
                    lastLISTItem = t;
                }
                else
                {
                    t->prev = lastLISTItem;
                    lastLISTItem->next = t;
                    lastLISTItem = t;
                }
            }

            currentText[0] = '\0';
            currentTextIndex = 0;
            continue;
        }

        if (cc == '>' && i > 0 && file_content[i - 1] != '>' && isTag)
        {

            isTag = 0;
            // printf("<%s>\n", currentTagText);

            // SDL_Color color = {0, 0, 0, 255};
            ItemNode *t = calloc(1, sizeof(*t));

            if (!t)
            {
                fprintf(stderr, "createDOM: failed to allocate t");
                free(currentText);
                free(currentTagText);
                freeLISTItems(LIST);
                return;
            }

            t->isText = 0;
            if (currentTagText[0] == '/')
            {
                t->isClosing = 1;

                currentTagText[currentTagIndex] = '\0';
                t->content = SDL_strdup(currentTagText);

                if (!t->content)
                {
                    fprintf(stderr, "createDOM: failed to allocate t->content");
                    free(currentText);
                    free(currentTagText);
                    freeLISTItems(LIST);
                    free(t);
                    return;
                }
            }
            else
            {
                t->isClosing = 0;

                currentTagText[currentTagIndex] = '\0';
                t->content = SDL_strdup(currentTagText);

                if (!t->content)
                {
                    fprintf(stderr, "createDOM: failed to allocate t->content");
                    free(currentText);
                    free(currentTagText);
                    freeLISTItems(LIST);
                    free(t);
                    return;
                }
                if (!strncasecmp(t->content, "script", 6))
                    isScript = 1;
            }

            t->next = NULL;
            t->prev = NULL;

            if (!LIST)
            {
                LIST = t;
                lastLISTItem = t;
            }
            else
            {
                t->prev = lastLISTItem;
                lastLISTItem->next = t;
                lastLISTItem = t;
            }

            currentTagText[0] = '\0';
            currentTagIndex = 0;
            i += 1;
            continue;
        }

        if (currentTextIndex >= currentTextLen - 1)
        {
            currentTextLen *= 2;
            char *newText = realloc(currentText, currentTextLen + 1);
            if (!newText)
            {
                fprintf(stderr, "createDOM: failed to reallocate newText");
                free(currentText);
                free(currentTagText);
                freeLISTItems(LIST);
                return;
            }
            currentText = newText;
        }

        if (currentTagIndex >= currentTagLen - 1)
        {
            currentTagLen *= 2;
            char *newText = realloc(currentTagText, currentTagLen + 1);

            if (!newText)
            {
                fprintf(stderr, "createDOM: failed to reallocate newText");
                free(currentText);
                free(currentTagText);
                freeLISTItems(LIST);
                return;
            }

            currentTagText = newText;
        }

        if (!isTag)
        {
            currentText[currentTextIndex++] = cc;
            currentText[currentTextIndex] = '\0';
        }
        else
        {
            currentTagText[currentTagIndex++] = cc;
            currentTagText[currentTagIndex] = '\0';
        }

        i++;
    }

    free(currentText);
    free(currentTagText);

    ItemNode *node = LIST;

    TagNode *list = NULL;
    TagNode *currentParent = list;
    while (node != NULL)
    {

        TagNode *temp = (TagNode *)calloc(1, sizeof(TagNode));
        if (!temp)
        {
            fprintf(stderr, "createDOM: failed to allocate temp");
            freeLISTItems(LIST);
            return;
        }

        temp->parent = currentParent;
        temp->isText = node->isText;
        temp->isClosing = node->isClosing;
        // if(node->name){
        //     size_t size = strlen(node->name);
        //     temp->name = malloc(size+1);
        //     strcpy(temp->name, node->name);
        // }

        if (node->content)
        {
            size_t size = strlen(node->content);
            // temp->content = malloc(size + 1);
            temp->content = SDL_strdup(node->content);

            if (!temp->content)
            {
                fprintf(stderr, "createDOM: failed to allocate temp->content");
                freeLISTItems(LIST);
                freeDOM(list);
                free(temp);
                return;
            }

            // strcpy(temp->content, node->content);
            // printf("%s\n", node->content);
            parseTag(temp);
        }

        if (list == NULL)
        {
            list = temp;

            node = node->next;
            currentParent = temp;
            continue;
        }

        if (node->isText)
        {
            if (!currentParent->child)
            {
                currentParent->child = temp;
            }
            else
            {
                TagNode *abc = currentParent->child;
                while (abc->next != NULL)
                {
                    abc = abc->next;
                }
                abc->next = temp;
                temp->prev = abc;
            }

            temp->parent = currentParent;

            node = node->next;
            continue;
        }

        if (!node->isClosing)
        {
            // printf("%s, %s\n", currentParent ? currentParent->content : "NULL", temp->content);

            temp->parent = currentParent;

            if (!currentParent->child)
            {
                currentParent->child = temp;
            }
            else
            {
                TagNode *abc = currentParent->child;
                while (abc->next != NULL)
                {
                    abc = abc->next;
                }
                abc->next = temp;
                temp->prev = abc;
            }

            currentParent = temp;

            if (temp->name && !strcasecmp(temp->name, "style"))
            {
                StyleNode *node = calloc(sizeof(StyleNode), 1);

                if (!node)
                {
                    fprintf(stderr, "createDOM: failed to allocate node");
                    freeLISTItems(LIST);
                    freeDOM(list);
                    SDL_free(temp->content);
                    free(temp);
                    return;
                }

                node->node = temp;
                node->next = (*tab)->stylenodes;
                (*tab)->stylenodes = node;
            }
        }

        if (node->isClosing || isVoidTag(temp->name))
        {
            if (currentParent && currentParent->name && !strcasecmp(currentParent->name, "title"))
            {
                SDL_Surface *s = TTF_RenderText_Blended(poppins_regular, currentParent->child->content, (SDL_Color){255, 255, 255, 255});

                if (!s)
                {
                    fprintf(stderr, "createDOM: failed to create surface s");
                    freeLISTItems(LIST);
                    freeDOM(list);
                    SDL_free(temp->content);
                    free(temp);
                    return;
                }


                if (s)
                    (*tab)->s1 = s;
                else
                    (*tab)->s1 = NULL;
            }

            if (currentParent)
            {
                TagNode *tempParent = currentParent;
                int found = 0;
                while (currentParent->parent)
                {
                    if (!strcmp(currentParent->name, temp->name))
                    {
                        found = 1;
                        currentParent = currentParent->parent;
                        break;
                    }
                    currentParent = currentParent->parent;
                }

                if (!found)
                    currentParent = tempParent;
            }
        }

        node = node->next;
    }

    (*tab)->DOM = list;

    StyleNode *stylenode = (*tab)->stylenodes;
    CSSBlockNode *finalCSS = NULL;
    while (stylenode)
    {
        CSSBlockNode *l = NULL;
        if (stylenode->node && stylenode->node->child && stylenode->node->child->content)
        {
            // printf("%s %d\n", stylenode->node->child->content, (int)strlen(stylenode->node->child->content));
            l = parseFromStyle(stylenode->node->child->content);
        }

        // if(l){
        //     printParsedStyles(l, 0);
        // }

        finalCSS = insertToCSS(finalCSS, l);

        stylenode = stylenode->next;
    }
    // printParsedStyles(finalCSS, 0);
    (*tab)->CSOM = finalCSS;
    applyCSOMtoDOM((*tab)->DOM, (*tab)->CSOM);

    parseCSS(list);
    double width = 0, height = 0;
    (*tab)->MAXHEIGHT = 0;

    layout(list, 0, 0, &width, &height);
    printlist(list, (*tab)->stylenodes, 0);

    printf("\n\n\n\n\nNEWTAB\n\n\n");

    // double wdth = 0, hgt = 0;
    // (*tab)->LAYOUT = createLayoutTree(list, NULL);
    // BFCLayout((*tab)->LAYOUT, 0, 0, &wdth, &hgt);
    // printf("\n\n");
    // printLayoutTree((*tab)->LAYOUT, 0);
}

void freeDOM(TagNode *DOM)
{
    if (!DOM)
        return;
    TagNode *node = DOM;

    if(node->t1) SDL_DestroyTexture(node->t1);

    freeDOM(node->child);
    freeDOM(node->next);
    free(node);
}

void changeLayout(LayoutNode *layout)
{
    if (!layout)
        return;
    if (layout->child)
    {

        if (layout->tag && layout->tag->style.displayOuter == DISPLAY_OUTER_INLINE)
        {
            if (layout->child->tag->style.displayOuter == DISPLAY_OUTER_BLOCK && !layout->child->tag->isText)
            {
                layout->child->tag->name = SDL_strdup("<Altered Item>");
            }
        }

        changeLayout(layout->child);
    }

    changeLayout(layout->next);
}

void parseTag(TagNode *tag)
{
    if (tag->isText)
        return;
    if (tag->isClosing)
    {
        tag->content += 1;
    }
    char *name = malloc(1);
    name[0] = '\0';
    int len = 1;
    while (tag->content[0])
    {
        char *nm = realloc(name, len + 2);
        name = nm;
        name[len - 1] = tag->content[0];
        name[len] = '\0';
        len++;
        tag->content += 1;
        if (tag->content[0] == ' ' || tag->content[0] == '/')
        {
            tag->content += 1;
            break;
        }
    }
    tag->name = SDL_strdup(name);
    free(name);

    int ind = 0;

    while (tag->content[ind])
    {
        while (tag->content[ind] && (tag->content[ind] == ' ' || tag->content[ind] == '\n'))
        {
            ind++;
        }

        char v[1024] = "";
        int vInd = 0;
        while (tag->content[ind] && tag->content[ind] != '=' && tag->content[ind] != ' ' && tag->content[ind] != '>')
        {
            v[vInd++] = tag->content[ind];
            ind++;
        }

        v[vInd] = '\0';

        attributeNode *attr = (attributeNode *)malloc(sizeof(attributeNode));
        attr->name = SDL_strdup(v);
        attr->len = 0;

        while (tag->content[ind] && (tag->content[ind] == ' ' || tag->content[ind] == '\n'))
        {
            ind++;
        }

        if (tag->content[ind] && tag->content[ind] == '=')
        {
            ind++;
            while (tag->content[ind] && (tag->content[ind] == ' ' || tag->content[ind] == '\n'))
            {
                ind++;
            }

            char terminator = ' ';

            if (tag->content[ind] && tag->content[ind] == '"')
                terminator = '"';
            else if (tag->content[ind] && tag->content[ind] == '\'')
                terminator = '\'';

            ind++;

            attr->value = tag->content + ind;
            int valueLen = 0;

            while (tag->content[ind] && tag->content[ind] != terminator)
            {
                valueLen++;
                ind++;
            }

            attr->len = valueLen;

            ind++;
        }

        if (vInd > 0 && !strcasecmp(attr->name, "id"))
        {
            tag->attr_id = malloc(attr->len + 1);
            memcpy(tag->attr_id, attr->value, attr->len);
            tag->attr_id[attr->len] = '\0';
        }

        if (vInd > 0 && !strcasecmp(attr->name, "class"))
        {
            if (attr->len)
            {
                int i = 0;
                while (i < attr->len)
                {
                    while (i < attr->len && attr->value[i] == ' ')
                    {
                        i++;
                    }

                    ContentNode *class = (ContentNode *)malloc(sizeof(ContentNode));
                    class->content = NULL;
                    class->next = tag->classes;
                    int len = 0;
                    while (i < attr->len && attr->value[i] != ' ')
                    {
                        len++;
                        i++;
                    }

                    if (len)
                    {
                        class->len = len;

                        class->content = malloc(attr->len + 1);
                        memcpy(class->content, attr->value, attr->len);
                        class->content[attr->len] = '\0';

                        tag->classes = class;
                    }
                    else
                    {
                        free(class);
                    }
                }
            }
        }
    }
}
