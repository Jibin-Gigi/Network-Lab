#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 1024
#define PORT 8080

int main() {
    int client;
    char buffer[MAX];
    struct sockaddr_in serv_addr;

    // Create socket
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created..\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    // Connect to server
    if (connect(client, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) {
        perror("Connection with the server failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server..\n");

    for (;;) {
        bzero(buffer, sizeof(buffer));
        printf("Enter the string: ");

        int n = 0;
        while ((buffer[n++] = getchar()) != '\n');
        //buffer[n - 1] = '\0'; // Replace '\n' with null terminator

        // Send to server
        send(client, buffer, strlen(buffer), 0);

        // Clear buffer and receive from server
        bzero(buffer, sizeof(buffer));
        recv(client, buffer, sizeof(buffer), 0);

        printf("From Server: %s\n", buffer);

        // Exit condition
        if (strncmp("exit", buffer, 4) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }

    // Close socket
    close(client);
    return 0;
}
