#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "request.h"
#include "response.h"
#include "router.h"

void handle_index(int client_fd, request_t * request)
{
    FILE *file = fopen("index.html", "r");
    if (!file)
    {
        response_t r = create_response(500, "Internal Server Error");
        send_response(client_fd, &r);
        return;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *body = malloc(size + 1);
    fread(body, 1, size, file);
    fclose(file);

    body[size] = '\0';

    char body_size[32];

    sprintf(body_size, "%ld", size);

    response_t response = create_response(200, "OK");
    add_header(&response, "Body-Size", body_size);
    response.body = body;

    send_response(client_fd, &response);

    free(body);
}

void handle_echo(int client_fd, request_t *request)
{
    response_t response = create_response(200, "OK");

    if (request->body)
    {
        response.body = request->body;
    }
    else 
    {
        response.body = "";
    }
    for (int i = 0; i < request->header_count; ++i)
    {
        add_header(&response, request->headers[i].key, request->headers[i].value);
    }

    send_response(client_fd, &response);
}

void handle_post(int client_fd, request_t *request)
{

    FILE *file = fopen(request->path + 1, "a+");
    if (!file)
    {
        response_t response = create_response(500, "Internal Server Error");
        send_response(client_fd, &response);
        return;
    }
    else 
    {
        fseek(file, 0, SEEK_END);
        if (request->body)
        {
            fwrite(request->body, 1, strlen(request->body), file);
        }
        fclose(file);
        response_t response = create_response(200, "OK");
        send_response(client_fd, &response);
    }

}
App app; 

void* client_thread(void *arg)
{
    int client_fd = *(int*)arg;
    free(arg);

    char buffer[4096];
    int received = recv(client_fd, buffer, 4095, 0);
    if(received > 0)
    {

        request_t *req = parse_request(buffer);

        if(req)
        {
            time_t now = time(NULL);
            struct tm *t = localtime(&now);

            printf("[%02d:%02d:%02d] %s %s\n", t->tm_hour, t->tm_min, t->tm_sec, req->method, req->path);

            route_request(&app, client_fd, req);
            free_request(req);
        }
    }

    close(client_fd);
    return NULL;
}

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(sock,(struct sockaddr*)&addr,sizeof(addr));
    listen(sock, SOMAXCONN);
    printf("Сервер слушает на порте : 9090\n");

    app_init(&app);
    add_route(&app, "GET","/index.html", handle_index);
    add_route(&app, "ECHO","/echo", handle_echo);
    add_route(&app, "POST","/index.html", handle_post);
    

    while(1)
    {
        int *client_fd=malloc(sizeof(int));
        *client_fd = accept(sock, NULL, NULL);

        pthread_t tid;
        pthread_create(&tid, NULL, client_thread, client_fd);
        pthread_detach(tid);
    }
}