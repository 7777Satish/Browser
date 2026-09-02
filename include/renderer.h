#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>
#include "utils/utils.h"
#include "HTML/layout.h"
#include "HTML/parser.h"
#include "network/network.h"

typedef struct ImageElms{
    char src1[30];
    char src2[30];
    char src3[30];
    char src4[30];
    char src5[30];
    char src6[30];
    char src7[30];
    char src8[30];
    char src9[30];
    char src10[30];
    SDL_Texture* t1;
    SDL_Texture* t2;
    SDL_Texture* t3;
    SDL_Texture* t4;
    SDL_Texture* t5;
    SDL_Texture* t6;
    SDL_Texture* t7;
    SDL_Texture* t8;
    SDL_Texture* t9;
    SDL_Texture* t10;
    int r1;
    int r2;
    int r3;
    int r4;
    int r5;
    int r6;
    int r7;
    int r8;
    int r9;
    int r10;
} ImageElms;

typedef struct TagNode TagNode;
typedef struct CSSBlockNode CSSBlockNode;

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

typedef struct StyleNode StyleNode;

typedef enum{
    TAB_UNINITIALIZED,
    TAB_LOADING,
    TAB_READY
} TAB_STATE;

typedef struct Tab{
    char title[20];
    TAB_STATE state;
    int scrollY;
    int MAXHEIGHT;
    SDL_Color faviconColor;
    char* src;
    char* logoSrc;
    SDL_Surface* s1;
    SDL_Texture* t1;
    SDL_Texture* t2;
    int r1;
    int r2;
    StyleNode* stylenodes;
    struct Tab* next;
    struct Tab* prev;
    TagNode* hoveredElement;
    TagNode* DOM;
    CSSBlockNode* CSOM;
    LayoutNode* LAYOUT;
} Tab;

struct ThreadTabData{
    Tab* tab;
    char* url;
};

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

extern int SHOW_SEARCH_SUGGESTION;

void initRenderer();
void initImageElms(ImageElms *elms);
void drawBorder(Tab* tabHead, int tabOffset);
void drawSearchBar();
void renderPage(Tab* tab);
void renderSetting();

#endif