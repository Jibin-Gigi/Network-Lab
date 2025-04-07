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
    struct sockaddr_in serv_addr;
    socklen_t addr_size;

    // 1. Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("UDP socket created. Server ready to send.\n");

    // 2. Configure destination (client) address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    addr_size = sizeof(serv_addr);

    while (1) {
        // 3. Clear buffer and get message to send
        bzero(buffer, MAX);
        printf("Message to send: ");
        n = 0;
        while ((buffer[n++] = getchar()) != '\n');
        buffer[n - 1] = '\0'; // Replace newline with null terminator

        // 4. Send message to client
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, addr_size);

        if (strncmp(buffer, "quit", 4) == 0)
            break;

        // 5. Receive response from client
        bzero(buffer, MAX);
        m = recvfrom(sockfd, buffer, MAX, 0, (struct sockaddr*)&serv_addr, &addr_size);
        buffer[m] = '\0';
        printf("Received from client: %s\n", buffer);
    }

    close(sockfd);
    printf("Server socket closed.\n");
    return 0;
}
