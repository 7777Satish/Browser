#ifndef CSS_PARSER_H
#define CSS_PARSER_H

#include "CSS/utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

typedef struct attributeNode
{
    char* name;
    char* value;
    int len;
    struct attributeNode* next;
} attributeNode;


typedef struct TagNode TagNode;

typedef struct StyleNode{
    TagNode* node;
    struct StyleNode* next;
} StyleNode;

typedef struct CSSBlockNode CSSBlockNode;
typedef struct Tab Tab;

void parseCSS(TagNode *DOM);
void parseStyle(TagNode *tag);
int parseInt(char *str, int start, int end);
void parseColor(char *str, int start, int end, int *r, int *g, int *b, int *a);
void parseRGB(char *str, int start, int end, int *r, int *g, int *b);
CSSBlockNode *parseFromStyle(const char *source);
void applyCSOMtoDOM(TagNode *DOM, CSSBlockNode *CSOM);
CSSBlockNode *insertToCSS(CSSBlockNode *final, CSSBlockNode *current);
CSSBlockNode *parseFromStyle(const char *source);

#endif