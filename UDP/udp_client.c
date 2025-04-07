#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 4000
#define MAX 1024

int main() {
    int sockfd, n;
    char buffer[MAX];
    struct sockaddr_in cli_addr;
    socklen_t addr_size;

    // 1. Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("UDP socket created. Client ready to receive.\n");

    // 2. Bind to local port
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(PORT);
    cli_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&cli_addr, sizeof(cli_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    addr_size = sizeof(cli_addr);
    printf("Client is listening on port %d...\n", PORT);

    while (1) {
        // 3. Receive from server
        n = recvfrom(sockfd, buffer, MAX, 0, (struct sockaddr*)&cli_addr, &addr_size);
        buffer[n] = '\0';
        printf("Received from server: %s\n", buffer);

        if (strncmp(buffer, "quit", 4) == 0)
            break;

        // 4. Reply to server
        printf("Enter reply to server: ");
        fgets(buffer, MAX, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&cli_addr, addr_size);
    }

    close(sockfd);
    printf("Client socket closed.\n");
    return 0;
}
