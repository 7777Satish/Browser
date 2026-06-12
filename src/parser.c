#include "parser.h"

void printlist(TagNode *list, StyleNode* stylenodes, int x);
void layout(TagNode *root, double x, double y, double *w, double *h);
Text *parseText(char *content, TTF_Font *font, SDL_Color fg);
int isStyle(char *s, int ind);
void parseCSS(TagNode *DOM);
void parseTag(TagNode *tag);
int parseInt(char *str, int start, int end);
void parseColor(char *str, int start, int end, int *r, int *g, int *b, int *a);
void parseRGB(char *str, int start, int end, int *r, int *g, int *b);
void renderTag2(TagNode *tag, Tab *tab);

void createDOM(char *file_content, Tab **tab)
{
    int i = 0;
    // int y = 0;

    ItemNode *LIST = NULL;

    int isTag = 0;
    char *currentText = malloc(1);
    strcpy(currentText, "");

    char *currentTagText = malloc(1);
    strcpy(currentTagText, "");

    while (file_content[i] != '\0')
    {
        char cc = file_content[i];
        if (cc == '\n' || cc == '\t' || (cc == ' ' && file_content[i + 1] == ' ') || (i > 0 && cc == ' ' && file_content[i - 1] == '\n'))
        {
            i += 1;
            continue;
        }

        if (cc == '<' && file_content[i + 1] != '<' && file_content[i + 1] != ' ')
        {
            isTag = 1;
            i += 1;
            if (strlen(currentText) > 1)
            {
                // printf("Text: %s\n", currentText);

                // SDL_Color color = {0, 0, 0, 255};
                ItemNode *t = (ItemNode *)(malloc(sizeof(ItemNode)));

                t->isText = 1;
                size_t size = strlen(currentText);
                t->content = malloc(size + 1);
                strcpy(t->content, currentText);

                t->next = NULL;
                t->prev = NULL;

                if (!LIST)
                {
                    LIST = t;
                }
                else
                {
                    ItemNode *ptr = LIST;
                    while (ptr->next != NULL)
                    {
                        ptr = ptr->next;
                    }
                    t->prev = ptr;
                    ptr->next = t;
                }
            }
            currentText = realloc(currentText, 1);
            currentText[0] = '\0';
            continue;
        }

        if (cc == '>')
        {
            isTag = 0;
            // printf("<%s>\n", currentTagText);

            // SDL_Color color = {0, 0, 0, 255};
            ItemNode *t = (ItemNode *)(malloc(sizeof(ItemNode)));

            size_t size = strlen(currentTagText);

            t->isText = 0;
            if (currentTagText[0] == '/')
            {
                t->isClosing = 1;

                t->content = malloc(size);
                int tIndex = 0;
                for (tIndex = 0; tIndex < size; tIndex++)
                {
                    t->content[tIndex] = currentTagText[tIndex + 1];
                }
                t->content[size - 1] = '\0';
            }
            else
            {
                t->isClosing = 0;

                t->content = malloc(size + 1);
                strcpy(t->content, currentTagText);
            }

            t->next = NULL;
            t->prev = NULL;

            if (!LIST)
            {
                LIST = t;
            }
            else
            {
                ItemNode *ptr = LIST;
                while (ptr->next != NULL)
                {
                    ptr = ptr->next;
                }
                t->prev = ptr;
                ptr->next = t;
            }

            currentTagText = realloc(currentTagText, 1);
            currentTagText[0] = '\0';
            i += 1;
            continue;
        }

        if (!isTag)
        {
            if (cc != '\t' || (cc != ' ' && file_content[i + 1] != ' '))
            {
                size_t t = strlen(currentText);
                char *newText = realloc(currentText, t + 2);

                currentText = newText;
                currentText[t] = cc;
                currentText[t + 1] = '\0';
            }
        }
        else
        {
            size_t t = strlen(currentTagText);
            char *newTagText = realloc(currentTagText, t + 2);

            currentTagText = newTagText;
            currentTagText[t] = cc;
            currentTagText[t + 1] = '\0';
        }

        i++;
    }

    ItemNode *node = LIST;

    TagNode *list = NULL;
    TagNode *currentParent = list;
    while (node != NULL)
    {

        TagNode *temp = (TagNode *)calloc(1, sizeof(TagNode));
        temp->parent = currentParent;
        temp->isText = node->isText;
        // if(node->name){
        //     size_t size = strlen(node->name);
        //     temp->name = malloc(size+1);
        //     strcpy(temp->name, node->name);
        // }

        if (node->content)
        {
            size_t size = strlen(node->content);
            temp->content = malloc(size + 1);
            strcpy(temp->content, node->content);
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
            }

            currentParent = temp;


            if(!strcasecmp(temp->name, "style")){
                StyleNode* node = (StyleNode*)malloc(sizeof(StyleNode));
                node->node = temp;
                node->next = (*tab)->stylenodes;
                (*tab)->stylenodes = node;
            }

        }
        else
        {
            currentParent = currentParent->parent;
        }

        node = node->next;
    }

    free(currentText);
    free(currentTagText);

    (*tab)->DOM = list;

    parseCSS(list);
    double width = 0, height = 0;
    layout(list, 0, 0, &width, &height);

    printlist(list, (*tab)->stylenodes, 0);
}

