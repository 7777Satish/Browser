#include "HTML/layout.h"
#include "renderer.h"
#include "HTML/parser.h"

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
                if (root->parent->style.alignItems == 2)
                {
                    root->layout.x = x + root->parent->layout.w / 2 - root->layout.w / 2;
                }

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
                    if (root->parent->style.alignItems == 2)
                    {
                        root->layout.x = x + root->parent->layout.w / 2 - root->layout.w / 2;
                    }
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

    if (root->layout.y + root->layout.h > currentTab->MAXHEIGHT)
        currentTab->MAXHEIGHT = root->layout.y + root->layout.h;

    if (root->parent)
    {
        if (!root->next && root->parent->style.display == FLEX && root->parent->style.flexDirection == 0 && !root->parent->style.alignItems)
        {
            TagNode *node = root->parent->child;
            while (node)
            {
                if (!node->style.height)
                {
                    node->layout.h = *height;
                    node->layout.r.h = *height;
                }
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
                // SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);

                Text *node = (Text *)malloc(sizeof(Text));

                node->content = SDL_strdup(currentWord);
                node->width = s->w;
                node->height = s->h;
                node->s = s;
                node->t = NULL;
                node->next = NULL;

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

void renderDOM(Tab *tab)
{
    if (!tab->DOM)
        return;

    // int j = -tab->scrollY;
    renderTag(tab->DOM, tab);
}

void renderTag(TagNode *tag, Tab *tab)
{
    if (!tag)
        return;

    TagNode *ptr = tag;
    if ((*tab).MAXHEIGHT > WINDOW_H)
    {
        if (tab->scrollY + WINDOW_H - 2 * BORDER_HEIGHT > (*tab).MAXHEIGHT)
            tab->scrollY = (*tab).MAXHEIGHT - WINDOW_H + 2 * BORDER_HEIGHT;
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

                if (!node->t)
                {
                    node->t = SDL_CreateTextureFromSurface(renderer, node->s);
                    SDL_FreeSurface(node->s);
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
            renderTag(ptr->child, tab);
        }
        ptr = ptr->next;
    }
}

int isStyle(char *s, int ind)
{
    if (s[ind] == 's' && s[ind + 1] == 't' && s[ind + 2] == 'y' && s[ind + 3] == 'l' && s[ind + 4] == 'e' && s[ind + 5] == '=' && s[ind + 6] == '"')
        return 1;
    else
        return 0;
}
