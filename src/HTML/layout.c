#include "HTML/layout.h"
#include "renderer.h"
#include "HTML/parser.h"
#include "utils/mouse.h"

void layout(TagNode *root, double x, double y, double *width, double *height)
{
    if (!root)
        return;

    if (root->name && (!strcasecmp(root->name, "head") || !strcasecmp(root->name, "style") || !strcasecmp(root->name, "script")))
    {
        layout(root->next, x, y, width, height);
        return;
    }

    if (root->style.displayOuter == DISPLAY_OUTER_NONE)
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
        {
            if (root->parent->style.displayOuter == DISPLAY_OUTER_INLINE)
            {
                root->layout.h = 20;

                TagNode *lastBlock = root->parent;

                while (lastBlock->parent && root->parent->style.displayOuter == DISPLAY_OUTER_INLINE)
                {
                    lastBlock = lastBlock->parent;
                }

                Text *node = root->text;
                int x = 0, y = 0;
                while (node)
                {
                    if (x + node->width >= lastBlock->layout.w)
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
                    root->layout.h = root->text->height;
                    root->layout.w = x;
                }

                *width += root->layout.w;
                *height = root->layout.h;
            }
            else
            {
                root->layout.w = root->parent->layout.w;

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
        }
        else
        {
            root->layout.w = WINDOW_W;
            *height += root->layout.h;
        }
    }
    else
    {
        if (root->parent)
        {
            if (root->parent->style.displayInner == DISPLAY_INNER_FLOW)
            {
                if (root->style.displayOuter == DISPLAY_OUTER_BLOCK)
                {
                    TagNode *lastBlock = root->parent;

                    while (lastBlock->parent && root->parent->style.displayOuter == DISPLAY_OUTER_INLINE)
                    {
                        lastBlock = lastBlock->parent;
                    }

                    if (root->style.width)
                        root->layout.w = root->style.width + root->style.paddingright + root->style.paddingleft;
                    else
                        root->layout.w = lastBlock->layout.w - root->parent->style.paddingright - root->parent->style.paddingleft + root->style.paddingright + root->style.paddingleft;
                    // }

                    root->layout.x = lastBlock->layout.x + lastBlock->style.paddingleft + root->style.marginleft;
                    root->layout.y = y + root->style.margintop;

                    // if (root->parent->style.displayInner == DISPLAY_INNER_FLEX && root->parent->style.alignItems == 2)
                    // {
                    //     root->layout.x = x + root->parent->layout.w / 2 - root->layout.w / 2;
                    // }

                    double wdth = 0, hght = 0;
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
                    *width += root->layout.w + root->style.marginleft + root->style.marginright;

                    // layout(root->next, root->layout.x - root->style.marginleft, root->layout.y + root->layout.h + root->style.marginbottom, width, height);
                    // return;
                }
                else if (root->style.displayOuter == DISPLAY_OUTER_INLINE)
                {
                    // if (root->style.width)
                    //     root->layout.w = root->style.width + root->style.paddingright + root->style.paddingleft;
                    // else
                    //     root->layout.w = root->parent->layout.w - root->parent->style.paddingright - root->parent->style.paddingleft + root->style.paddingright + root->style.paddingleft;
                    // }

                    root->layout.x = x + root->style.marginleft;
                    root->layout.y = y + root->style.margintop;
                    if (root->parent->style.alignItems == 2)
                    {
                        root->layout.x = x + root->parent->layout.w / 2 - root->layout.w / 2;
                    }

                    double wdth = root->style.paddingleft + root->style.paddingright, hght = root->style.paddingtop + root->style.paddingbottom;
                    if (root->child)
                        layout(root->child, root->layout.x + root->style.paddingleft, root->layout.y + root->style.paddingtop, &wdth, &hght);

                    hght += root->style.paddingtop + root->style.paddingbottom;
                    if (root->style.height)
                        hght = root->style.height + root->style.paddingtop + root->style.paddingbottom;
                    root->layout.h = hght;

                    if (root->style.width)
                        wdth = root->style.width + root->style.paddingleft + root->style.paddingright;
                    root->layout.w = wdth;
                    *height += hght + root->style.margintop + root->style.marginbottom;
                    *width += root->layout.w + root->style.marginleft + root->style.marginright;
                }
            }
            else if (root->parent->style.displayInner == DISPLAY_INNER_FLEX)
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
                    *width += root->layout.w + root->style.marginleft + root->style.marginright;
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
                    *width += root->layout.w + root->style.marginleft + root->style.marginright;
                }
            }

            // if (root->style.displayOuter == DISPLAY_OUTER_BLOCK)
            // {
            // }
            // else if (root->style.displayOuter == DISPLAY_OUTER_INLINE)
            // {
            //     // if (root->style.width)
            //     //     root->layout.w = root->style.width + root->style.paddingright + root->style.paddingleft;
            //     // else
            //     //     root->layout.w = root->parent->layout.w - root->parent->style.paddingright - root->parent->style.paddingleft + root->style.paddingright + root->style.paddingleft;
            //     // }

            //     root->layout.x = x + root->style.marginleft;
            //     root->layout.y = y + root->style.margintop;
            //     if (root->parent->style.alignItems == 2)
            //     {
            //         root->layout.x = x + root->parent->layout.w / 2 - root->layout.w / 2;
            //     }

            //     double wdth = root->style.paddingleft + root->style.paddingright, hght = root->style.paddingtop + root->style.paddingbottom;
            //     if (root->child)
            //         layout(root->child, root->layout.x + root->style.paddingleft, root->layout.y + root->style.paddingtop, &wdth, &hght);

            //     // hght += root->style.paddingtop + root->style.paddingbottom;
            //     if (root->style.height)
            //         hght = root->style.height + root->style.paddingtop + root->style.paddingbottom;
            //     root->layout.h = hght;

            //     if (root->style.width)
            //         wdth = root->style.width + root->style.paddingleft + root->style.paddingright;
            //     root->layout.w = wdth;
            //     *height += hght + root->style.margintop + root->style.marginbottom;
            //     *width += root->layout.w + root->style.marginleft + root->style.marginright;
            // }
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
            *width += root->layout.w + root->style.marginleft + root->style.marginright;
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
        if (!root->next && root->parent->style.displayInner == DISPLAY_INNER_FLEX && root->parent->style.flexDirection == 0 && !root->parent->style.alignItems)
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

        if (root->parent->style.displayInner == DISPLAY_INNER_FLEX)
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
        if (root->parent->style.displayInner == DISPLAY_INNER_FLOW)
        {
            if (root->style.displayOuter == DISPLAY_OUTER_BLOCK)
            {
                layout(root->next, r.x - root->style.marginleft, r.y + r.h + root->style.marginbottom, width, height);
            }
            else if (root->style.displayOuter == DISPLAY_OUTER_INLINE || root->isText)
            {
                if (root->next && root->next->style.displayOuter == DISPLAY_OUTER_BLOCK)
                {
                    layout(root->next, x, r.y + r.h + root->style.marginbottom, width, height);
                }
                else
                    layout(root->next, r.x + r.w + root->style.marginright, r.y - root->style.margintop, width, height);
            }
        }
        // else
    }
    else
    {
        layout(root->next, x, r.y + r.h, width, height);
    }
}

