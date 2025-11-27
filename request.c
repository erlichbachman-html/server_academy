#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"

int find_header(request_t *req, const char *key)
{
    for(int i=0; i<req->header_count; ++i)
    {
        if(strcmp(req->headers[i].key,key)==0)
            return i;
    }
    return -1;
}

request_t* parse_request(char *buffer)
{
    request_t *req = malloc(sizeof(request_t));
    if(!req) return NULL;

    memset(req, 0, sizeof(request_t));

    char *first_line = strchr(buffer,'\n');
    if(!first_line) {
        free(req);
        return NULL;
    }

    *first_line = '\0';

    if(sscanf(buffer,"%15s %255s %15s", req->method, req->path, req->version) != 3)
    {
        free(req);
        return NULL;
    }

    char *headers = first_line + 1;

    while(1)
    {
        char *line_end = strchr(headers,'\n');
        if(!line_end) {
            req->body = NULL;
            return req;
        }

        if(line_end == headers)
        {
            ++headers;
            req->body = headers;
            break;
        }

        *line_end = '\0';

        char *row = strchr(headers,':');
        if(row)
        {
            *row = '\0';
            
            char *key = headers;
            char *value = row + 1;

            while (*value == ' ') value++;
            strncpy(req->headers[req->header_count].key, key, KEY_SIZE - 1);
            strncpy(req->headers[req->header_count].value, value, VALUE_SIZE - 1);

            
            req->header_count++;
        }

        headers = line_end + 1;
    }

    return req;
}

void free_request(request_t *req)
{
    if(req) free(req);
}