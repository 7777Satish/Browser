#ifndef CSS_UTILS_H
#define CSS_UTILS_H

#include <stdio.h>

typedef struct CSSBlockNode{
    char* name;
    char* content;
    int length;
    struct CSSBlockNode* next;
    struct CSSBlockNode* child;
} CSSBlockNode;

void printParsedStyles(CSSBlockNode *l, int offset);

#endif