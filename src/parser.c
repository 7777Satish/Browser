#include "parser.h"

int isStyle(char *s, int ind);
void parseCSS(TagNode *DOM);
void parseTag(TagNode *tag);
int parseInt(char *str, int start, int end);
void parseColor(char *str, int start, int end, int *r, int *g, int *b, int *a);
void parseRGB(char *str, int start, int end, int *r, int *g, int *b);

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
            size_t t = strlen(currentText);
            char *newText = realloc(currentText, t + 2);

            currentText = newText;
            currentText[t] = cc;
            currentText[t + 1] = '\0';
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

        TagNode *temp = (TagNode *)malloc(sizeof(TagNode));
        temp->child = NULL;
        temp->lastChild = NULL;
        temp->next = NULL;
        temp->prev = NULL;
        temp->parent = currentParent;
        temp->t1 = NULL;
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
}

void parseCSS(TagNode *DOM)
{
    if (!DOM)
        return;
    if (DOM->content)
        parseTag(DOM);
    if (DOM->child)
        parseCSS(DOM->child);
    if (DOM->next)
        parseCSS(DOM->next);
}

void parseTag(TagNode *tag)
{

    if(tag->parent){
        tag->style.color = tag->parent->style.color;
        tag->style.background = tag->parent->style.background;
        tag->style.fontsize = tag->parent->style.fontsize;
    } else {
        tag->style.color = (SDL_Color){20, 20, 20, 255};
        tag->style.color = (SDL_Color){255, 255, 255, 255};
        tag->style.fontsize = BORDER_ICON_H+1;
    }

    tag->style.marginleft = 0;
    tag->style.marginright = 0;
    tag->style.margintop = 0;
    tag->style.marginbottom = 0;

    tag->style.paddingleft = 0;
    tag->style.paddingright = 0;
    tag->style.paddingtop = 0;
    tag->style.paddingbottom = 0;

    char *name = malloc(1);
    name[0] = '\0';
    int len = 1;
    if (tag->isText)
        return;
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
    // printf("%s|%s\n\n", tag->name, tag->content);

    if(strcmp(tag->name, "div") == 0 || strcmp(tag->name, "h1") == 0 || strcmp(tag->name, "p") == 0 || strcmp(tag->name, "section") == 0 || strcmp(tag->name, "footer") == 0 || strcmp(tag->name, "nav") == 0 || strcmp(tag->name, "header") == 0){
        tag->type = 0;
    } else {
        tag->type = 1;
    }

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

    printf("\nContent: %s\n", tag->content + ind);

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
            printf("\n%s :: %s\n", key, value);

            int v = parseInt(value, 0, valueInd);
            if (strcmp(key, "width") == 0)
            {
                tag->style.width = v;
            }
            else if (strcmp(key, "height") == 0)
            {
                tag->style.height = v;
            }
            else if (strcmp(key, "font-size") == 0)
            {
                tag->style.fontsize = v;
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
                            printf("--- %d %d\n", lastInd, i);
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

    int j = 0;
    renderTag(&tab->DOM, &j, &tab);
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
            SDL_Surface *s1 = TTF_RenderText_Blended(poppins_bold, ptr->child->content, tab_fg);
            (*tab)->t1 = SDL_CreateTextureFromSurface(renderer, s1);
            SDL_FreeSurface(s1);
            ptr = ptr->next;
            continue;
        }

        if (ptr->content)
        {
            // if(!ptr->t1){
            // SDL_Color color = {20, 20, 220, 255};
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
                if(ptr->isText){
                    s1 = TTF_RenderText_Blended(poppins_regular, ptr->content, ptr->parent->style.color);
                } else {
                    s1 = TTF_RenderText_Blended(poppins_regular, ptr->content, ptr->style.color);
                }
            }
            ptr->t1 = SDL_CreateTextureFromSurface(renderer, s1);
            SDL_FreeSurface(s1);
            // }
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
                    ptr->parent->style.marginleft, r1.y - ptr->parent->style.paddingtop, WINDOW_W - ptr->parent->style.marginright - ptr->parent->style.marginleft, r1.h + ptr->parent->style.paddingtop + ptr->parent->style.paddingbottom
                };
                if(ptr->parent->type == 1){
                    bg.w = r1.w + ptr->parent->style.paddingright;
                }
                
                SDL_SetRenderDrawColor(renderer, ptr->parent->style.background.r, ptr->parent->style.background.g, ptr->parent->style.background.b, ptr->parent->style.background.a);
                SDL_RenderFillRect(renderer, &bg);
                SDL_RenderCopy(renderer, ptr->t1, NULL, &r1);
                (*j) += h;
            }
            (*j) += ptr->style.paddingtop + ptr->style.margintop;
            if(!ptr->next && ptr->parent){
                (*j) += ptr->parent->style.marginbottom + ptr->parent->style.marginbottom;
            }
        }

        if (ptr->child)
        {
            renderTag(&ptr->child, j, tab);
        }

        ptr = ptr->next;
    }
}