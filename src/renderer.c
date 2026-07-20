#include "renderer.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int WINDOW_W = 1200;
int WINDOW_H = 850;

int BORDER_HEIGHT = 45;
int BORDER_PADDING = 5;
int BORDER_ICON_H = 15;
int TAB_WIDTH = 250;

TTF_Font *poppins_regular = NULL;
TTF_Font *poppins_bold = NULL;

SDL_Surface *logoSurface = NULL;
SDL_Texture *logoTexture = NULL;

SDL_Color tab_fg = {214, 214, 214};

ImageElms borderIcons = {
    .src1 = "assets/icons/logo.png",
    .src2 = "assets/icons/left.png",
    .src3 = "assets/icons/minus-square.png",
    .src4 = "assets/icons/right.png",
    .src5 = "assets/icons/x.png",
    .src6 = "assets/icons/add.png",
    .t1 = NULL,
    .t2 = NULL,
    .t3 = NULL,
    .t4 = NULL,
    .t5 = NULL,
    .t6 = NULL,
    .r1 = 0,
    .r2 = 0,
    .r3 = 0,
    .r4 = 0,
    .r5 = 0,
    .r6 = 0};

ImageElms searchIcons = {
    .src1 = "assets/icons/back.png",
    .src2 = "assets/icons/forward.png",
    .src3 = "assets/icons/x.png",
    .src4 = "assets/icons/reload.png",
    .src5 = "assets/icons/star.png",
    .src6 = "assets/icons/settings.png",
};

SearchBar searchBar = {
    .defaultText = "Search the web",
    .text = "",
    .r1 = 0,
    .t2 = NULL};

SDL_Color MAIN_COLOR = {35, 35, 35, 255};
SDL_Color ACCENT_COLOR = {71, 71, 71, 255};

void initRenderer()
{
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");   // best filtering for scaled textures
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0"); // allow native backing pixels
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    SDL_Init(0);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    window = SDL_CreateWindow("FlexIDE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    poppins_regular = TTF_OpenFont("assets/Poppins/Poppins-Regular.ttf", BORDER_ICON_H + 1);
    poppins_bold = TTF_OpenFont("assets/Poppins/Poppins-Bold.ttf", BORDER_ICON_H + 5);

    // Images
    logoSurface = IMG_Load("assets/icons/cube-scan.png");
    logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);

    SDL_Color color = {140, 140, 140};
    SDL_Surface *searchSurface = TTF_RenderText_Blended(poppins_regular, searchBar.defaultText, color);
    searchBar.t1 = SDL_CreateTextureFromSurface(renderer, searchSurface);
    searchBar.dtw = searchSurface->w;
    searchBar.dth = searchSurface->h;
    searchBar.text = malloc(1);
    searchBar.text[0] = '\0';
    searchBar.tw = 0;
    searchBar.th = BORDER_ICON_H + 5;
    SDL_Rect searchRect = {
        12 * BORDER_PADDING + 4 * BORDER_ICON_H,
        3 * BORDER_HEIGHT / 2 - BORDER_ICON_H - 1,
        WINDOW_W - (4 * BORDER_PADDING + BORDER_ICON_H),
        3 * BORDER_HEIGHT / 2 + BORDER_ICON_H + 1};
    searchBar.rect = searchRect;
}

void initImageElms(ImageElms *elms)
{
    SDL_Surface *s1 = IMG_Load(elms->src1);
    elms->t1 = SDL_CreateTextureFromSurface(renderer, s1);
    SDL_FreeSurface(s1);

    SDL_Surface *s2 = IMG_Load(elms->src2);
    elms->t2 = SDL_CreateTextureFromSurface(renderer, s2);
    SDL_FreeSurface(s2);

    SDL_Surface *s3 = IMG_Load(elms->src3);
    elms->t3 = SDL_CreateTextureFromSurface(renderer, s3);
    SDL_FreeSurface(s3);

    SDL_Surface *s4 = IMG_Load(elms->src4);
    elms->t4 = SDL_CreateTextureFromSurface(renderer, s4);
    SDL_FreeSurface(s4);

    SDL_Surface *s5 = IMG_Load(elms->src5);
    elms->t5 = SDL_CreateTextureFromSurface(renderer, s5);
    SDL_FreeSurface(s5);

    SDL_Surface *s6 = IMG_Load(elms->src6);
    elms->t6 = SDL_CreateTextureFromSurface(renderer, s6);
    SDL_FreeSurface(s6);
}

