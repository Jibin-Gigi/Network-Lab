#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX 1024
#define PORT 8080

void chat(int new_sock_fd) {
    char buffer[MAX];
    int n;

    for (;;) {
        bzero(buffer, MAX);

        // Receive data from client
        recv(new_sock_fd, buffer, MAX, 0);
        printf("From client: %s\t To client: ", buffer);

        bzero(buffer, MAX);
        n = 0;
        while ((buffer[n++] = getchar()) != '\n');

        // Send message to client
        send(new_sock_fd, buffer, strlen(buffer), 0);

        // Exit condition
        if (strncmp("exit", buffer, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main() {
    int server_fd, new_sock_fd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t addr_size;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    printf("Socket successfully created..\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(serv_addr.sin_zero), 8);

    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        exit(0);
    }
    printf("Socket successfully binded..\n");

    if (listen(server_fd, 5) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    printf("Server listening..\n");

    addr_size = sizeof(client_addr);
    new_sock_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if (new_sock_fd < 0) {
        printf("Server accept failed...\n");
        exit(0);
    }
    printf("Server accepted the client...\n");

    // Handle chat
    chat(new_sock_fd);

    close(new_sock_fd);
    close(server_fd);

    return 0;
}
