#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include "renderer.h"
#include "HTML/layout.h"
#include "HTML/utils.h"
#include "CSS/parser.h"

typedef struct ItemNode{
    int isText;
    int isClosing;
    char* name;
    char* content;
    struct ItemNode* next;
    struct ItemNode* prev;
} ItemNode;

typedef struct ContentNode
{
    char* content;
    int len;
    struct ContentNode* next;
} ContentNode;

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

void parseTag(TagNode *tag);;
void createDOM(char *file_content, Tab **tab);

#endif
