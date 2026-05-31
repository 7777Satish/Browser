#ifndef PARSER_H
#define PARSER_H

#include "renderer.h"
#include "utils.h"

typedef struct ItemNode{
    int isText;
    int isClosing;
    char* name;
    char* content;
    struct ItemNode* next;
    struct ItemNode* prev;
} ItemNode;

typedef struct Styles
{
    SDL_Color background;
    SDL_Color color;
    int margin;
    int marginleft;
    int marginright;
    int margintop;
    int marginbottom;
    int padding;
    int paddingleft;
    int paddingright;
    int paddingtop;
    int paddingbottom;
    int width;
    int height;
    int fontsize;
    int borderRadius;
    char* fontfamily;
    char* display;
    char* position;
} Styles;

typedef struct TagNode{
    int isText;
    int isClosing;
    char* name;
    char* content;
    int type;
    Styles style;
    struct TagNode* next;
    struct TagNode* prev;
    struct TagNode* child;
    struct TagNode* parent;
    struct TagNode* lastChild;
    SDL_Texture* t1;
} TagNode;

typedef struct TagAttribute{
    TagNode* tag;
    char* name;
    char* value;
    struct TagAttribute* next;
    struct TagAttribute* prev;
} TagAttribute;

typedef struct Tab Tab;

void createDOM(char* file_content, Tab** tab);
void renderDOM(Tab* tab);
void createCSOM(char* file_content, Tab** tab);
void renderTag(TagNode** tag, int* j, Tab** tab);

// void parseHTML(const char* html, Tag* tags, int* tagCount);
// void parseTag(const char* tagStr, Tag* tag);
// void parseAttributes(const char* attrStr, Tag* tag);


#endif