LayoutNode *createLayoutTree(TagNode *root, LayoutNode *parent)
{
    if (!root)
    {
        return NULL;
    }

    LayoutNode *layout = NULL;
    LayoutNode *last = NULL;
    TagNode *tnode = root;
    int returnLayout = 1;

    while (tnode)
    {

        if (tnode->style.displayOuter == DISPLAY_OUTER_NONE)
        {
            tnode = tnode->next;
            continue;
        }

        LayoutNode *node = (LayoutNode *)malloc(sizeof(LayoutNode));
        // if(tnode->name) printf("%s\n", tnode->name);

        node->parent = parent;
        node->next = NULL;
        node->prev = last;
        node->tag = tnode;
        node->child = NULL;
        node->lastChild = NULL;
        node->type = 0;
        node->lines = NULL;
        node->lastLine = NULL;

        // if(!parent){
        //     node->layout.x = 0;
        //     node->layout.y = 0;
        //     node->layout.w = WINDOW_W;
        //     node->layout.h = h;

        //     node->layout.r = (SDL_Rect){node->layout.x, node->layout.y, node->layout.w, node->layout.h};
        // }

        // else {
        if (tnode->isText)
        {
            /*
            // if (!tnode->text)
            // {
            //     TTF_Font *font;

            //     if (tnode->style.fontweight >= 500)
            //     {
            //         font = TTF_OpenFont("assets/Poppins/Poppins-Bold.ttf", tnode->style.fontsize);
            //     }
            //     else if (tnode->style.fontweight >= 400)
            //     {
            //         font = TTF_OpenFont("assets/Poppins/Poppins-Medium.ttf", tnode->style.fontsize);
            //     }
            //     else
            //     {
            //         font = TTF_OpenFont("assets/Poppins/Poppins-Regular.ttf", tnode->style.fontsize);
            //     }

            //     // tnode->text = parseText(tnode->content, font, tnode->style.color);
            // }

            if (tnode->text)
            {
                LayoutNode *p = parent;
                while (p->parent && p->type != 2)
                {
                    p = p->parent;
                }

                LineNode *line = p->lastLine;

                if (!line)
                {
                    LineNode *l = calloc(sizeof(LineNode), 1);
                    l->prev = line;
                    line = l;
                    p->lines = l;
                    p->lastLine = l;
                }

                Text *text = tnode->text;
                double maxWidth = p->layout.w;

                if (line->width + 8 + text->width < maxWidth && text->height > line->height)
                    line->height = text->height;

                if (line->width + 8 + text->width < maxWidth && line->lastWord)
                {
                    line->lastWord->next = text;
                }

                Text *t = line->text;
                int px = p->layout.x;
                while (t)
                {
                    t->layout.x = px + 8;
                    t->layout.y = p->layout.y + p->layout.h + line->height - t->height;
                    t->layout.w = t->width;
                    t->layout.h = t->height;

                    px += 8 + t->width;
                    t = t->next;
                }

                while (text)
                {
                    int x = line->width + 8;
                    Text *next = text->next;
                    if (x + text->width > maxWidth)
                    {
                        LineNode *l = calloc(sizeof(LineNode), 1);
                        if (line)
                        {
                            line->next = l;
                        }
                        l->prev = line;
                        p->layout.h += line->height;

                        line = l;
                        p->lastLine = l;
                        if (!p->lines)
                            p->lines = l;

                        text->next = NULL;
                    }

                    text->layout.x = p->layout.x + x;
                    text->layout.y = p->layout.y + p->layout.h + line->height - text->height;
                    text->layout.w = text->width;
                    text->layout.h = text->height;

                    line->lastWord = text;

                    if (line->text)
                    {
                        line->width += 8 + text->width;
                    }
                    else
                    {
                        line->width += text->width;
                        line->text = text;
                    }

                    text = next;
                }
            }
*/

            if (!layout)
            {
                layout = node;
                last = node;
            }
            else
            {
                last->next = node;
                node->prev = last;
                last = node;
            }

            if (parent)
            {
                parent->lastChild = node;
            }
            // Todo: Handle if no parent

            tnode = tnode->next;
            continue;
        }
        else
        {
            if (!tnode->parent || tnode->parent->style.displayInner == DISPLAY_INNER_FLOW)
            {
                if (tnode->style.displayOuter == DISPLAY_OUTER_BLOCK)
                {

                    node->child = createLayoutTree(tnode->child, node);

                    if ((tnode->prev && tnode->prev->isText) || (tnode->prev && tnode->prev->style.displayOuter == DISPLAY_OUTER_INLINE) || (parent && parent->type == 2) || (parent && parent->tag->style.displayOuter == DISPLAY_OUTER_INLINE))
                    {
                        if (!tnode->prev)
                        {
                            returnLayout = 0;
                        }

                        while (parent && (parent->type == 2 || parent->tag->style.displayOuter == DISPLAY_OUTER_INLINE))
                        {
                            parent = parent->parent;
                        }

                        node->parent = parent;

                        if (parent)
                        {
                            last = parent->lastChild;
                        }
                        else
                            last = NULL;
                    }

                    if (last)
                    {
                        last->next = node;
                    }
                    node->prev = last;
                    last = node;

                    if (!layout)
                    {
                        layout = node;
                    }

                    if (parent)
                    {
                        parent->lastChild = node;
                    }

                    tnode = tnode->next;
                    continue;
                }

                else if (tnode->style.displayOuter == DISPLAY_OUTER_INLINE)
                {
                    if ((tnode->prev && tnode->prev->style.displayOuter == DISPLAY_OUTER_BLOCK) || (tnode->prev == NULL && tnode->parent && tnode->parent->style.displayOuter == DISPLAY_OUTER_BLOCK))
                    {
                        // printf("%s : %s :: %d - %d\n", tnode->name, tnode->parent->name, (tnode->prev && tnode->prev->style.displayOuter == DISPLAY_OUTER_BLOCK) , (!tnode->prev && tnode->parent && tnode->parent->style.displayOuter == DISPLAY_OUTER_BLOCK));
                        // if(tnode->prev) printf("%s\n", tnode->prev->name);
                        LayoutNode *aNode = (LayoutNode *)malloc(sizeof(LayoutNode));
                        aNode->child = node;
                        aNode->parent = node->parent;
                        node->parent = aNode;

                        aNode->lastChild = node;
                        aNode->next = NULL;

                        aNode->prev = node->prev;
                        node->prev = NULL;

                        aNode->tag = NULL;
                        aNode->type = 2;

                        if (last)
                            last->next = aNode;

                        if (aNode->parent)
                        {
                            aNode->parent->lastChild = aNode;
                        }

                        parent = aNode;

                        last = node;

                        if (!layout)
                        {
                            layout = aNode;
                        }

                        double wdth = 0, hght = 0;
                        node->child = createLayoutTree(tnode->child, node);

                        tnode = tnode->next;

                        continue;
                    }
                    else
                    {
                        if (last)
                            last->next = node;
                        last = node;

                        if (parent)
                        {
                            parent->lastChild = node;
                        }

                        if (!layout)
                            layout = node;

                        double wdth = 0, hght = 0;
                        node->child = createLayoutTree(tnode->child, node);

                        tnode = tnode->next;
                        continue;
                    }
                }
            }
        }

        tnode = tnode->next;
    }

    if (returnLayout)
        return layout;
    else
        return NULL;
}

