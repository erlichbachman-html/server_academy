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

    char request[] =
        "ECHO /echo CHLP/1.0\n"
        "Host: localhost\n"
        "User-Agent: CHLPAGENT\n"
        "\n"
        "Hello BROTHER\n";

    char post_request[] =
        "POST /index.html CHLP/1.0\n"
        "Host: localhost\n"
        "User-Agent: MyClient\n"
        "Content-Length: 13\n"
        "\n"
        "CHLP message!";
    char get_request[] =
        "GET /index.html CHLP/1.0\n"
        "User-Agent: Who ARE YOU\n"
        "\n";

    send(client_socket,request,strlen(request),0);
    char buffer[8192];
    int received = recv(client_socket,buffer,8191,0);
    printf("Response:\n%s\n",buffer);

    close(client_socket);
    return 0;
}