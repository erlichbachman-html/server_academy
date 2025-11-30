#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_socket,(struct sockaddr*)&addr,sizeof(addr));

    char method[16];
    char path[256];
    char body[2048] = "";

    printf("Метод: ");
    fgets(method, sizeof(method), stdin);
    method[strcspn(method, "\n")] = 0;

    printf("Путь: ");
    fgets(path, sizeof(path), stdin);
    path[strcspn(path, "\n")] = 0;

    printf("Тело(Если метод GET просто нажми ENTER): ");
    fgets(body, sizeof(body), stdin);
    body[strcspn(body, "\n")] = 0;

    if(strlen(body) > 0 && strcmp(method, "GET") == 0)
    {
        perror("При методе GET не принято отправлять BODY");
        close(client_socket);
        return 1;
    }

    char request[4096];
    int len = 0;
    len += sprintf(request + len, "%s %s CHLP/1.0\n", method, path);
    if(strlen(body) > 0)
    {
        char body_len[32];
        sprintf(body_len, "%lu", strlen(body));
        len += sprintf(request + len, "Body-Size: %s\n", body_len);
    }


    len += sprintf(request + len, "\n");
    if(strlen(body) > 0)
    {
        len += sprintf(request + len, "%s", body);
    }

    send(client_socket, request, len, 0);

    char buffer[8192];
    int bytes = recv(client_socket, buffer, 8191, 0);
    printf("Ответ сервера:\n%s\n", buffer);

    close(client_socket);
    return 0;
}