void BFCLayout(LayoutNode *root, double x, double y, double *w, double *h)
{
    if (!root)
        return;

    LayoutNode *node = root;
    int offX = x;
    int offY = y;

    while (node)
    {
        if (node->type != 2 && node->tag)
        {
            node->layout.x = offX + node->tag->style.marginleft;
            node->layout.y = offY + node->tag->style.margintop;
            
            if (node->tag->style.width)
            {
                node->layout.w = node->tag->style.paddingleft + node->tag->style.width + node->tag->style.paddingright;
            }
            else
            {
                if (node->parent)
                    node->layout.w = node->tag->style.paddingleft + node->parent->layout.w + node->tag->style.paddingright;
                else
                    node->layout.w = WINDOW_W;
            }

            printf("\n\n%d\n\n", node->layout.w);

            double w = 0, h = 0;

            BFCLayout(node->child, node->layout.x, node->layout.y, &w, &h);
            node->layout.h = node->tag->style.paddingtop + h + node->tag->style.paddingbottom;

            offY += node->tag->style.margintop + node->layout.h + node->tag->style.marginbottom;
        }
        else
        {
            node->layout.x = offX;
            node->layout.y = offY;
        
            if (node->parent)
                node->layout.w = node->parent->layout.w;
            else
                node->layout.w = WINDOW_W;

            double w = 0, h = 0;

            IFCLayout(node->child, node, node->layout.x, node->layout.y, &w, &h);
            node->layout.h = h;

            offY += node->layout.h;
        }

        node = node->next;
    }
}

