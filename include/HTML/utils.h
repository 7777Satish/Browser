#ifndef HTML_UTILS_H
#define HTML_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSS/parser.h"
#include "HTML/parser.h"

typedef struct Tab Tab;
typedef struct TagNode TagNode;

int isVoidTag(char *name);
void printlist(TagNode *list, StyleNode *stylenodes, int x);

#endif