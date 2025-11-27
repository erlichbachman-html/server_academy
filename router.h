#ifndef ROUTER
#define ROUTER

#include "request.h"
#include "response.h"

typedef struct {
    char method[16];
    char path[256];
    void (*handler)(int client_fd, request_t *req);
} route_t;

typedef struct {
    route_t routes[64];
    int route_count;
} App;

void app_init(App *app);

void add_route(App *app, const char *method, const char *path, void (*handler)(int, request_t *));

void route_request(App *app, int client_fd, request_t *req);

#endif