void IFCLayout(LayoutNode *root, LayoutNode *parent, double x, double y, double *w, double *h)
{
    if (!root)
        return;
    if (!parent)
        return;

    LayoutNode *node = root;
    int offX = x;
    int offY = y;
    while (node)
    {
        if (node->tag->isText)
        {
            if (!node->tag->text)
            {
                TTF_Font *font;

                if (node->tag->style.fontweight >= 500)
                {
                    font = TTF_OpenFont("assets/Poppins/Poppins-Bold.ttf", node->tag->style.fontsize);
                }
                else if (node->tag->style.fontweight >= 400)
                {
                    font = TTF_OpenFont("assets/Poppins/Poppins-Medium.ttf", node->tag->style.fontsize);
                }
                else
                {
                    font = TTF_OpenFont("assets/Poppins/Poppins-Regular.ttf", node->tag->style.fontsize);
                }

                node->tag->text = parseText(node->tag->content, font, node->tag->style.color);
            }

            LineNode *line = parent->lastLine;
            Text *word = node->tag->text;

            if (!line)
            {
                LineNode *l = calloc(sizeof(LineNode), 1);
                parent->lines = l;
                parent->lastLine = l;
                line = l;
                line->height = word->height;
            }

            if (line->width + 8 + word->width > parent->layout.w)
            {
                if(line->lastWord) line->lastWord->next = NULL;
                LineNode *l = calloc(sizeof(LineNode), 1);
                parent->lastLine = l;
                line->next = l;
                line = l;
                line->height = word->height;
            }

            if (line->height < word->height)
                line->height = word->height;

            while (word)
            {
                printf("%s [%d %d %d] ", word->content, word->width, line->width, parent->layout.w);
                Text *next = word->next;
                word->layout.w = word->width;
                word->layout.h = word->height;

                if (line->width + 8 + word->width > parent->layout.w)
                {
                    line->lastWord->next = NULL;
                    LineNode *l = calloc(sizeof(LineNode), 1);
                    parent->lastLine = l;
                    line->next = l;
                    line = l;
                    line->height = word->height;
                }

                if (line->text)
                {
                    word->layout.x = line->width + 8;
                    line->width += 8 + word->width;
                }
                else
                {
                    word->layout.x = line->width;
                    line->width += word->width;

                    line->text = word;
                    line->lastWord = word;
                }

                word->layout.y = y + line->height - word->height;

                line->lastWord = word;

                word = next;
            }

            printf("\n");
        }
        else
        {
            IFCLayout(node->child, parent, x, y, w, h);
        }

        node = node->next;
    }
}

