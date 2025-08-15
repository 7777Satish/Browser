#include "utils.h"

Tab* tabHead = NULL;
Tab* tabTail = NULL;
Tab* currentTab = NULL;
int tabOffset = 0;
int tabN = 0;

void addTab(char title[], char* logoSrc){
    
    Tab* tab = (Tab*)malloc(sizeof(Tab));
    strcpy(tab->title, title);
    tab->DOM = NULL;

    if (logoSrc && strlen(logoSrc) > 0) {
        tab->logoSrc = malloc(strlen(logoSrc) + 1);
        if (tab->logoSrc) {
            strcpy(tab->logoSrc, logoSrc);
        } else {
            fprintf(stderr, "Memory allocation failed for logoSrc\n");
            tab->logoSrc = NULL;
        }
    } else {
        tab->logoSrc = NULL;
    }


    tab->r1 = 0;
    tab->r2 = 0;
    SDL_Surface* s1 = TTF_RenderText_Blended(poppins_bold, title, tab_fg);
    tab->t1 = SDL_CreateTextureFromSurface(renderer, s1);

    if(logoSrc){
        SDL_Surface* l = IMG_Load(logoSrc);
        tab->t2 = SDL_CreateTextureFromSurface(renderer, l);
    }
    
    if(tabHead == NULL){
        tab->next = NULL;
        tab->prev = NULL;
        tabHead = tab;
        tabTail = tab;
    }
    else{
        (tabTail)->next = tab;
        tab->next = NULL;
        tab->prev = tabTail;
        tabTail = tab;
    }
    currentTab = tab;
}

void closeTab(Tab* tab) {
    if (tab == NULL) return;

    if (tab->prev) {
        tab->prev->next = tab->next;
    } else {
        tabHead = tab->next;
    }

    if (tab->next) {
        tab->next->prev = tab->prev;
        if(currentTab==tab) currentTab = tab->next;
    } else {
        tabTail = tab->prev;
        if(currentTab==tab) currentTab = tab->prev;
    }

    if(!tab->next && !tab->prev){
        currentTab = NULL;
    }

    free(tab);
}



void clearTabs() {
    if (tabHead) return;

    Tab* temp = tabHead;
    while (temp != NULL) {
        Tab* ptr = temp;
        temp = temp->next;
        free(ptr);
    }

    tabHead = NULL;
    tabTail = NULL;
    tabN = 0;
    tabOffset = 0;
}
