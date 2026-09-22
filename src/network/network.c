#include "network/network.h"
#include <curl/curl.h>

void networkInit(){
    curl_easy_init();
}


// CURL START

typedef struct
{
    char *data;
    size_t size;
} Response;

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t total = size * nmemb;
    Response *response = userp;

    char *new_data = realloc(response->data, response->size + total + 1);

    if (!new_data)
        return 0;

    response->data = new_data;

    memcpy(response->data + response->size, contents, total);

    response->size += total;
    response->data[response->size] = '\0';

    return total;
}

char *fetchSite(const char *url)
{
    CURL *curl = curl_easy_init();

    if (!curl)
        return NULL;

    Response response = {
        .data = NULL,
        .size = 0
    };

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    /* Follow redirects */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        free(response.data);
        return NULL;
    }

    return response.data;
}

// CURL END





char *getCodeFromResponse(char *response)
{
    char *html = strcasestr(response, "<html");
    
    return html;
}

char *fetchURL(char *url)
{
    char* query = "\0";
    int i = 0;
    while (url[i])
    {
        if(url[i] == '/'){
            query = url + i + 1;
            url[i] = '\0';
            break;
        }
        i++;
    }
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct hostent *host = gethostbyname(url);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(80);
    memcpy(&addr.sin_addr, host->h_addr_list[0], host->h_length);

    connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr));

    char request[1024];

    sprintf(request, "GET /%s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n", query, url);
        
    send(sock, request, strlen(request), 0);

    char buffer[1024];
    int size = 0;
    char *response = malloc(size + 1);
    response[0] = '\0';
    size_t n = 0;

    while ((n = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[n] = '\0';
        char *new = realloc(response, size + n + 1);
        response = new;

        strcat(response, buffer);
        size += n;
    }
    printf("%s\n", response);
    close(sock);
    return getCodeFromResponse(response);
}

void *fetchUrlAsync(void *arg)
{
    struct ThreadTabData *d = arg;

    FILE *f = fopen(d->tab->src, "r");

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *file_content = malloc(file_size + 1);
    if (file_content)
    {
        fread(file_content, 1, file_size, f);
        file_content[file_size] = '\0';
    }

    // char *response = fetchURL(d->url);
    // char *response = fetchSite(d->url);

    createDOM(file_content, &d->tab);
    d->tab->state = TAB_READY;
    fclose(f);
    return NULL;
}

char* getWebPage(char* url){
    int protocol = 0;
    if(!strncasecmp(url, "http://", 7)){
        protocol = 1;
    }
    if(!strncasecmp(url, "https://", 8)){
        protocol = 2;
    }
    if(!strncasecmp(url, "file://", 7)){
        protocol = 3;
    }
}