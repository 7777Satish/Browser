#ifndef HTML_LAYOUT_H
#define HTML_LAYOUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

typedef struct Tab Tab;
typedef struct TagNode TagNode;

void layout(TagNode *root, double x, double y, double *width, double *height);
Text *parseText(char *content, TTF_Font *font, SDL_Color fg);
void renderDOM(Tab *tab);
void renderTag(TagNode *tag, Tab *tab);

#endif