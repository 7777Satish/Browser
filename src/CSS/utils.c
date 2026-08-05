#include "CSS/utils.h"

void printParsedStyles(CSSBlockNode *l, int offset)
{
    if (!l)
        return;
    CSSBlockNode *node = l;
    while (node)
    {
        // CSSBlockNode *child = node;
        // while (child)
        // {
        //     if (child && child->name)
        //         printf("->|%s|", child->name);

        //     if (!child->child && child->content)
        //         printf("\n%.*s\n", child->length, child->content);

        //     child = child->child;
        // }
        for (int i = 0; i < offset; i++)
            printf("  ");

        if (node && node->name)
            printf("%s\n", node->name);
        if (node->content)
        {
            for (int i = 0; i < offset; i++)
                printf("  ");
            printf("  -> %.*s\n", node->length, node->content);
        }

        printParsedStyles(node->child, offset + 1);

        printf("\n");
        node = node->next;
    }
}

