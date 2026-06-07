#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "utils.h"

typedef struct ImageElms{
    char src1[30];
    char src2[30];
    char src3[30];
    char src4[30];
    char src5[30];
    char src6[30];
    SDL_Texture* t1;
    SDL_Texture* t2;
    SDL_Texture* t3;
    SDL_Texture* t4;
    SDL_Texture* t5;
    SDL_Texture* t6;
    int r1;
    int r2;
    int r3;
    int r4;
    int r5;
    int r6;
} ImageElms;

typedef struct TagNode TagNode;

typedef struct{
    SDL_Rect rect;
    int r1;
    int dth;
    int dtw;
    int th;
    int tw;
    char* text;
    char* defaultText;
    SDL_Texture* t1;
    SDL_Texture* t2;
} SearchBar;


typedef struct Tab{
    char title[20];
    // int scrollX;
    int scrollY;
    char* src;
    char* logoSrc;
    SDL_Texture* t1;
    SDL_Texture* t2;
    int r1;
    int r2;
    struct Tab* next;
    struct Tab* prev;
    TagNode* DOM;
} Tab;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern int WINDOW_W;
extern int WINDOW_H;
extern int BORDER_HEIGHT;
extern int BORDER_PADDING;
extern int BORDER_ICON_H;
extern int TAB_WIDTH;

extern TTF_Font* poppins_regular;
extern TTF_Font* poppins_bold;

extern SDL_Surface* logoSurface;
extern SDL_Texture* logoTexture;

extern ImageElms borderIcons;
extern ImageElms searchIcons;
extern SearchBar searchBar;

extern SDL_Color tab_fg;

void initRenderer();
void initImageElms(ImageElms *elms);
void drawBorder(Tab* tabHead, int tabOffset);
void drawSearchBar();
void renderPage(Tab* tab);

#endif