void layout1(TagNode *root, double x, double y, double *width, double *height)
{
    if (!root)
        return;

    if (root->name && (!strcasecmp(root->name, "head") || !strcasecmp(root->name, "style") || !strcasecmp(root->name, "script")))
    {
        layout(root->next, x, y, width, height);
        return;
    }

    if (root->style.displayOuter == DISPLAY_OUTER_NONE)
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
        {
            TagNode *lastBlock = root->parent;
            while (lastBlock->parent && lastBlock->style.displayOuter != DISPLAY_OUTER_BLOCK)
            {
                lastBlock = lastBlock->parent;
            }

            int totalTextWidth = 0;
            int widthSoFar = 0;
            int isFullWidth = 0;
            int textHeight = 0;
            Text *text = root->text;
            while (text)
            {

                if (!isFullWidth && widthSoFar + x > lastBlock->layout.w)
                {
                    isFullWidth = 1;
                    widthSoFar = 0;
                }

                if (isFullWidth && widthSoFar > lastBlock->layout.w)
                {
                    widthSoFar = 0;
                    textHeight += text->height;
                }

                widthSoFar += text->width;
                totalTextWidth += text->width;
                if (text->next)
                {
                    widthSoFar += 8;
                    totalTextWidth += 8;
                }
                text = text->next;
            }
        }

        // layout(root->next);
        else
        {
            root->layout.w = WINDOW_W;
            root->layout.h = 100; // Will fix later
            *height += root->layout.h;
        }
    }
    else
    {
        if (root->parent)
        {
            if (root->parent->style.displayInner == DISPLAY_INNER_FLOW)
            {
                if (root->style.displayOuter == DISPLAY_OUTER_BLOCK)
                {
                    TagNode *lastBlock = root->parent;

                    while (lastBlock->parent && root->parent->style.displayOuter == DISPLAY_OUTER_INLINE)
                    {
                        lastBlock = lastBlock->parent;
                    }

                    if (root->style.width)
                        root->layout.w = root->style.width + root->style.paddingright + root->style.paddingleft;
                    else
                        root->layout.w = lastBlock->layout.w - root->parent->style.paddingright - root->parent->style.paddingleft + root->style.paddingright + root->style.paddingleft;
                    // }

                    root->layout.x = lastBlock->layout.x + lastBlock->style.paddingleft + root->style.marginleft;
                    root->layout.y = y + root->style.margintop;

                    // if (root->parent->style.displayInner == DISPLAY_INNER_FLEX && root->parent->style.alignItems == 2)
                    // {
                    //     root->layout.x = x + root->parent->layout.w / 2 - root->layout.w / 2;
                    // }

                    double wdth = 0, hght = 0;
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
                    *width += root->layout.w + root->style.marginleft + root->style.marginright;

                    // layout(root->next, root->layout.x - root->style.marginleft, root->layout.y + root->layout.h + root->style.marginbottom, width, height);
                    // return;
                }
                else if (root->style.displayOuter == DISPLAY_OUTER_INLINE)
                {
                    // if (root->style.width)
                    //     root->layout.w = root->style.width + root->style.paddingright + root->style.paddingleft;
                    // else
                    //     root->layout.w = root->parent->layout.w - root->parent->style.paddingright - root->parent->style.paddingleft + root->style.paddingright + root->style.paddingleft;
                    // }

                    root->layout.x = x + root->style.marginleft;
                    root->layout.y = y + root->style.margintop;
                    if (root->parent->style.alignItems == 2)
                    {
                        root->layout.x = x + root->parent->layout.w / 2 - root->layout.w / 2;
                    }

                    double wdth = root->style.paddingleft + root->style.paddingright, hght = root->style.paddingtop + root->style.paddingbottom;
                    if (root->child)
                        layout(root->child, root->layout.x + root->style.paddingleft, root->layout.y + root->style.paddingtop, &wdth, &hght);

                    hght += root->style.paddingtop + root->style.paddingbottom;
                    if (root->style.height)
                        hght = root->style.height + root->style.paddingtop + root->style.paddingbottom;
                    root->layout.h = hght;

                    if (root->style.width)
                        wdth = root->style.width + root->style.paddingleft + root->style.paddingright;
                    root->layout.w = wdth;
                    *height += hght + root->style.margintop + root->style.marginbottom;
                    *width += root->layout.w + root->style.marginleft + root->style.marginright;
                }
            }
            else if (root->parent->style.displayInner == DISPLAY_INNER_FLEX)
            {
                flexLayout(root, x, y, width, height);
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
            *width += root->layout.w + root->style.marginleft + root->style.marginright;
        }
    }
}