void printlist(TagNode *list, StyleNode* stylenodes, int x)
{
    if (!list)
        return;
    // if(list->isText) return;
    TagNode *node = list;

    while (node)
    {
        int i = 0;
        while (i < x)
        {
            i++;
            printf(" ");
        }
        if (!node->isText)
            printf("%s - x: %f y: %f w: %f h: %f\n", node->name, node->layout.x, node->layout.y, node->layout.w, node->layout.h);
        else if (node->text)
            printf("%s %s - x: %f y: %f w: %f h: %f\n", node->name, node->text->content, node->layout.x, node->layout.y, node->layout.w, node->layout.h);
        else
            printf("%s - x: %f y: %f w: %f h: %f\n", node->name, node->layout.x, node->layout.y, node->layout.w, node->layout.h);
        if (node->child)
            printlist(node->child, NULL, x + 2);
        node = node->next;
    }

    StyleNode* temp = stylenodes;
    while (temp)
    {
        if(temp->node && temp->node) printf("%s\n", temp->node->child->content);
        temp = temp->next;
    }
    
}

int PAGEHEIGHT = 0;

void layout(TagNode *root, double x, double y, double *width, double *height)
{
    if (!root)
        return;

    if (root->name && (!strcasecmp(root->name, "head") || !strcasecmp(root->name, "style") || !strcasecmp(root->name, "script")))
    {
        layout(root->next, x, y, width, height);
        return;
    }

    if (root->isText)
    {
        if (!root->text)
        {
            TTF_Font *font;

            if (root->style.fontweight >= 500)
            {
                font = TTF_OpenFont("assets/Poppins/Poppins-Bold.ttf", root->style.fontsize);
            }
            else if (root->style.fontweight >= 400)
            {
                font = TTF_OpenFont("assets/Poppins/Poppins-Medium.ttf", root->style.fontsize);
            }
            else
            {
                font = TTF_OpenFont("assets/Poppins/Poppins-Regular.ttf", root->style.fontsize);
            }

            root->text = parseText(root->content, font, root->style.color);
        }

        root->layout.x = x;
        root->layout.y = y;

        if (root->parent)
            root->layout.w = root->parent->layout.w;
        else
            root->layout.w = WINDOW_W;
        root->layout.h = 20;

        Text *node = root->text;
        int x = 0, y = 0;
        while (node)
        {

            if (x + node->width >= root->layout.w)
            {
                x = 0;
                if (root->style.lineheight)
                    y += root->style.lineheight;
                else
                    y += root->text->height;
            }

            x += node->width + 8;
            node = node->next;
        }

        if (root->text)
        {
            printf("%d %d\n", root->style.lineheight, root->text->height);
            root->layout.h = y + root->text->height;
        }
        *height += root->layout.h;
    }
    else
    {
        if (root->parent)
        {
            if (root->parent->style.display == BLOCK)
            {
                //     if (root->style.display == BLOCK || root->style.display == FLEX || root->style.display == GRID)
                //     {
                if (root->style.width)
                    root->layout.w = root->style.width + root->style.paddingright + root->style.paddingleft;
                else
                    root->layout.w = root->parent->layout.w - root->parent->style.paddingright - root->parent->style.paddingleft + root->style.paddingright + root->style.paddingleft;
                // }

                root->layout.x = x + root->style.marginleft;
                root->layout.y = y + root->style.margintop;
                // if (root->parent->style.alignItems == 2)
                // {
                //     root->layout.x = x + root->parent->layout.w / 2 - root->layout.w / 2;
                // }

                double wdth = 0, hght = root->style.paddingtop + root->style.paddingbottom;
                if (root->child)
                    layout(root->child, root->layout.x + root->style.paddingleft, root->layout.y + root->style.paddingtop, &wdth, &hght);

                if (!strcasecmp(root->name, "br"))
                {
                    hght += 20;
                }
                hght += root->style.paddingtop + root->style.paddingbottom;
                if (root->style.height)
                    hght = root->style.height + root->style.paddingtop + root->style.paddingbottom;
                root->layout.h = hght;
                *height += hght + root->style.margintop + root->style.marginbottom;
            }
            else
            {
                if (root->parent->style.flexDirection == 0)
                { // Row

                    root->layout.x = x + root->style.marginleft;
                    root->layout.y = y + root->style.margintop;

                    int availWidth = root->parent->layout.w - root->parent->style.paddingleft - root->parent->style.paddingright;
                    int count = 0;
                    int i = 0;
                    TagNode *node = root->parent->child;
                    while (node)
                    {
                        if (node == root)
                            i = count;
                        if (node->style.width)
                        {
                            availWidth -= node->style.width;
                        }
                        else
                            count++;
                        availWidth -= node->parent->style.gap;
                        availWidth -= node->style.paddingright + node->style.paddingleft + node->style.marginleft + node->style.marginright;
                        node = node->next;
                    }
                    if (root->style.width)
                        root->layout.w = root->style.width + root->style.paddingright + root->style.paddingleft;
                    else
                        root->layout.w = (availWidth) / count + root->style.paddingright + root->style.paddingleft;

                    double wdth = 0, hght = root->style.paddingtop + root->style.paddingbottom;
                    if (root->child)
                        layout(root->child, root->layout.x + root->style.paddingleft, root->layout.y + root->style.paddingtop, &wdth, &hght);
                    hght += root->style.paddingtop + root->style.paddingbottom;
                    if (root->style.height)
                        hght = root->style.height + root->style.paddingtop + root->style.paddingbottom;
                    root->layout.h = hght;

                    double h = hght + root->style.margintop + root->style.marginbottom;
                    if (h > *height)
                        *height = h;
                }
                else
                { // Column
                    //     if (root->style.display == BLOCK || root->style.display == FLEX || root->style.display == GRID)
                    //     {
                    if (root->style.width)
                        root->layout.w = root->style.width + root->style.paddingright + root->style.paddingleft;
                    else
                        root->layout.w = root->parent->layout.w - root->parent->style.paddingright - root->parent->style.paddingleft + root->style.paddingright + root->style.paddingleft;
                    // }

                    root->layout.x = x + root->style.marginleft;
                    // if (root->parent->style.alignItems == 2)
                    // {
                    //     root->layout.x = x + root->parent->layout.w / 2 - root->layout.w / 2;
                    // }
                    root->layout.y = y + root->style.margintop;

                    double wdth = 0, hght = root->style.paddingtop + root->style.paddingbottom;
                    if (root->child)
                        layout(root->child, root->layout.x + root->style.paddingleft, root->layout.y + root->style.paddingtop, &wdth, &hght);

                    if (!strcasecmp(root->name, "br"))
                    {
                        hght += 20;
                    }
                    hght += root->style.paddingtop + root->style.paddingbottom;
                    if (root->style.height)
                        hght = root->style.height + root->style.paddingtop + root->style.paddingbottom;
                    root->layout.h = hght;
                    *height += hght + root->style.margintop + root->style.marginbottom;
                }
            }
        }
        else
        {
            root->layout.x = 0;
            root->layout.y = 0;
            root->layout.w = WINDOW_W;

            double wdth = 0, hght = 0;
            if (root->child)
                layout(root->child, root->layout.x, root->layout.y, &wdth, &hght);

            root->layout.h = hght;
            *height += hght + root->style.margintop + root->style.marginbottom;
        }
    }

    if (root->style.maxWidth && root->layout.w > root->style.maxWidth)
        root->layout.w = root->style.maxWidth;
    SDL_Rect r = {root->layout.x, root->layout.y, root->layout.w, root->layout.h};
    root->layout.r = r;

    // *height += root->layout.h + root->style.margintop + root->style.marginbottom;

    if (root->layout.y + root->layout.h > PAGEHEIGHT)
        PAGEHEIGHT = root->layout.y + root->layout.h;

    if (root->parent)
    {
        if (!root->next && root->parent->style.display == FLEX && root->parent->style.flexDirection == 0 && !root->parent->style.alignItems)
        {
            TagNode *node = root->parent->child;
            while (node)
            {
                node->layout.h = *height;
                node->layout.r.h = *height;
                node = node->next;
            }
        }

        if (root->parent->style.display == FLEX)
        {
            if (root->parent->style.flexDirection == 0)
            {
                layout(root->next, r.x + r.w + root->parent->style.gap + root->style.paddingright + root->style.marginright, y, width, height);
            }
            else if (root->parent->style.flexDirection == 1)
            {
                layout(root->next, x, r.y + r.h + root->parent->style.gap + root->style.paddingbottom + root->style.marginbottom, width, height);
            }
        }
        // if (root->parent->style.display == BLOCK)
        else
            layout(root->next, r.x - root->style.marginleft, r.y + r.h + root->style.marginbottom, width, height);
    }
    else
    {
        layout(root->next, x, r.y + r.h, width, height);
    }
}

