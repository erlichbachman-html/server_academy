#include <string.h>
#include "router.h"

void app_init(App *app)
{
    app->route_count = 0;
}

void add_route(App *app, const char *method, const char *path, void (*handler)(int, request_t *))
{
    strncpy(app->routes[app->route_count].method, method, 15);

    strncpy(app->routes[app->route_count].path, path, 255);

    app->routes[app->route_count].handler = handler;

    app->route_count++;
}

void route_request(App *app, int client_fd, request_t *req)
{
    for(int i = 0; i < app->route_count; i++)
    {
        if(strcmp(req->method, app->routes[i].method) == 0 && strcmp(req->path, app->routes[i].path) == 0)
        {
            app->routes[i].handler(client_fd, req);
            return;
        }
    }

    response_t r = create_response(404, "Not Found");
    
    send_response(client_fd, &r);
}