#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "response.h"

response_t create_response(int status_code, const char *status_text)
{
    response_t response;
    memset(&response,0,sizeof(response_t));
    strncpy(response.version, "CHLP/1.0", 15);
    response.status_code = status_code;
    strncpy(response.status_text, status_text, 63);

    return response;
}

void add_header(response_t *response, const char *key, const char *value)
{
    strncpy(response->headers[response->header_count].key, key, KEY_SIZE - 1);

    strncpy(response->headers[response->header_count].value, value, VALUE_SIZE - 1);

    response->header_count++;
}

void send_response(int client_fd, response_t *response)
{
    char buffer[8192];
    int len = 0;

    len += sprintf(buffer + len, "%s %d %s\n", response->version, response->status_code, response->status_text);

    for(int i=0;i<response->header_count;++i)
    {
        len += sprintf(buffer + len, "%s: %s\n", response->headers[i].key, response->headers[i].value);
    }

    len += sprintf(buffer + len, "\n");

    if(response->body)
    {
        int b = strlen(response->body);
        memcpy(buffer + len, response->body, b);
        len += b;
    }

    send(client_fd, buffer, len, 0);
}