Text *parseText(char *content, TTF_Font *font, SDL_Color fg)
{
    int i = 0;
    size_t len = strlen(content);

    Text *text = NULL;
    Text *tail = NULL;
    int wordlen = 0;
    int maxlen = 4;
    char *currentWord = malloc(maxlen);
    currentWord[0] = '\0';

    while (i <= len)
    {
        if (content[i] == ' ' || content[i] == '\0')
        {
            if (wordlen > 0)
            {
                currentWord[wordlen] = '\0';

                SDL_Surface *s = TTF_RenderText_Blended(font, currentWord, fg);
                SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);

                Text *node = (Text *)malloc(sizeof(Text));

                node->content = SDL_strdup(currentWord);
                node->width = s->w;
                node->height = s->h;
                node->t = t;
                node->next = NULL;

                SDL_FreeSurface(s);
                if (!text)
                {
                    text = node;
                }
                else
                {
                    tail->next = node;
                }

                tail = node;
            }

            wordlen = 0;
            i++;
            continue;
        }

        if (wordlen >= maxlen - 1)
        {
            maxlen *= 2;
            char *txt = realloc(currentWord, maxlen);
            currentWord = txt;
        }

        currentWord[wordlen++] = content[i];
        i++;
    }
    free(currentWord);
    return text;
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

