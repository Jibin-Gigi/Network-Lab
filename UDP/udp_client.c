#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER_PORT 4000
#define MAX 1024

int main() {
    int sockfd, n;
    char buffer[MAX];
    struct sockaddr_in server_addr;
    socklen_t server_len;

    // 1. Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("UDP socket created. Client ready to send messages.\n");

    // 2. Set up server address
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    
    server_len = sizeof(server_addr);
    printf("Client ready to communicate with server on port %d...\n", SERVER_PORT);

    while (1) {
        // 3. Send message to server
        printf("Enter message to server: ");
        bzero(buffer, MAX);
        fgets(buffer, MAX, stdin);
        
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, server_len);
        
        if (strncmp(buffer, "quit", 4) == 0)
            break;

        // 4. Receive reply from server
        bzero(buffer, MAX);
        n = recvfrom(sockfd, buffer, MAX, 0, (struct sockaddr*)&server_addr, &server_len);
        buffer[n] = '\0';
        printf("Received from server: %s\n", buffer);
    }

    close(sockfd);
    printf("Client socket closed.\n");
    return 0;
}
