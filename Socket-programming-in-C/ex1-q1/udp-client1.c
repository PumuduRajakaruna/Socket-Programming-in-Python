#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Include for close function

#define MAX_BUFFER_SIZE 100

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <IP address>\n", argv[0]);
        return -1;
    }

    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t len;
    char buffer[MAX_BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(32000);

    printf("Client is running.\n");

    while (1) {
        len = sizeof(servaddr);

        // Receive the current time from the server
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&servaddr, &len);

        // Display the received time
        buffer[n] = '\0';
        printf("Received Time: %s\n", buffer);
    }

    close(sockfd);

    return 0;
}