void parseTag(TagNode *tag)
{
    if (tag->isText)
        return;

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
        if (tag->content[0] == ' ')
        {
            tag->content += 1;
            break;
        }
    }
    tag->name = name;
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

    tag->style.display = BLOCK;
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
    if (tag->name && strcasecmp(tag->name, "tr") == 0)
    {
        tag->style.display = FLEX;
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

    // Parse styles
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

    // printf("\nContent: %s\n", tag->content + ind);

    char key[100];
    int keyInd = 0;
    char value[100];
    int valueInd = 0;
    int which = 0;

    while (tag->content[ind] != '"' && tag->content[ind] != '\0')
    {

        char c = tag->content[ind];
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
                    tag->style.display = FLEX;
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

void parseColor(char *str, int start, int end, int *r, int *g, int *b, int *a)
{
    int i = start;
    while (i < end)
    {
        if (str[i] == 'r' && str[i + 1] == 'g' && str[i + 2] == 'b' && str[i + 3] == '(')
        {
            parseRGB(str, i + 4, end, r, g, b);
            *a = 255;
            return;
        }
        i++;
    }
}

void parseRGB(char *str, int start, int end, int *r, int *g, int *b)
{
    int i = start;
    int values[3] = {0};
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

int isStyle(char *s, int ind)
{
    if (s[ind] == 's' && s[ind + 1] == 't' && s[ind + 2] == 'y' && s[ind + 3] == 'l' && s[ind + 4] == 'e' && s[ind + 5] == '=' && s[ind + 6] == '"')
        return 1;
    else
        return 0;
}

void createCSOM(char *file_content, Tab **tab)
{
}

void renderDOM(Tab *tab)
{
    if (!tab->DOM)
        return;

    // int j = -tab->scrollY;
    renderTag2(tab->DOM, tab);
}

void renderTag(TagNode **tag, int *j, Tab **tab)
{
    if (!(*tag))
        return;

    TagNode *ptr = *tag;

    while (ptr != NULL)
    {

        if (ptr->name && !strcmp(ptr->name, "style"))
        {
            ptr = ptr->next;
            continue;
        }

        if (ptr->name && !strcmp(ptr->name, "title"))
        {
            // (*tag)->name = ptr->child->content;
            if (!(*tab)->t1)
            {
                SDL_Surface *s1 = TTF_RenderText_Blended(poppins_bold, ptr->child->content, tab_fg);
                (*tab)->t1 = SDL_CreateTextureFromSurface(renderer, s1);
                SDL_FreeSurface(s1);
            }
            ptr = ptr->next;
            continue;
        }

        if (ptr->content)
        {
            // SDL_Color color = {20, 20, 220, 255};
            if (!ptr->t1)
            {
                SDL_Surface *s1;
                if (ptr->parent && strcmp(ptr->parent->name, "h1") == 0)
                {
                    // color = (SDL_Color){0, 0, 0, 255};
                    poppins_bold = TTF_OpenFont("assets/Poppins/Poppins-Bold.ttf", ptr->style.fontsize);
                    s1 = TTF_RenderText_Blended(poppins_bold, ptr->content, ptr->style.color);
                }
                else
                {
                    poppins_regular = TTF_OpenFont("assets/Poppins/Poppins-Regular.ttf", ptr->style.fontsize);
                    if (ptr->isText)
                    {
                        s1 = TTF_RenderText_Blended(poppins_regular, ptr->content, ptr->parent->style.color);
                    }
                    else
                    {
                        s1 = TTF_RenderText_Blended(poppins_regular, ptr->content, ptr->style.color);
                    }
                }
                ptr->t1 = SDL_CreateTextureFromSurface(renderer, s1);
                SDL_FreeSurface(s1);
            }
            // }
            if (ptr->type == 0)
            {
                int y = BORDER_HEIGHT * 2 + (*j) + ptr->style.margintop;
                SDL_Rect bg = {
                    ptr->style.marginleft, y, WINDOW_W - ptr->style.marginright - ptr->style.marginleft, ptr->style.paddingtop + ptr->style.paddingtop};

                SDL_SetRenderDrawColor(renderer, ptr->style.background.r, ptr->style.background.g, ptr->style.background.b, ptr->parent->style.background.a);
                SDL_RenderFillRect(renderer, &bg);
            }

            if (!ptr->next && ptr->parent && ptr->parent->type == 0)
            {
                int y = BORDER_HEIGHT * 2 + (*j);
                SDL_Rect bg = {
                    ptr->parent->style.marginleft, y, WINDOW_W - ptr->parent->style.marginright - ptr->parent->style.marginleft, ptr->parent->style.paddingbottom + ptr->parent->style.marginbottom};

                SDL_SetRenderDrawColor(renderer, ptr->parent->style.background.r, ptr->parent->style.background.g, ptr->parent->style.background.b, ptr->parent->parent->style.background.a);
                SDL_RenderFillRect(renderer, &bg);
            }

            if (ptr->isText)
            {
                int w, h;
                SDL_QueryTexture(ptr->t1, NULL, NULL, &w, &h);
                // printf("%s, %d\n", ptr->content, w);
                SDL_Rect r1 = {
                    ptr->parent->style.marginleft + ptr->parent->style.paddingleft,
                    BORDER_HEIGHT * 2 + (*j) + ptr->parent->style.paddingtop + ptr->parent->style.margintop,
                    w,
                    h};
                SDL_Rect bg = {
                    ptr->parent->style.marginleft, r1.y - ptr->parent->style.paddingtop, ptr->parent->style.width - ptr->parent->style.marginright - ptr->parent->style.marginleft, r1.h + ptr->parent->style.paddingtop + ptr->parent->style.paddingbottom};
                int marginandpadding = ptr->parent->style.marginleft + ptr->parent->style.paddingleft + ptr->parent->style.paddingright + ptr->parent->style.marginright;
                if (ptr->parent->style.textalign == 1)
                {
                    r1.x += ptr->parent->style.width / 2 - w / 2 - marginandpadding / 2;
                    if (ptr->parent->type == 1)
                        bg.x += ptr->parent->style.width / 2 - w / 2 - marginandpadding / 2;
                }

                if (ptr->parent->type == 1)
                {
                    bg.w = r1.w + ptr->parent->style.paddingright + ptr->parent->style.paddingleft;
                }

                SDL_SetRenderDrawColor(renderer, ptr->parent->style.background.r, ptr->parent->style.background.g, ptr->parent->style.background.b, ptr->parent->style.background.a);
                SDL_RenderFillRect(renderer, &bg);
                SDL_RenderCopy(renderer, ptr->t1, NULL, &r1);
                (*j) += h;
                (*j) += ptr->parent->style.paddingtop + ptr->parent->style.margintop;
                if (!ptr->next && ptr->parent)
                {
                    (*j) += ptr->parent->style.marginbottom + ptr->parent->style.paddingbottom;
                }
            }
        }

        if (ptr->child)
        {
            renderTag(&ptr->child, j, tab);
        }

        ptr = ptr->next;
    }
}

void renderTag2(TagNode *tag, Tab *tab)
{
    if (!tag)
        return;

    TagNode *ptr = tag;
    if (PAGEHEIGHT > WINDOW_H)
    {
        if (tab->scrollY + WINDOW_H - 2 * BORDER_HEIGHT > PAGEHEIGHT)
            tab->scrollY = PAGEHEIGHT - WINDOW_H + 2 * BORDER_HEIGHT;
    }
    else
    {
        tab->scrollY = 0;
    }
    while (ptr)
    {
        if (ptr->name && !strcmp(ptr->name, "style"))
        {
            ptr = ptr->next;
            continue;
        }

        if (ptr->name && !strcmp(ptr->name, "head"))
        {
            ptr = ptr->next;
            continue;
        }

        if (ptr->name && (!strcmp(ptr->name, "script") || !strcmp(ptr->name, "style")))
        {
            ptr = ptr->next;
            continue;
        }

        if (!ptr->isText)
        {

            SDL_SetRenderDrawColor(renderer, 120, 200, 100, 255);
            SDL_SetRenderDrawColor(renderer, ptr->style.background.r, ptr->style.background.g, ptr->style.background.b, ptr->style.background.a);
            SDL_Rect r = ptr->layout.r;

            r.y -= tab->scrollY - 2 * BORDER_HEIGHT;

            SDL_RenderFillRect(renderer, &r);

            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);

            if (ptr->style.border)
            {
                SDL_RenderDrawRect(renderer, &r);
            }
            if (ptr->style.bordertop)
            {
                SDL_RenderDrawLine(renderer, r.x, r.y, r.x + r.w, r.y);
            }
            if (ptr->style.borderright)
            {
                SDL_RenderDrawLine(renderer, r.x + r.w - 1, r.y, r.x + r.w - 1, r.y + r.h);
            }
            if (ptr->style.borderbottom)
            {
                SDL_RenderDrawLine(renderer, r.x, r.y + r.h, r.x + r.w, r.y + r.h);
            }
            if (ptr->style.borderleft)
            {
                SDL_RenderDrawLine(renderer, r.x, r.y, r.x, r.y + r.h);
            }

            // printf("%s:: %f %f %f %f -- %d %d %d\n", ptr->name, ptr->layout.x, ptr->layout.y, ptr->layout.w, ptr->layout.h, ptr->style.background.r, ptr->style.background.g, ptr->style.background.b);
        }
        else
        {
            Text *node = ptr->text;
            int x = 0;
            int y = 0;

            while (node)
            {
                // printf("%d %d - ", node->width, node->height);
                SDL_Rect r = ptr->layout.r;
                if (x + node->width >= ptr->layout.w)
                {
                    x = 0;
                    if (ptr->style.lineheight)
                        y += ptr->style.lineheight;
                    else
                        y += ptr->text->height;
                }
                r.x += x;
                r.y -= tab->scrollY - y - 2 * BORDER_HEIGHT;
                r.w = node->width;
                r.h = node->height;

                if (ptr->parent)
                {
                    r.x += ptr->parent->style.paddingleft;
                    r.y += ptr->parent->style.paddingtop;
                }

                if (node->t)
                    SDL_RenderCopy(renderer, node->t, NULL, &r);

                x += node->width + 8;
                node = node->next;
            }

            // printf("\n");
        }

        if (ptr->child)
        {
            renderTag2(ptr->child, tab);
        }
        ptr = ptr->next;
    }
}