#include "renderer.h"
#include "utils.h"
#include "network.h"

int main()
{
    srand(1000);
    initRenderer();

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    if (!window || !renderer)
    {
        return -1;
    }

    int running = 1;
    SDL_Event event;

    addTab("New Tab", "");
    addTab("Google", "");

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (event.type == SDL_MOUSEWHEEL)
            {
                if (mouseY < BORDER_HEIGHT)
                {
                    int scroll = event.wheel.y;

                    if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
                    {
                        scroll = -scroll;
                    }

                    tabOffset -= scroll * 20;

                    if (tabOffset < 0)
                        tabOffset = 0;
                }
                else if (mouseY > 2 * BORDER_HEIGHT)
                {

                    int scroll = event.wheel.y;

                    if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
                    {
                        scroll = -scroll;
                    }

                    currentTab->scrollY -= scroll * 40;
                    if (currentTab->scrollY < 0)
                        currentTab->scrollY = 0;
                }
            }

            if (event.type == SDL_TEXTINPUT)
            {
                SDL_Keymod mods = SDL_GetModState();
                if (searchBar.r1 == 1 && !(mods & KMOD_CTRL))
                {
                    size_t len = strlen(searchBar.text);
                    size_t len2 = strlen(event.text.text);
                    char *txt = realloc(searchBar.text, len + len2 + 1);
                    searchBar.text = txt;
                    strcat(searchBar.text, event.text.text);
                    SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, searchBar.text, (SDL_Color){205, 205, 205, 255});
                    SDL_Texture *t1 = SDL_CreateTextureFromSurface(renderer, s1);
                    SDL_DestroyTexture(searchBar.t2);
                    searchBar.t2 = t1;
                    searchBar.tw = s1->w;
                    searchBar.th = s1->h;
                    SDL_FreeSurface(s1);
                    
                    SHOW_SEARCH_SUGGESTION = 1;
                }
            }

            if (event.type == SDL_KEYDOWN)
            {
                SDL_KeyCode key = event.key.keysym.sym;
                if (searchBar.r1)
                {
                    // Backspace
                    if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
                    {
                        size_t len = strlen(searchBar.text);
                        if (len > 0)
                        {
                            if (event.key.keysym.mod & KMOD_CTRL)
                            {
                                while (len > 1 && searchBar.text[len - 2] != ' ')
                                {
                                    len--;
                                }
                            }
                            searchBar.text[len - 1] = '\0';

                            if (len == 1)
                            {
                                SDL_DestroyTexture(searchBar.t2);
                                searchBar.t2 = 0;
                                searchBar.tw = 0;
                            }
                            else
                            {
                                SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, searchBar.text, (SDL_Color){205, 205, 205, 255});
                                SDL_Texture *t1 = SDL_CreateTextureFromSurface(renderer, s1);
                                SDL_DestroyTexture(searchBar.t2);
                                searchBar.t2 = t1;
                                searchBar.tw = s1->w;
                                searchBar.th = s1->h;
                                SDL_FreeSurface(s1);
                                
                                SHOW_SEARCH_SUGGESTION = 1;
                            }
                        }
                    }
                }

                if(key == SDLK_ESCAPE){
                    searchBar.r1 = 0;
                }

                if ((event.key.keysym.mod & KMOD_CTRL) &&
                    key == SDLK_e)
                {
                    searchBar.r1 = 1;
                }

                if ((event.key.keysym.mod & KMOD_CTRL) &&
                    key == SDLK_t)
                {
                    addTab("New Tab", "");
                }

                if ((event.key.keysym.mod & KMOD_CTRL) &&
                    key == SDLK_w)
                {
                    closeTab(currentTab);
                    if (!currentTab)
                        running = 0;
                }

                if (tabHead && (event.key.keysym.mod & KMOD_CTRL) &&
                    key >= SDLK_1 && key < SDLK_9)
                {
                    int tab = key - SDLK_1;
                    Tab *node = tabHead;
                    int i = 0;
                    for (i; i < tab; i++)
                    {
                        if (!node->next)
                            break;
                        node = node->next;
                    }
                    currentTab = node;

                    double w = 0, h = 0;
                    currentTab->MAXHEIGHT = 0;
                    layout(currentTab->DOM, 0, 0, &w, &h);
                }

                if (tabHead && (event.key.keysym.mod & KMOD_CTRL) &&
                    key == SDLK_9)
                {
                    currentTab = tabTail;

                    double w = 0, h = 0;
                    currentTab->MAXHEIGHT = 0;
                    layout(currentTab->DOM, 0, 0, &w, &h);
                }

                if (tabHead && (event.key.keysym.mod & KMOD_CTRL) && (event.key.keysym.mod & KMOD_SHIFT) &&
                    key == SDLK_TAB)
                {
                    if (currentTab->prev)
                        currentTab = currentTab->prev;
                    else
                        currentTab = tabTail;

                    double w = 0, h = 0;
                    currentTab->MAXHEIGHT = 0;
                    layout(currentTab->DOM, 0, 0, &w, &h);
                }
                else if (tabHead && (event.key.keysym.mod & KMOD_CTRL) &&
                         key == SDLK_TAB)
                {
                    if (currentTab->next)
                        currentTab = currentTab->next;
                    else
                        currentTab = tabHead;

                    double w = 0, h = 0;
                    currentTab->MAXHEIGHT = 0;
                    layout(currentTab->DOM, 0, 0, &w, &h);
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.motion.x, y = event.motion.y;

                if (y > BORDER_HEIGHT && y < 2 * BORDER_HEIGHT)
                {
                    if (x > searchBar.rect.x && x < searchBar.rect.w && y > searchBar.rect.y && y < searchBar.rect.h)
                    {
                        searchBar.r1 = 1;
                    }
                    else
                        searchBar.r1 = 0;
                        SHOW_SEARCH_SUGGESTION = 0;
                }
                else
                    searchBar.r1 = 0;
                    SHOW_SEARCH_SUGGESTION = 0;

                if (y < BORDER_HEIGHT + 50)
                {
                    int i = 0;
                    Tab *ptr = tabHead;
                    while (ptr != NULL)
                    {

                        int a = TAB_WIDTH * i + 2 * BORDER_PADDING * (i + 1) + BORDER_ICON_H / 2 - tabOffset;

                        int dx = x - (a + TAB_WIDTH - 2 * BORDER_PADDING - BORDER_ICON_H);
                        int dy = y - BORDER_HEIGHT / 2;

                        if (dx * dx + dy * dy <= (BORDER_ICON_H) * (BORDER_ICON_H))
                        {
                            closeTab(ptr);
                            i++;
                            ptr = ptr->next;
                            continue;
                        }

                        if (x > BORDER_PADDING * 2 * (i + 1) + TAB_WIDTH * i - tabOffset && x < BORDER_PADDING * 2 * (i + 1) + TAB_WIDTH * (i + 1) - tabOffset && y > BORDER_PADDING * 1.3 && y < BORDER_HEIGHT - BORDER_PADDING)
                        {
                            currentTab = ptr;
                            double w = 0, h = 0;
                            currentTab->MAXHEIGHT = 0;
                            layout(currentTab->DOM, 0, 0, &w, &h);
                        }

                        i++;
                        ptr = ptr->next;
                    }
                }

                if (x > WINDOW_W - 200 && y < 100)
                {
                    // SDL_Rect r1 = {
                    //     WINDOW_W - BORDER_PADDING*6 - 3*BORDER_ICON_H + BORDER_ICON_H/2,
                    //     BORDER_HEIGHT/2,
                    //     BORDER_ICON_H,
                    //     BORDER_ICON_H
                    // };
                    // SDL_Rect r2 = {
                    //     WINDOW_W - BORDER_PADDING*4 - 2*BORDER_ICON_H + BORDER_ICON_H/2,
                    //     BORDER_HEIGHT/2,
                    //     BORDER_ICON_H,
                    //     BORDER_ICON_H
                    // };

                    SDL_Rect r3 = {
                        WINDOW_W - BORDER_PADDING * 2 - BORDER_ICON_H + BORDER_ICON_H / 2,
                        BORDER_HEIGHT / 2,
                        BORDER_ICON_H,
                        BORDER_ICON_H};

                    // if( (x - r1.x) * (x - r1.x) + (y - r1.y) * (y - r1.y) <= (BORDER_ICON_H/2 + 5) * (BORDER_ICON_H/2 + 5) ){
                    //     addTab(&tabHead, &tabTail, "New Tab", "");
                    // }

                    // if( (x - r2.x) * (x - r2.x) + (y - r2.y) * (y - r2.y) <= (BORDER_ICON_H/2 + 5) * (BORDER_ICON_H/2 + 5) ){
                    //     borderIcons.r4 = 1;
                    // } else {
                    //     borderIcons.r4 = 0;
                    // }

                    // Add Tab Button
                    if ((x - r3.x) * (x - r3.x) + (y - r3.y) * (y - r3.y) <= (BORDER_ICON_H / 2 + 5) * (BORDER_ICON_H / 2 + 5))
                    {
                        addTab("New Tab", "");
                    }
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP)
            {
                // searchBar.r1 = 0;
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                int x = event.motion.x, y = event.motion.y;

                if (x > WINDOW_W - 200 && y < 100)
                {
                    SDL_Rect r1 = {
                        WINDOW_W - BORDER_PADDING * 6 - 3 * BORDER_ICON_H + BORDER_ICON_H / 2,
                        BORDER_HEIGHT / 2,
                        BORDER_ICON_H,
                        BORDER_ICON_H};
                    SDL_Rect r2 = {
                        WINDOW_W - BORDER_PADDING * 4 - 2 * BORDER_ICON_H + BORDER_ICON_H / 2,
                        BORDER_HEIGHT / 2,
                        BORDER_ICON_H,
                        BORDER_ICON_H};
                    SDL_Rect r3 = {
                        WINDOW_W - BORDER_PADDING * 2 - BORDER_ICON_H + BORDER_ICON_H / 2,
                        BORDER_HEIGHT / 2,
                        BORDER_ICON_H,
                        BORDER_ICON_H};

                    if ((x - r1.x) * (x - r1.x) + (y - r1.y) * (y - r1.y) <= (BORDER_ICON_H / 2 + 5) * (BORDER_ICON_H / 2 + 5))
                    {
                        borderIcons.r2 = 1;
                    }
                    else
                    {
                        borderIcons.r2 = 0;
                    }

                    if ((x - r2.x) * (x - r2.x) + (y - r2.y) * (y - r2.y) <= (BORDER_ICON_H / 2 + 5) * (BORDER_ICON_H / 2 + 5))
                    {
                        borderIcons.r4 = 1;
                    }
                    else
                    {
                        borderIcons.r4 = 0;
                    }

                    if ((x - r3.x) * (x - r3.x) + (y - r3.y) * (y - r3.y) <= (BORDER_ICON_H / 2 + 5) * (BORDER_ICON_H / 2 + 5))
                    {
                        borderIcons.r6 = 1;
                    }
                    else
                    {
                        borderIcons.r6 = 0;
                    }
                }
            }

            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event == SDL_WINDOWEVENT_MAXIMIZED || event.window.event == SDL_WINDOWEVENT_RESTORED)
                {
                    double w = 0, h = 0;
                    currentTab->MAXHEIGHT = 0;
                    layout(currentTab->DOM, 0, 0, &w, &h);

                    SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);
                }
            }
        }
        if (!currentTab)
            running = 0;
        SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        renderPage(currentTab);
        drawBorder(tabHead, tabOffset);
        drawSearchBar();

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // SDL_Rect r = {0, 0, 100, 100};
        // SDL_SetRenderDrawColor(renderer, 100, 200, 255, 55);
        // SDL_RenderFillRect(renderer, &r);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    clearTabs();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
