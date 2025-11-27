#ifndef RESPONSE
#define RESPONSE

#include "request.h"

typedef struct {
    char version[16];
    int status_code;
    char status_text[64];
    header_t headers[64];
    int header_count;
    char *body;
} response_t;

response_t create_response(int status_code, const char *status_text);

void add_header(response_t *response, const char *key, const char *value);

void send_response(int client_fd, response_t *response);

#endif