// Border
void drawBorder(Tab *tabHead, int tabOffset)
{

    if (borderIcons.t1 == 0)
    {
        initImageElms(&borderIcons);
    }

    SDL_Rect border_rect = {
        0, 0,
        WINDOW_W,
        BORDER_HEIGHT};

    // SDL_SetRenderDrawColor(renderer, 219, 188, 255, 255);
    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);

    SDL_RenderFillRect(renderer, &border_rect);

    Tab *temp = tabHead;
    int i = 0;

    SDL_SetRenderDrawColor(renderer, 150, 8, 255, 255);
    while (temp != NULL)
    {
        SDL_Rect r = {
            TAB_WIDTH * i + 2 * BORDER_PADDING * (i + 1) - tabOffset,
            BORDER_PADDING * 1.3,
            TAB_WIDTH,
            BORDER_HEIGHT - 2.6 * BORDER_PADDING};

        // Filled rounded tab
        if (temp == currentTab)
        {
            roundedBoxRGBA(renderer,
                           r.x,
                           r.y,
                           r.x + r.w,
                           r.y + r.h,
                           5,
                           // 237, 206, 255, 255
                           71, 71, 71, 255);
        }
        else
        {
            // roundedBoxRGBA(renderer,
            //                r.x,
            //                r.y,
            //                r.x + r.w,
            //                r.y + r.h,
            //                5,
            //                // 237, 206, 255, 255
            //                71, 71, 71, 255);

            SDL_SetRenderDrawColor(renderer, 101, 101, 101, 101);
            SDL_RenderDrawLine(renderer, r.x + r.w, r.y + 5, r.x + r.w, r.y + r.h - 5);
        }

        int w, h;
        SDL_QueryTexture(temp->t1, NULL, NULL, &w, &h);
        int FAVICON_WIDTH = 20;

        SDL_SetRenderDrawColor(renderer, temp->faviconColor.r, temp->faviconColor.g, temp->faviconColor.b, 255);
        SDL_Rect faviconRect = {
            r.x + BORDER_PADDING * 2,
            BORDER_HEIGHT / 2 - (FAVICON_WIDTH) / 2,
            // (w + 0.0) / h * (BORDER_ICON_H + 5),
            FAVICON_WIDTH,
            FAVICON_WIDTH};

        SDL_RenderFillRect(renderer, &faviconRect);

        int wdth = w > TAB_WIDTH - 40 - FAVICON_WIDTH ? TAB_WIDTH - 40 - FAVICON_WIDTH : w;
        SDL_Rect textRect = {
            r.x + BORDER_PADDING * 4 + FAVICON_WIDTH,
            BORDER_HEIGHT / 2 - (BORDER_ICON_H + 4.5) / 2,
            // (w + 0.0) / h * (BORDER_ICON_H + 5),
            (wdth + 0.0) / h * (BORDER_ICON_H + 4.5),
            BORDER_ICON_H + 4.5};

        SDL_Rect textRectSrc = {
            0,
            0,
            wdth,
            h};

        SDL_RenderCopy(renderer, temp->t1, &textRectSrc, &textRect);

        SDL_Rect buttonRect = {
            r.x + r.w - BORDER_PADDING * 2 - BORDER_ICON_H,
            BORDER_HEIGHT / 2 - BORDER_ICON_H / 2,
            BORDER_ICON_H,
            BORDER_ICON_H};
        SDL_RenderCopy(renderer, borderIcons.t5, NULL, &buttonRect);

        i++;
        temp = temp->next;
    }

    // SDL_RenderCopy(renderer, borderIcons.t1, NULL, &borderIcons.r1);

    SDL_Rect buttons_rect = {
        WINDOW_W - BORDER_PADDING * 8 - 3 * BORDER_ICON_H,
        0,
        BORDER_PADDING * 8 + 4 * BORDER_ICON_H,
        BORDER_HEIGHT};

    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
    SDL_RenderFillRect(renderer, &buttons_rect);

    SDL_Rect r1 = {
        WINDOW_W - BORDER_PADDING * 6 - 3 * BORDER_ICON_H,
        BORDER_HEIGHT / 2 - BORDER_ICON_H / 2,
        BORDER_ICON_H,
        BORDER_ICON_H};
    if (borderIcons.r2)
        filledCircleRGBA(renderer, r1.x + BORDER_ICON_H / 2, BORDER_HEIGHT / 2, BORDER_ICON_H / 2 + 5, 71, 71, 71, 255);
    SDL_RenderCopy(renderer, borderIcons.t2, NULL, &r1);

    SDL_Rect r2 = {
        WINDOW_W - BORDER_PADDING * 4 - 2 * BORDER_ICON_H,
        BORDER_HEIGHT / 2 - BORDER_ICON_H / 2,
        BORDER_ICON_H,
        BORDER_ICON_H};
    if (borderIcons.r4)
        filledCircleRGBA(renderer, r2.x + BORDER_ICON_H / 2, BORDER_HEIGHT / 2, BORDER_ICON_H / 2 + 5, 71, 71, 71, 255);
    SDL_RenderCopy(renderer, borderIcons.t4, NULL, &r2);

    SDL_Rect r3 = {
        WINDOW_W - BORDER_PADDING * 2 - BORDER_ICON_H,
        BORDER_HEIGHT / 2 - BORDER_ICON_H / 2,
        BORDER_ICON_H,
        BORDER_ICON_H};
    if (borderIcons.r6)
        filledCircleRGBA(renderer, r3.x + BORDER_ICON_H / 2, BORDER_HEIGHT / 2, BORDER_ICON_H / 2 + 5, 71, 71, 71, 255);
    SDL_RenderCopy(renderer, borderIcons.t6, NULL, &r3);
}

