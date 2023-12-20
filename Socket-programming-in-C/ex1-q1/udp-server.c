#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int sockfd;

void sigint_handler(int signo) {
    printf("Terminating the server...\n");
    close(sockfd);
    exit(0);
}

int main(int argc, char** argv) {
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char mesg[1000];
    char* banner = "Hello UDP client! This is UDP server";

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(32000);
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Register the signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigint_handler);

    printf("Server is running. Press Ctrl+C to terminate.\n");


    //because of this loop, server will listen to the clients until user terminate the program 
    while (1) {
        len = sizeof(cliaddr);
        int n = recvfrom(sockfd, mesg, sizeof(mesg), 0, (struct sockaddr *)&cliaddr, &len);
        sendto(sockfd, banner, n, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
        mesg[n] = 0;
        printf("Received: %s\n", mesg);
    }

    return 0;
}
