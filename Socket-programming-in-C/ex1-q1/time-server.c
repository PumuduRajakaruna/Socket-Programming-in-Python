#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 100

int sockfd;

void sigint_handler(int signo) {
    printf("Terminating the server...\n");
    close(sockfd);
    exit(0);
}

int main() {
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buffer[MAX_BUFFER_SIZE];
    time_t rawtime;
    struct tm *info;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(32000);
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Time server is running. Press Ctrl+C to terminate.\n");

    while (1) {
        len = sizeof(cliaddr);
        
        // Get the current time
        time(&rawtime);
        info = localtime(&rawtime);
        strftime(buffer, MAX_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", info);

        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&cliaddr, len);

        // Wait for one second before sending the time again
        sleep(1);
    }

    return 0;
}
