#ifndef HTML_LAYOUT_H
#define HTML_LAYOUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct Tab Tab;
typedef struct TagNode TagNode;

typedef struct{
    double x;
    double y;
    double w;
    double h;
    SDL_Rect r;
} Layout;

typedef struct LineNode
{
    Text* text;
    struct LineNode* next;
    struct LineNode* prev;
    int width;
    int height;
} LineNode;

typedef struct LayoutNode{
    Layout layout;
    TagNode* tag;
    int type;
    struct LineNode* lines;
    struct LineNode* lastLine;
    struct LayoutNode* next;
    struct LayoutNode* prev;
    struct LayoutNode* parent;
    struct LayoutNode* child;
    struct LayoutNode* lastChild;
} LayoutNode;

typedef struct Text{
    SDL_Surface* s;
    SDL_Texture* t;
    char* content;
    int width;
    int height;
    struct Text* next;
} Text;

LayoutNode *createLayoutTree(TagNode *root, LayoutNode *parent, double x, double y, int *width, int *height);
void layout(TagNode *root, double x, double y, double *width, double *height);
void printLayoutTree(LayoutNode* root, int off);
Text *parseText(char *content, TTF_Font *font, SDL_Color fg);
void renderDOM(Tab *tab);
void renderTag(TagNode *tag, Tab *tab);

#endif