#include "utils/utils.h"

Tab *tabHead = NULL;
Tab *tabTail = NULL;
Tab *currentTab = NULL;
int tabOffset = 0;
int tabN = 0;

int I = 4;

void addTab(char title[], char *logoSrc)
{

    Tab *tab = calloc(1, sizeof(Tab));
    if (!tab)
    {
        fprintf(stderr, "addTab: failed to allocate Tab\n");
        return;
    }

    strcpy(tab->title, title);

    if (logoSrc && logoSrc[0] != '\0')
    {
        tab->logoSrc = malloc(strlen(logoSrc) + 1);

        if (tab->logoSrc == NULL)
        {
            fprintf(stderr, "addTab: failed to allocate logoSrc\n");
            free(tab);
            return;
        }

        strcpy(tab->logoSrc, logoSrc);
    }

    // tab->scrollX = 0;
    tab->scrollY = 0;
    tab->state = TAB_UNINITIALIZED;
    tab->r1 = 0;
    tab->r2 = 0;
    SDL_Surface *s1 = TTF_RenderText_Blended(poppins_bold, title, tab_fg);
    if (s1 == NULL)
    {
        fprintf(stderr, "addTab: failed to render title: %s\n", SDL_GetError());
        free(tab->logoSrc);
        free(tab);
        return;
    }

    tab->t1 = SDL_CreateTextureFromSurface(renderer, s1);
    if (tab->t1 == NULL)
    {
        fprintf(stderr, "addTab: failed to create title texture: %s\n", SDL_GetError());
        free(tab->logoSrc);
        free(tab);
        return;
    }

    tab->faviconColor.r = rand() % 255;
    tab->faviconColor.g = rand() % 255;
    tab->faviconColor.b = rand() % 255;
    tab->faviconColor.a = 255;

    if (logoSrc && strlen(logoSrc) > 0)
    {
        SDL_Surface *l = IMG_Load(logoSrc);

        if (l == NULL)
        {
            fprintf(stderr, "addTab: failed to load logo '%s': %s\n", logoSrc, IMG_GetError());
            SDL_DestroyTexture(tab->t1);
            free(tab->logoSrc);
            free(tab);
            return;
        }

        tab->t2 = SDL_CreateTextureFromSurface(renderer, l);
        if (tab->t2 == NULL)
        {
            fprintf(stderr, "addTab: failed to create logo texture: %s\n", SDL_GetError());
            SDL_DestroyTexture(tab->t1);
            free(tab->logoSrc);
            free(tab);
            return;
        }
    }

    if (tabHead == NULL)
    {
        tab->next = NULL;
        tab->prev = NULL;
        tabHead = tab;
        tabTail = tab;
    }
    else
    {
        (tabTail)->next = tab;
        tab->next = NULL;
        tab->prev = tabTail;
        tabTail = tab;
    }

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "pages/default%d.html", I % 6);

    tab->src = malloc(strlen(buffer) + 1);

    if (tab->src == NULL)
    {
        fprintf(stderr, "addTab: failed to allocate src\n");
        SDL_DestroyTexture(tab->t2);
        SDL_DestroyTexture(tab->t1);
        free(tab->logoSrc);
        free(tab);
        return;
    }

    strcpy(tab->src, buffer);

    I++;
    currentTab = tab;
}

void closeTab(Tab *tab)
{
    if (tab == NULL)
        return;

    if (tab->prev)
    {
        tab->prev->next = tab->next;
    }
    else
    {
        tabHead = tab->next;
    }

    if (tab->next)
    {
        tab->next->prev = tab->prev;
        if (currentTab == tab)
            currentTab = tab->next;
    }
    else
    {
        tabTail = tab->prev;
        if (currentTab == tab)
            currentTab = tab->prev;
    }

    if (!tab->next && !tab->prev)
    {
        currentTab = NULL;
    }

    if(tab->logoSrc) free(tab->logoSrc);
    if(tab->src) free(tab->src);
    if(tab->title) free(tab->title);

    if(tab->s1) SDL_FreeSurface(tab->s1);

    if(tab->t1) SDL_DestroyTexture(tab->t1);
    if(tab->t2) SDL_DestroyTexture(tab->t2);

    if(tab->DOM){
        
    }

    free(tab);
}

void clearTabs()
{
    if (!tabHead)
        return;

    Tab *temp = tabHead;
    while (temp != NULL)
    {
        Tab *ptr = temp;
        temp = temp->next;
        free(ptr);
    }

    tabHead = NULL;
    tabTail = NULL;
    tabN = 0;
    tabOffset = 0;
}
