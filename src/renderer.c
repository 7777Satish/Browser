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
    .src7 = "assets/icons/search.png",
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

ImageElms settingsIcons = {
    .src1 = "assets/icons/add.png",
    .src2 = "assets/icons/history.png",
    .src3 = "assets/icons/github.png",
    .src4 = "assets/icons/debug.png",
    .src5 = "assets/icons/help.png",
    .src6 = "assets/icons/settings2.png",
    .src7 = "assets/icons/exit.png",
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

ImageElms settingsText = {
    .src1 = "New Tab",
    .src2 = "History",
    .src3 = "Github",
    .src4 = "Debug",
    .src5 = "Help",
    .src6 = "Settings",
    .src7 = "Exit",
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

SearchBar searchBar = {
    .defaultText = "Search the web",
    .text = "",
    .r1 = 0,
    .t2 = NULL};

SDL_Color MAIN_COLOR = {35, 35, 35, 255};
SDL_Color ACCENT_COLOR = {71, 71, 71, 255};

int SHOW_SEARCH_SUGGESTION = 0;
ImageElms searchSuggestions = {};

int history_size = 300;
const char *history[] = {
    "google.com",
    "how to center a div",
    "github.com",
    "css flexbox tutorial",
    "stackoverflow.com",
    "c array of strings syntax",
    "leetcode.com",
    "two sum problem solution python",
    "wikipedia.org",
    "c++ vector vs array memory",
    "reddit.com",
    "best mechanical keyboard switches",
    "youtube.com",
    "lofi hip hop radio live",
    "amazon.com",
    "ergonomic office chair reviews",
    "developer.mozilla.org",
    "mdn array prototype map",
    "chatgpt.com",
    "explain quantum computing to a child",
    "w3schools.com",
    "sql inner join vs left join",
    "linkedin.com",
    "software engineer jobs remote",
    "://ycombinator.com",
    "how to exit vim terminal",
    "twitter.com",
    "git undo last commit reset soft",
    "nytimes.com",
    "world news today headlines",
    "twitch.tv",
    "speedrun world record mario 64",
    "spotify.com",
    "discover weekly playlist",
    "bbc.com",
    "weather forecast London weekend",
    "imdb.com",
    "best sci fi movies on netflix",
    "pinterest.com",
    "minimalist home office setup ideas",
    "ebay.com",
    "vintage leather jacket men",
    "medium.com",
    "clean architecture introduction guide",
    "quora.com",
    "why does c malloc return void pointer",
    "geeksforgeeks.org",
    "binary search tree implementation in c",
    "coursera.org",
    "machine learning basics specialization",
    "udemy.com",
    "react crash course for beginners",
    "digitalocean.com",
    "how to configure nginx reverse proxy",
    "behance.net",
    "web design trends portfolios",
    "dribbble.com",
    "mobile app ui landing page design",
    "flickr.com",
    "street photography tips focal length",
    "imgur.com",
    "funny cat memes compilation",
    "giphy.com",
    "thumbs up high quality gif",
    "vimeo.com",
    "indie short films cinematic lighting",
    "dailymotion.com",
    "highlights of champions league match",
    "soundcloud.com",
    "synthwave cyberpunk music mix",
    "bandcamp.com",
    "independent chiptune artist albums",
    "patreon.com",
    "exclusive digital art tutorial videos",
    "kickstarter.com",
    "smart home gadgets crowdfunding campaigns",
    "indiegogo.com",
    "innovative travel backpacks pre order",
    "producthunt.com",
    "best daily productivity tool mac",
    "slack.com",
    "how to set custom status slack",
    "discord.com",
    "join public tech developer servers",
    "whatsapp.com",
    "download whatsapp web application macos",
    "telegram.org",
    "bot api documentation telegram web",
    "signal.org",
    "is signal completely open source",
    "skype.com",
    "how to screen share skype mobile",
    "viber.com",
    "activate international calling plans viber",
    "wechat.com",
    "mini programs development setup guide",
    "tiktok.com",
    "trending audio tracks dance challenge",
    "deviantart.com",
    "fantasy landscape concept art tutorial",
    "microsoft.com",
    "download vscode for windows 11",
    "apple.com",
    "m3 macbook air specs benchmarks",
    "docker.com",
    "docker compose vs dockerfile difference",
    "kubernetes.io",
    "k8s pod lifecycle explained",
    "gitlab.com",
    "setup self hosted gitlab runner",
    "bitbucket.org",
    "git cherry pick multiple commits",
    "://amazon.com",
    "ec2 free tier limitations limit",
    "://google.com",
    "enable compute engine api gcp",
    "://azure.com",
    "azure functions blob trigger example",
    "heroku.com",
    "free alternatives to heroku hosting",
    "vercel.com",
    "nextjs static site generation build",
    "netlify.com",
    "deploy react app custom domain",
    "cloudflare.com",
    "dns propagation checker online tool",
    "godaddy.com",
    "transfer domain to cloudflare registrar",
    "namecheap.com",
    "cheap ssl certificate for apache",
    "notion.so",
    "best productivity templates for students",
    "trello.com",
    "kanban board template software development",
    "://atlassian.com",
    "how to create epic jira",
    "asana.com",
    "asana vs trello project management",
    "zoom.us",
    "test microphone online zoom audio",
    "://google.com",
    "share tab audio google meet",
    "://microsoft.com",
    "clear teams cache windows 10",
    "figma.com",
    "auto layout wrap feature figma",
    "canva.com",
    "remove image background free canva",
    "adobe.com",
    "photoshop generative fill tutorial tools",
    "://adobe.com",
    "cancel adobe subscription without fee",
    "hulu.com",
    "best horror movies hulu 2024",
    "disneyplus.com",
    "star wars chronological watch order",
    "max.com",
    "house of the dragon stream",
    "paramountplus.com",
    "live champions league stream free",
    "crunchyroll.com",
    "demon slayer latest episode watch",
    "fandom.com",
    "elden ring ranni quest guide",
    "ign.com",
    "gta 6 release date leaks",
    "gamespot.com",
    "best rpg games pc 2023",
    "pcgamer.com",
    "rtx 4070 ti vs 3080",
    "kotaku.com",
    "nintendo switch 2 hardware rumors",
    "polygon.com",
    "indie games of the year",
    "epicgames.com",
    "free mystery game this week",
    "steampowered.com",
    "steam deck OLED battery life",
    "gog.com",
    "classic pc games drm free",
    "nexusmods.com",
    "cyberpunk 2077 performance mods install",
    "speedrun.com",
    "minecraft rsg glitches allowed rules",
    "chess.com",
    "how to counter queen gambit",
    "lichess.org",
    "puzzle streak tactic training free",
    "wolframalpha.com",
    "integrate x squared ln x",
    "khanacademy.org",
    "linear algebra matrix multiplication steps",
    "coursera.org",
    "andrew ng deep learning specialization",
    "edx.org",
    "mit introduction to computer science",
    "udemy.com",
    "ultimate docker masterclass coupon code",
    "pluralsight.com",
    "clean code principles c# course",
    "frontendmasters.com",
    "advanced javascript hard parts tutorial",
    "freecodecamp.org",
    "responsive web design certification project",
    "theodinproject.com",
    "full stack javascript curriculum review",
    "scrimba.com",
    "learn react interactively free course",
    "sololearn.com",
    "python for beginners certificate test",
    "hackerrank.com",
    "sql basic join interview questions",
    "codewars.com",
    "kyu 4 kata javascript solutions",
    "codechef.com",
    "long challenge division 3 problems",
    "topcoder.com",
    "single round match competitive programming",
    "kaggle.com",
    "titanic dataset machine learning tutorial",
    "huggingface.co",
    "fine tune llama 3 model",
    "arxiv.org",
    "attention is all you need",
    "://google.com",
    "image segmentation neural networks citation",
    "researchgate.net",
    "download full text pdf free",
    "sciencedirect.com",
    "graphene properties thermal conductivity review",
    "nature.com",
    "superconductivity room temperature breakthrough paper",
    "space.com",
    "james webb telescope latest images",
    "nasa.gov",
    "artemis moon mission launch timeline",
    "esa.int",
    "ariane 6 rocket engine specs",
    "spacex.com",
    "starship flight test video stream",
    "blueorigin.com",
    "new glenn rocket payload capacity",
    "techcrunch.com",
    "ai startup funding rounds silicon",
    "theverge.com",
    "apple vision pro review usability",
    "wired.com",
    "the future of generative ai",
    "engadget.com",
    "best wireless earbuds under 100",
    "gizmodo.com",
    "smart home privacy concerns devices",
    "mashable.com",
    "viral trends tiktok this week",
    "vice.com",
    "underground electronic music scene dokumentary",
    "forbes.com",
    "billionaires list real time net",
    "bloomberg.com",
    "fed interest rate hike decision",
    "reuters.com",
    "global inflation rate charts 2024",
    "wsj.com",
    "tech stocks market correction analysis",
    "ft.com",
    "european central bank monetary policy",
    "cnbc.com",
    "bitcoin price analysis support levels",
    "marketwatch.com",
    "s&p 500 historical returns data",
    "yahoo.com",
    "finance market trends stock tickers",
    "fool.com",
    "top dividend stocks to buy",
    "investopedia.com",
    "options trading call vs put",
    "coinmarketcap.com",
    "ethereum layer 2 scaling tokens",
    "coindesk.com",
    "crypto regulation bill updates house",
    "etherscan.io",
    "check gas fee price gwei",
    "bscscan.com",
    "verify smart contract code address",
    "tradingview.com",
    "macd indicator settings day trading",
    "binance.com",
    "withdraw crypto to hardware wallet",
    "coinbase.com",
    "tax document forms crypto gains",
    "kraken.com",
    "margin trading leverage limits usa",
    "metamask.io",
    "connect metamask to arbitrum network",
    "opensea.io",
    "floor price pixelmon nft ranking",
    "discord.com",
    "nft project alpha leak channels",
    "medium.com",
    "solana development tutorial anchor framework",
    "rust-lang.org",
    "rust ownership memory management rules",
    "golang.org",
    "go channels concurrency patterns example",
    "python.org",
    "asyncio gather vs wait exceptions",
    "nodejs.org",
    "event loop blocked troubleshooting performance",
    "npmjs.com",
    "package json dependency version caret",
    "pypi.org",
    "install pandas package virtual environment",
    "rubygems.org",
    "bundle install error dynamic libraries",
    "packagist.org",
    "composer require phpmailer script tutorial",
    "crates.io",
    "serde json serialize struct rust",
    "maven.apache.org",
    "pom xml dependency exclusion spring",
    "gradle.org",
    "build gradle dynamic versions block",
    "spring.io",
    "spring boot security config oauth2",
    "laravel.com",
    "laravel middleware routes group protect",
    "django-project.com",
    "django custom user model abstractuser", "rubyonrails.org", "rails active record polymorphic association", "expressjs.com", "express error handling middleware template", "tiangolo.com", "fastapi background tasks dependency injection", "nextjs.org", "nextjs server actions form validation", "remix.run", "remix loader function error boundary", "nuxt.com", "nuxt async data fetch pattern", "vuejs.org", "vue 3 composition api watch", "angular.io", "angular signals state management tutorial", "svelte.dev", "svelte kit dynamic routing parameters", "react.dev", "useeffect cleanup function execution order", "reactnative.dev", "flatlist performance optimization removeclipping", "flutter.dev", "flutter statefulwidget lifecycle stages diagram", "dart.dev", "dart null safety sound migration", "kotlinlang.org", "kotlin coroutines flow flatmaplatest operators", "android.com", "jetpack compose lazycolumn sticky headers", "apple.com", "swiftui nsvigationstack deep linking manual", "typescriptlang.org", "typescript conditional types infer keyword", "eslint.org", "configure eslint prettier conflict fix", "babeljs.io", "babel optional chaining plugin configuration", "webpack.js.org", "code splitting dynamic imports webpack", "vite.dev", "vite env variables prefix import", "tailwindcss.com", "arbitrary variants tailwind responsive screen", "getbootstrap.com", "bootstrap 5 grid offset utilities", "sass-lang.com", "sass mixin forward include directives", "postgresql.org", "postgresql optimize slow query indexing", "mysql.com", "mysql upsert syntax on duplicate", "mongodb.com", "mongodb aggregation framework match group", "redis.io", "redis cache eviction policies maxmemory", "elastic.co", "elasticsearch fuzzy search match query", "sqlite.org", "sqlite foreign key support enabling", "cassandra.apache.org", "cassandra primary key partitioning clustering", "neo4j.com", "cypher query language shortest path", "influxdata.com", "influxdb retention policy continuous query", "prisma.io", "prisma raw query sql parameterization", "sequelize.org", "sequelize transactions managed rollback automatic", "mongoosejs.com", "mongoose populate nested reference fields", "typeorm.io", "typeorm migration generate datasource file", "supabase.com", "supabase row level security policies", "google.com", "firestore rules security resource data", "auth0.com", "jwt validation verify signature rs256", "jwt.io", "decode jwt payload token online", "postman.com", "postman environment variables collection runner", "swagger.io", "openapi spec 3 bearer token", "graphql.org", "graphql dataloader pattern n+1 problem", "apollo-server", "apollo server error handling context", "hasura.io", "hasura custom business logic actions", "grafana.com", "prometheus dashboard setup alerts rule", "datadoghq.com", "datadog apm trace id correlation", "newrelic.com", "new relic infrastructure agent install", "sentry.io", "sentry source maps upload webpack", "logrocket.com", "frontend session replay error tracking", "stackoverflow.com", "git stash pop conflict resolution", "serverfault.com", "nginx return 502 bad gateway", "superuser.com", "chmod 755 vs 644 difference", "askubuntu.com", "apt get upgrade vs update", "reddit.com", "r/selfhosted home lab gear dashboard", "ycombinator.com", "show hn lightweight database wrapper", "lobste.rs", "functional programming compiler optimization techniques", "dev.to", "tips for junior developers onboarding", "hashnode.dev", "blogging for software engineers tips", "medium.com", "system design interview distributed rate", "bytebytego.com", "consistent hashing algorithm distributed systems", "lethain.com", "staff engineer archetypes book review", "martinfowler.com", "microservices architecture pattern pros cons", "refactoring.guru", "strategy pattern class diagram example", "sourcemaking.com", "anti patterns software engineering projects", "12factor.net", "twelve factor app config environment", "semver.org", "semantic versioning minor vs patch", "regex101.com", "regex non capturing group match", "jsonlint.com", "validate format json online text", "jwt.io", "verify jwt token signatures online", "crontab.guru", "cron expression every 5 minutes", "explainshell.com", "tar xvzf command line arguments", "ssllabs.com", "ssl server test security grade", "caniuse.com", "css subgrid browser support metrics", "bundlephobia.com", "npm package bundle size analyzer", "packagephobia.com", "npm package installation size footprint", "hoppscotch.io", "open source lightweight postman alternative", "insomnia.rest", "insomnia graphql client request body", "cyberchef", "base64 decode convert hex utility", "json2ts.com", "generate typescript interfaces from json", "quicktype.io", "convert json schema to rust", "carbon.now.sh", "beautiful code snippets images generator", "excalidraw.com", "hand drawn diagram tool collaboration", "draw.io", "architecture diagram cloud infrastructure template", "mermaid.js.org", "mermaid sequence diagram syntax flowchart", "plantuml.com", "component diagram text generation tools", "overleaf.com", "latex resume template clean formatting", "ctan.org", "latex packages font encodings fonts", "languagetool.org", "open source grammar checker api", "deepl.com", "best translator precision technical texts", "wikipedia.org", "turing machine computational complexity definition", "archive.org", "wayback machine vintage webpage snapshots", "gutenberg.org", "download free classic ebooks epub", "librivox.org", "free audiobooks public domain recordings", "duolingo.com", "learn spanish vocab daily streak", "memrise.com", "spaced repetition system vocabulary trainer", "ankiweb.net", "shared flashcard decks medical board", "goodreads.com", "best sci fi novels list", "librarything.com", "catalog personal home book collection", "storygraph.com", "personalized reading analytics tracker books", "letterboxd.com", "movie review ranking tracking diary", "trakt.tv", "track tv show watch history", "myanimelist.net", "top anime list rank season", "anilist.co", "modern anime tracker planning list"};

void renderSearchSuggestion();
void *fetchUrlAsync(void *arg);

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

    SDL_Surface *s7 = IMG_Load(elms->src7);
    elms->t7 = SDL_CreateTextureFromSurface(renderer, s7);
    SDL_FreeSurface(s7);

    SDL_Surface *s8 = IMG_Load(elms->src8);
    elms->t8 = SDL_CreateTextureFromSurface(renderer, s8);
    SDL_FreeSurface(s8);

    SDL_Surface *s9 = IMG_Load(elms->src9);
    elms->t9 = SDL_CreateTextureFromSurface(renderer, s9);
    SDL_FreeSurface(s9);

    SDL_Surface *s10 = IMG_Load(elms->src10);
    elms->t10 = SDL_CreateTextureFromSurface(renderer, s10);
    SDL_FreeSurface(s10);
}

void initTextElms(ImageElms *elms, TTF_Font *font, SDL_Color fg)
{
    SDL_Surface *s1 = TTF_RenderText_Blended(font, elms->src1, fg);
    elms->t1 = SDL_CreateTextureFromSurface(renderer, s1);
    SDL_FreeSurface(s1);

    SDL_Surface *s2 = TTF_RenderText_Blended(font, elms->src2, fg);
    elms->t2 = SDL_CreateTextureFromSurface(renderer, s2);
    SDL_FreeSurface(s2);

    SDL_Surface *s3 = TTF_RenderText_Blended(font, elms->src3, fg);
    elms->t3 = SDL_CreateTextureFromSurface(renderer, s3);
    SDL_FreeSurface(s3);

    SDL_Surface *s4 = TTF_RenderText_Blended(font, elms->src4, fg);
    elms->t4 = SDL_CreateTextureFromSurface(renderer, s4);
    SDL_FreeSurface(s4);

    SDL_Surface *s5 = TTF_RenderText_Blended(font, elms->src5, fg);
    elms->t5 = SDL_CreateTextureFromSurface(renderer, s5);
    SDL_FreeSurface(s5);

    SDL_Surface *s6 = TTF_RenderText_Blended(font, elms->src6, fg);
    elms->t6 = SDL_CreateTextureFromSurface(renderer, s6);
    SDL_FreeSurface(s6);

    SDL_Surface *s7 = TTF_RenderText_Blended(font, elms->src7, fg);
    elms->t7 = SDL_CreateTextureFromSurface(renderer, s7);
    SDL_FreeSurface(s7);

    SDL_Surface *s8 = TTF_RenderText_Blended(font, elms->src8, fg);
    elms->t8 = SDL_CreateTextureFromSurface(renderer, s8);
    SDL_FreeSurface(s8);

    SDL_Surface *s9 = TTF_RenderText_Blended(font, elms->src9, fg);
    elms->t9 = SDL_CreateTextureFromSurface(renderer, s9);
    SDL_FreeSurface(s9);

    SDL_Surface *s10 = TTF_RenderText_Blended(font, elms->src10, fg);
    elms->t10 = SDL_CreateTextureFromSurface(renderer, s10);
    SDL_FreeSurface(s10);
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

        if (temp->s1)
        {
            temp->t1 = SDL_CreateTextureFromSurface(renderer, temp->s1);
            SDL_FreeSurface(temp->s1);
            temp->s1 = NULL;
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
        WINDOW_W - (7 * BORDER_PADDING + 2 * BORDER_ICON_H),
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

    if (SHOW_SEARCH_SUGGESTION && searchBar.r1 && searchBar.text[0])
    {
        renderSearchSuggestion();
    }
}

void renderSearchSuggestion()
{
    if (!searchBar.r1)
        return;
    if (!searchBar.text)
        return;

    // if (!searchSuggestions.r1)
    // {
    int i = 0;
    int j = 0;
    char *filtered[6] = {0};
    while (i < history_size && history[i] && j < 6)
    {
        if (strncasecmp(searchBar.text, history[i], strlen(searchBar.text)) == 0)
        {
            filtered[j] = history[i];
            j++;
        }
        i++;
    }

    int suggestionHeight = BORDER_HEIGHT - 4;
    SDL_Rect suggestionRect = {
        8 * BORDER_PADDING + BORDER_ICON_H + 3 * BORDER_ICON_H,
        2 * BORDER_HEIGHT,
        WINDOW_W - (4 * BORDER_PADDING + BORDER_ICON_H),
        suggestionHeight * j};

    suggestionRect.w -= suggestionRect.x;

    SDL_SetRenderDrawColor(renderer, MAIN_COLOR.r, MAIN_COLOR.g, MAIN_COLOR.b, 255);
    SDL_RenderFillRect(renderer, &suggestionRect);

    SDL_SetRenderDrawColor(renderer, ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, ACCENT_COLOR.a);
    SDL_RenderDrawRect(renderer, &suggestionRect);

    SDL_Rect r = {
        suggestionRect.x + BORDER_ICON_H + BORDER_PADDING * 6,
        suggestionRect.y + suggestionHeight / 2,
        0,
        0};

    SDL_Rect icon_r = {
        suggestionRect.x + BORDER_PADDING * 3,
        suggestionRect.y + suggestionHeight / 2 - BORDER_ICON_H / 2,
        BORDER_ICON_H,
        BORDER_ICON_H};

    if (filtered[0])
    {
        SDL_Surface *s = TTF_RenderText_Blended(poppins_regular, filtered[0], tab_fg);
        searchSuggestions.t1 = SDL_CreateTextureFromSurface(renderer, s);
        searchSuggestions.r1 = 0;
        strcpy(searchSuggestions.src1, filtered[0]);

        r.y -= s->h / 2;
        r.w = s->w;
        r.h = s->h;

        SDL_RenderCopy(renderer, searchIcons.t7, NULL, &icon_r);
        SDL_RenderCopy(renderer, searchSuggestions.t1, NULL, &r);

        SDL_FreeSurface(s);
    }

    if (filtered[1])
    {
        SDL_Surface *s = TTF_RenderText_Blended(poppins_regular, filtered[1], tab_fg);
        searchSuggestions.t2 = SDL_CreateTextureFromSurface(renderer, s);
        searchSuggestions.r2 = 1;
        strcpy(searchSuggestions.src2, filtered[1]);

        r.y += suggestionHeight;
        r.w = s->w;
        icon_r.y += suggestionHeight;
        SDL_RenderCopy(renderer, searchIcons.t7, NULL, &icon_r);
        SDL_RenderCopy(renderer, searchSuggestions.t2, NULL, &r);

        SDL_FreeSurface(s);
    }

    if (filtered[2])
    {
        SDL_Surface *s = TTF_RenderText_Blended(poppins_regular, filtered[2], tab_fg);
        searchSuggestions.t3 = SDL_CreateTextureFromSurface(renderer, s);
        searchSuggestions.r3 = 1;
        strcpy(searchSuggestions.src3, filtered[2]);

        r.y += suggestionHeight;
        r.w = s->w;
        icon_r.y += suggestionHeight;
        SDL_RenderCopy(renderer, searchIcons.t7, NULL, &icon_r);
        SDL_RenderCopy(renderer, searchSuggestions.t3, NULL, &r);

        SDL_FreeSurface(s);
    }

    if (filtered[3])
    {
        SDL_Surface *s = TTF_RenderText_Blended(poppins_regular, filtered[3], tab_fg);
        searchSuggestions.t4 = SDL_CreateTextureFromSurface(renderer, s);
        searchSuggestions.r4 = 1;
        strcpy(searchSuggestions.src4, filtered[3]);

        r.y += suggestionHeight;
        r.w = s->w;

        icon_r.y += suggestionHeight;
        SDL_RenderCopy(renderer, searchIcons.t7, NULL, &icon_r);
        SDL_RenderCopy(renderer, searchSuggestions.t4, NULL, &r);

        SDL_FreeSurface(s);
    }

    if (filtered[4])
    {
        SDL_Surface *s = TTF_RenderText_Blended(poppins_regular, filtered[4], tab_fg);
        searchSuggestions.t5 = SDL_CreateTextureFromSurface(renderer, s);
        searchSuggestions.r5 = 1;
        strcpy(searchSuggestions.src5, filtered[4]);

        r.y += suggestionHeight;
        r.w = s->w;

        icon_r.y += suggestionHeight;
        SDL_RenderCopy(renderer, searchIcons.t7, NULL, &icon_r);
        SDL_RenderCopy(renderer, searchSuggestions.t5, NULL, &r);

        SDL_FreeSurface(s);
    }

    if (filtered[5])
    {
        SDL_Surface *s = TTF_RenderText_Blended(poppins_regular, filtered[5], tab_fg);
        searchSuggestions.t6 = SDL_CreateTextureFromSurface(renderer, s);
        searchSuggestions.r6 = 1;
        strcpy(searchSuggestions.src6, filtered[5]);

        r.y += suggestionHeight;
        r.w = s->w;

        icon_r.y += suggestionHeight;
        SDL_RenderCopy(renderer, searchIcons.t7, NULL, &icon_r);
        SDL_RenderCopy(renderer, searchSuggestions.t6, NULL, &r);

        SDL_FreeSurface(s);
    }
    // }

    // if (searchSuggestions.t1)
    // {
    //     int w, h;
    //     SDL_QueryTexture(searchSuggestions.t1, NULL, NULL, &w, &h);
    //     SDL_Rect r1 = {
    //         suggestionRect.x + BORDER_ICON_H + BORDER_PADDING * 6,
    //         suggestionRect.y + suggestionHeight / 2 - h / 2,
    //         w,
    //         h};
    //     SDL_RenderCopy(renderer, searchSuggestions.t1, NULL, &r1);
    // }
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
    if (tab->state == TAB_UNINITIALIZED)
    {

        pthread_t t;
        struct ThreadTabData *data = (struct ThreadTabData *)malloc(sizeof(struct ThreadTabData));
        data->tab = tab;
        data->url = SDL_strdup("www.bing.com");

        if (pthread_create(&t, NULL, fetchUrlAsync, data) == 0)
        {
            pthread_detach(t);
            tab->state = TAB_LOADING;
        }
        else
        {
            free(data);
        }
    }

    if (tab->state == TAB_READY)
        renderDOM(tab);
}

void renderSetting()
{

    int w, h;
    SDL_QueryTexture(settingsText.t1, NULL, NULL, &w, &h);

    int height = h + 4 * BORDER_PADDING;
    int settingWidth = 300;
    int settingHeight = height;
    SDL_Rect r = {
        WINDOW_W - settingWidth,
        BORDER_HEIGHT * 2,
        settingWidth,
        7 * height};

    SDL_SetRenderDrawColor(renderer, MAIN_COLOR.r, MAIN_COLOR.g, MAIN_COLOR.b, MAIN_COLOR.a);
    SDL_RenderFillRect(renderer, &r);

    SDL_SetRenderDrawColor(renderer, ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, ACCENT_COLOR.a);
    SDL_RenderDrawRect(renderer, &r);

    if (!settingsIcons.t1)
        initImageElms(&settingsIcons);
    if (!settingsText.t1)
        initTextElms(&settingsText, poppins_regular, (SDL_Color){230, 230, 230});

    int iw, ih;
    SDL_QueryTexture(settingsIcons.t2, NULL, NULL, &iw, &ih);

    iw = ((iw + 0.0) / ih) * (h - 1.5 * BORDER_PADDING);
    ih = h - 1.5 * BORDER_PADDING;

    SDL_Rect ir = {
        r.x + BORDER_PADDING * 2,
        r.y + height / 2 - ih / 2,
        iw,
        ih};

    SDL_Rect tr = {
        r.x + 2 * 2 * BORDER_PADDING + iw,
        r.y + height / 2 - h / 2,
        w,
        h};

    SDL_RenderCopy(renderer, settingsIcons.t1, NULL, &ir);
    SDL_RenderCopy(renderer, settingsText.t1, NULL, &tr);

    SDL_QueryTexture(settingsText.t2, NULL, NULL, &w, &h);

    ir.y += height;
    tr.w = w;
    tr.y += height;

    SDL_RenderCopy(renderer, settingsIcons.t2, NULL, &ir);
    SDL_RenderCopy(renderer, settingsText.t2, NULL, &tr);

    SDL_QueryTexture(settingsText.t3, NULL, NULL, &w, &h);

    ir.y += height;
    tr.w = w;
    tr.y += height;

    SDL_RenderCopy(renderer, settingsIcons.t3, NULL, &ir);
    SDL_RenderCopy(renderer, settingsText.t3, NULL, &tr);

    SDL_QueryTexture(settingsText.t4, NULL, NULL, &w, &h);

    ir.y += height;
    tr.w = w;
    tr.y += height;

    SDL_RenderCopy(renderer, settingsIcons.t4, NULL, &ir);
    SDL_RenderCopy(renderer, settingsText.t4, NULL, &tr);

    SDL_QueryTexture(settingsText.t5, NULL, NULL, &w, &h);

    ir.y += height;
    tr.w = w;
    tr.y += height;

    SDL_RenderCopy(renderer, settingsIcons.t5, NULL, &ir);
    SDL_RenderCopy(renderer, settingsText.t5, NULL, &tr);

    SDL_QueryTexture(settingsText.t6, NULL, NULL, &w, &h);

    ir.y += height;
    tr.w = w;
    tr.y += height;

    SDL_RenderCopy(renderer, settingsIcons.t6, NULL, &ir);
    SDL_RenderCopy(renderer, settingsText.t6, NULL, &tr);

    SDL_QueryTexture(settingsText.t7, NULL, NULL, &w, &h);

    ir.y += height;
    tr.w = w;
    tr.y += height;

    SDL_RenderCopy(renderer, settingsIcons.t7, NULL, &ir);
    SDL_RenderCopy(renderer, settingsText.t7, NULL, &tr);
}