void printLayoutTree(LayoutNode *root, int off)
{
    if (!root)
        return;
    int i = 0;
    while (i < off)
    {
        printf("  ");
        i++;
    }
    if (root->type == 2)
    {
        printf("<Anonymous Tag>\n");
        LineNode *line = root->lines;
        while (line)
        {
            Text *word = line->text;
            while (word)
            {
                printf(" %s", word->content);

                word = word->next;
            }

            printf("\n");

            line = line->next;
        }
    }
    else if (root->tag && root->tag->name)
        printf("%s", root->tag->name);
    else if (root->tag && root->tag->isText)
    {
        printf("<Text: %.*s>", 10, root->tag->content);
    }
    else
        printf("<NULL>");
    printf("\n");
    printLayoutTree(root->child, off + 1);
    printLayoutTree(root->next, off);
}

void flexLayout(TagNode *root, double x, double y, int *width, int *height)
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
        *width += root->layout.w + root->style.marginleft + root->style.marginright;
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
        *width += root->layout.w + root->style.marginleft + root->style.marginright;
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
                if (s)
                {
                    node->width = s->w;
                    node->height = s->h;
                }
                else
                {
                    node->width = 0;
                    node->height = 0;
                }

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

    tab->hoveredElement = NULL;
    renderTag(tab->DOM, tab);

    if (!tab->hoveredElement)
        return;

    TagNode *elm = tab->hoveredElement;
    SDL_Rect r = elm->layout.r;
    r.y -= tab->scrollY - 2 * BORDER_HEIGHT;

    SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, 255);
    SDL_RenderDrawRect(renderer, &r);
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

            // SDL_SetRenderDrawColor(renderer, 120, 200, 100, 255);
            SDL_SetRenderDrawColor(renderer, ptr->style.background.r, ptr->style.background.g, ptr->style.background.b, ptr->style.background.a);
            SDL_Rect r = ptr->layout.r;

            r.y -= tab->scrollY - 2 * BORDER_HEIGHT;

            if (r.x + r.w < 0 || r.x > WINDOW_W || r.y > WINDOW_H || r.y + r.h < 0)
            {
                ptr = ptr->next;
                continue;
            }

            if (MouseInitialized && MouseX > r.x && MouseX < r.x + r.w && MouseY > r.y && MouseY < r.y + r.h)
            {
                tab->hoveredElement = ptr;
            }

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

void renderLayout(LayoutNode *root, Tab *tab)
{
    if (!root)
        return;

    LayoutNode *ptr = root;

    while (ptr)
    {

        if (ptr->type == 2)
        {
        }
        else
        {
        }

        if (ptr->child)
        {
            renderLayout(ptr->child, tab);
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
