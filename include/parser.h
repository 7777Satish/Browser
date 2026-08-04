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

typedef enum {
    BLOCK,
    INLINE,
    INLINE_BLOCK,
    FLEX,
    GRID
} DISPLAY;

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
    int maxWidth;
    int maxHeight;
    int fontsize;
    int fontweight;
    int lineheight;
    int textalign;
    int borderRadius;
    int border;
    int bordertop;
    int borderbottom;
    int borderleft;
    int borderright;
    DISPLAY display;
    int gap;
    int flexDirection;
    int alignItems;
    char* fontfamily;
    char* position;
} Styles;

typedef struct{
    double x;
    double y;
    double w;
    double h;
    SDL_Rect r;
} Layout;

typedef struct Text{
    SDL_Surface* s;
    SDL_Texture* t;
    char* content;
    int width;
    int height;
    struct Text* next;
} Text;

typedef struct ContentNode
{
    char* content;
    int len;
    struct ContentNode* next;
} ContentNode;

typedef struct attributeNode
{
    char* name;
    char* value;
    int len;
    struct attributeNode* next;
} attributeNode;

typedef struct TagNode{
    int isText;
    int isClosing;
    char* name;
    char* content;
    int type;
    Layout layout;
    Styles style;
    ContentNode* styleContentNodes;
    attributeNode* attributes;
    ContentNode* classes;
    char* attr_id;
    char* attr_class;
    char* attr_style;
    struct TagNode* next;
    struct TagNode* prev;
    struct TagNode* child;
    struct TagNode* parent;
    struct TagNode* lastChild;
    SDL_Texture* t1;
    Text* text;
} TagNode;

typedef struct StyleNode{
    TagNode* node;
    struct StyleNode* next;
} StyleNode;

typedef struct CSSBlockNode{
    char* name;
    char* content;
    int length;
    struct CSSBlockNode* next;
    struct CSSBlockNode* child;
} CSSBlockNode;

typedef struct Tab Tab;

void layout(TagNode *root, double x, double y, double *w, double *h);
void createDOM(char* file_content, Tab** tab);
void renderDOM(Tab* tab);
void createCSOM(char* file_content, Tab** tab);
void renderTag(TagNode** tag, int* j, Tab** tab);

// void parseHTML(const char* html, Tag* tags, int* tagCount);
// void parseTag(const char* tagStr, Tag* tag);
// void parseAttributes(const char* attrStr, Tag* tag);


#endif