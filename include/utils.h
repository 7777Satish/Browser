#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include "renderer.h"
#include "parser.h"

typedef struct Tab Tab;
typedef struct TagNode TagNode;


extern Tab* tabHead;
extern Tab* tabTail;
extern Tab* currentTab;
extern int tabOffset;
extern int tabN;

void addTab(char title[], char* logoSrc);
void closeTab(Tab* tab);
void clearTabs();

#endif