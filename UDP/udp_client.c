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
    struct sockaddr_in serv_addr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("UDP socket created. Client ready to send and receive.\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP of the server

    addr_size = sizeof(serv_addr);

    while (1) {
        printf("Enter message to server: ");
        fgets(buffer, MAX, stdin);

        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, addr_size);

        if (strncmp(buffer, "quit", 4) == 0)
            break;

        bzero(buffer, MAX);
        n = recvfrom(sockfd, buffer, MAX, 0, NULL, NULL);
        buffer[n] = '\0';
        printf("Received from server: %s\n", buffer);
    }

    close(sockfd);
    printf("Client socket closed.\n");
    return 0;
}
