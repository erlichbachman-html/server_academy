#ifndef REQUEST
#define REQUEST

#define KEY_SIZE 64
#define VALUE_SIZE 256

typedef struct {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
} header_t;

typedef struct {
    char method[16];
    char path[256];
    char version[16];
    header_t headers[64];
    int header_count;
    char *body;
} request_t;

request_t* parse_request(char *buffer);

int find_header(request_t *req, const char *key);

void free_request(request_t *req);

#endif