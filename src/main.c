#include "renderer.h"
#include "utils.h"


int main() {
    
    initRenderer();

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");


    if (!window || !renderer) {
        return -1;
    }

    int running = 1;
    SDL_Event event;

    addTab("New Tab", "");
    addTab("Google", "");

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (event.type == SDL_MOUSEWHEEL) {
                if (mouseY < BORDER_HEIGHT) {
                    int scroll = event.wheel.y;

                    if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                        scroll = -scroll;
                    }

                    tabOffset -= scroll * 20;
                }
            }

            if(event.type == SDL_TEXTINPUT){
                // strcpy(searchBar.text, event.text.text);
                printf("%s\n", event.text.text);
            }

            if(event.type == SDL_MOUSEBUTTONDOWN){
                int x = event.motion.x, y = event.motion.y;

                if(y>BORDER_HEIGHT && y<2*BORDER_HEIGHT){
                    if(x>searchBar.rect.x && x<searchBar.rect.w && y>searchBar.rect.y && y<searchBar.rect.h){
                        searchBar.r1 = 1;
                    }
                }

                if(y<BORDER_HEIGHT+50){
                    int i=0;
                    Tab* ptr = tabHead;
                    while(ptr!=NULL){

                        int a = TAB_WIDTH * i + 2 * BORDER_PADDING * (i+1) + BORDER_ICON_H/2 - tabOffset;
                        
                        int dx = x - (a + TAB_WIDTH - 2*BORDER_PADDING - BORDER_ICON_H);
                        int dy = y-BORDER_HEIGHT/2;

                        if(dx*dx + dy*dy <= (BORDER_ICON_H)*(BORDER_ICON_H)){
                            closeTab(ptr);
                            i++;
                            ptr = ptr->next;
                            continue;
                        }

                        if(x > BORDER_PADDING*2*(i+1) + TAB_WIDTH*i - tabOffset && x < BORDER_PADDING*2*(i+1) + TAB_WIDTH*(i+1) - tabOffset && y>BORDER_PADDING*1.3 && y<BORDER_HEIGHT - 2.6 * BORDER_PADDING){
                            currentTab = ptr;
                        }

                        i++;
                        ptr = ptr->next;
                    }

                }

                if(x>WINDOW_W - 200 && y<100){
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
                        WINDOW_W - BORDER_PADDING*2 - BORDER_ICON_H + BORDER_ICON_H/2,
                        BORDER_HEIGHT/2,
                        BORDER_ICON_H,
                        BORDER_ICON_H
                    };

                    // if( (x - r1.x) * (x - r1.x) + (y - r1.y) * (y - r1.y) <= (BORDER_ICON_H/2 + 5) * (BORDER_ICON_H/2 + 5) ){
                    //     addTab(&tabHead, &tabTail, "New Tab", "");
                    // }

                    // if( (x - r2.x) * (x - r2.x) + (y - r2.y) * (y - r2.y) <= (BORDER_ICON_H/2 + 5) * (BORDER_ICON_H/2 + 5) ){
                    //     borderIcons.r4 = 1;
                    // } else {
                    //     borderIcons.r4 = 0;
                    // }

                    // Add Tab Button
                    if( (x - r3.x) * (x - r3.x) + (y - r3.y) * (y - r3.y) <= (BORDER_ICON_H/2 + 5) * (BORDER_ICON_H/2 + 5) ){
                        addTab("New Tab", "");
                    }
                }

            }

            if(event.type == SDL_MOUSEBUTTONUP){
                searchBar.r1 = 0;
            }

            if(event.type == SDL_MOUSEMOTION){
                int x = event.motion.x, y = event.motion.y;

                if(x>WINDOW_W - 200 && y<100){
                    SDL_Rect r1 = {
                        WINDOW_W - BORDER_PADDING*6 - 3*BORDER_ICON_H + BORDER_ICON_H/2,
                        BORDER_HEIGHT/2,
                        BORDER_ICON_H,
                        BORDER_ICON_H
                    };
                    SDL_Rect r2 = {
                        WINDOW_W - BORDER_PADDING*4 - 2*BORDER_ICON_H + BORDER_ICON_H/2,
                        BORDER_HEIGHT/2,
                        BORDER_ICON_H,
                        BORDER_ICON_H
                    };
                    SDL_Rect r3 = {
                        WINDOW_W - BORDER_PADDING*2 - BORDER_ICON_H + BORDER_ICON_H/2,
                        BORDER_HEIGHT/2,
                        BORDER_ICON_H,
                        BORDER_ICON_H
                    };

                    if( (x - r1.x) * (x - r1.x) + (y - r1.y) * (y - r1.y) <= (BORDER_ICON_H/2 + 5) * (BORDER_ICON_H/2 + 5) ){
                        borderIcons.r2 = 1;
                    } else {
                        borderIcons.r2 = 0;
                    }

                    if( (x - r2.x) * (x - r2.x) + (y - r2.y) * (y - r2.y) <= (BORDER_ICON_H/2 + 5) * (BORDER_ICON_H/2 + 5) ){
                        borderIcons.r4 = 1;
                    } else {
                        borderIcons.r4 = 0;
                    }

                    if( (x - r3.x) * (x - r3.x) + (y - r3.y) * (y - r3.y) <= (BORDER_ICON_H/2 + 5) * (BORDER_ICON_H/2 + 5) ){
                        borderIcons.r6 = 1;
                    } else {
                        borderIcons.r6 = 0;
                    }
                }

            }

        }

        SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawBorder(tabHead, tabOffset);
        drawSearchBar();
        renderPage(currentTab);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    clearTabs();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
