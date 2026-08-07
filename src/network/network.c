#include "network/network.h"


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
}