void drawSearchBar()
{

    if (!searchIcons.t1)
        initImageElms(&searchIcons);

    SDL_Rect search_rect = {
        0, BORDER_HEIGHT,
        WINDOW_W,
        BORDER_HEIGHT};

    SDL_SetRenderDrawColor(renderer, 71, 71, 71, 255);
    SDL_RenderFillRect(renderer, &search_rect);

    SDL_Rect r1 = {
        2 * BORDER_PADDING,
        3 * BORDER_HEIGHT / 2 - BORDER_ICON_H / 2,
        BORDER_ICON_H,
        BORDER_ICON_H};
    SDL_RenderCopy(renderer, searchIcons.t1, NULL, &r1);

    SDL_Rect r2 = {
        5 * BORDER_PADDING + BORDER_ICON_H,
        3 * BORDER_HEIGHT / 2 - BORDER_ICON_H / 2,
        BORDER_ICON_H,
        BORDER_ICON_H};
    SDL_RenderCopy(renderer, searchIcons.t2, NULL, &r2);

    SDL_Rect r3 = {
        8 * BORDER_PADDING + 2 * BORDER_ICON_H,
        3 * BORDER_HEIGHT / 2 - BORDER_ICON_H / 2,
        BORDER_ICON_H,
        BORDER_ICON_H};
    SDL_RenderCopy(renderer, searchIcons.t4, NULL, &r3);

    roundedBoxRGBA(renderer, 11 * BORDER_PADDING + 3 * BORDER_ICON_H, 3 * BORDER_HEIGHT / 2 - BORDER_ICON_H - 1, WINDOW_W - (4 * BORDER_PADDING + BORDER_ICON_H), 3 * BORDER_HEIGHT / 2 + BORDER_ICON_H + 1, BORDER_ICON_H, 40, 40, 40, 255);
    
    SDL_Rect r4 = {
        14 * BORDER_PADDING + 3 * BORDER_ICON_H,
        3 * BORDER_HEIGHT / 2 - BORDER_ICON_H / 2,
        BORDER_ICON_H,
        BORDER_ICON_H};

    // if(borderIcons.r5) filledCircleRGBA(renderer, r4.x + BORDER_ICON_H/2, BORDER_HEIGHT/2, BORDER_ICON_H/2 + 5, 71, 71, 71, 255);
    SDL_RenderCopy(renderer, borderIcons.t1, NULL, &r4);

    if (strcmp(searchBar.text, "") == 0)
    {
        SDL_Rect textR = {
            17 * BORDER_PADDING + 4 * BORDER_ICON_H,
            3 * BORDER_HEIGHT / 2 - searchBar.dth / 2,
            searchBar.dtw,
            searchBar.dth};
        SDL_RenderCopy(renderer, searchBar.t1, NULL, &textR);
    }
    else
    {
        SDL_Rect textR = {
            17 * BORDER_PADDING + 4 * BORDER_ICON_H,
            3 * BORDER_HEIGHT / 2 - searchBar.th / 2,
            searchBar.tw,
            searchBar.th};
        SDL_RenderCopy(renderer, searchBar.t2, NULL, &textR);
    }

    if (searchBar.r1)
    {
        SDL_Rect r = {
            17 * BORDER_PADDING + 4 * BORDER_ICON_H + searchBar.tw + 1,
            3 * BORDER_HEIGHT / 2 - searchBar.th / 2,
            1,
            searchBar.th};
        SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
        SDL_RenderFillRect(renderer, &r);
    }

    SDL_Rect r5 = {
        WINDOW_W - (7 * BORDER_PADDING + 2*BORDER_ICON_H),
        3 * BORDER_HEIGHT / 2 - BORDER_ICON_H / 2,
        BORDER_ICON_H,
        BORDER_ICON_H};
    SDL_RenderCopy(renderer, searchIcons.t5, NULL, &r5);

    SDL_Rect r6 = {
        WINDOW_W - BORDER_PADDING * 2 - BORDER_ICON_H,
        3 * BORDER_HEIGHT / 2 - BORDER_ICON_H / 2,
        BORDER_ICON_H,
        BORDER_ICON_H};
    if (borderIcons.r6)
        filledCircleRGBA(renderer, r6.x + BORDER_ICON_H / 2, BORDER_HEIGHT / 2, BORDER_ICON_H / 2 + 5, 71, 71, 71, 255);
    SDL_RenderCopy(renderer, searchIcons.t6, NULL, &r6);
}

void renderPage(Tab *tab)
{
    if (!tab || (tab && !tab->t1))
        return;
    // int w,h;
    // SDL_QueryTexture(tab->t1, NULL, NULL, &w, &h);
    // SDL_Rect r1 = {
    //     BORDER_PADDING*2,
    //     2*BORDER_HEIGHT + BORDER_PADDING*2,
    //     w,
    //     h
    // };

    // SDL_RenderCopy(renderer, tab->t1, NULL, &r1);
    if (!tab->DOM)
    {
        FILE *f = fopen(tab->src, "r");

        fseek(f, 0, SEEK_END);
        long file_size = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *file_content = malloc(file_size + 1);
        if (file_content)
        {
            fread(file_content, 1, file_size, f);
            file_content[file_size] = '\0';
        }

        // char* response = fetchURL("info.cern.ch");

        createDOM(file_content, &tab);
        fclose(f);
    }

    renderDOM(tab);
}