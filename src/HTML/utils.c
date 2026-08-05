#include "HTML/utils.h"

int isVoidTag(char *name)
{
    if (!name)
        return 0;
    if (!strcmp(name, "br") || !strcmp(name, "meta") || !strcmp(name, "hr") || !strcmp(name, "img") || !strcmp(name, "link") || !strcmp(name, "input") || !strcmp(name, "area") || !strcmp(name, "base") || !strcmp(name, "col") || !strcmp(name, "embed") || !strcmp(name, "source") || !strcmp(name, "track") || !strcmp(name, "wbr") || !strcmp(name, "param"))
    {
        return 1;
    }
    return 0;
}

void printlist(TagNode *list, StyleNode *stylenodes, int x)
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

    StyleNode *temp = stylenodes;
    while (temp)
    {
        if (temp->node && temp->node->child)
            printf("%s\n", temp->node->child->content);
        temp = temp->next;
    }
}
