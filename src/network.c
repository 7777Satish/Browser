#include "network.h"

char *getCodeFromResponse(char *response);

char *fetchURL(char *url)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct hostent *host = gethostbyname(url);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(80);
    memcpy(&addr.sin_addr, host->h_addr_list[0], host->h_length);

    connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr));

    char request[1024];

    sprintf(request, "GET / HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n", url);
        
    send(sock, request, strlen(request), 0);

    char buffer[1024];
    int size = 0;
    char *response = malloc(size + 1);
    response[0] = '\0';
    size_t n = 0;

    while ((n = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[n] = '\0';
        // printf("%s", buffer);

        char *new = realloc(response, size + n);
        response = new;

        strcat(response, buffer);
        size += n;
    }

    close(sock);
    return getCodeFromResponse(response);
}

char *getCodeFromResponse(char *response)
{
    char *html = strcasestr(response, "<html");
    
    return html;
}