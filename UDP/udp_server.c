#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 4000
#define MAX 1024

int main() {
    int sockfd, n, m;
    char buffer[MAX];
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_len;

    // 1. Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("UDP socket created. Waiting for clients...\n");

    // 2. Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 3. Bind the socket to IP and port
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }
    printf("Server bound to port %d\n", PORT);

    client_len = sizeof(client_addr);

    while (1) {
        // 4. Receive message from client
        bzero(buffer, MAX);
        m = recvfrom(sockfd, buffer, MAX, 0, (struct sockaddr*)&client_addr, &client_len);
        buffer[m] = '\0';
        printf("Received from client: %s\n", buffer);

        if (strncmp(buffer, "quit", 4) == 0)
            break;

        // 5. Send response to the client
        bzero(buffer, MAX);
        printf("Message to send: ");
        n = 0;
        while ((buffer[n++] = getchar()) != '\n');
        buffer[n - 1] = '\0';

        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, client_len);
    }

    close(sockfd);
    printf("Server socket closed.\n");
    return 0;
}

