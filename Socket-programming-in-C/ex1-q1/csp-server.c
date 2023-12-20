#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_SENTENCE_LENGTH 1000

int sockfd;

void sigint_handler(int signo) {
    printf("Terminating the server...\n");
    close(sockfd);
    exit(0);
}

void capitalizeSentence(char *sentence) {
    for (int i = 0; sentence[i] != '\0'; ++i) {
        if (sentence[i] >= 'a' && sentence[i] <= 'z') {
            sentence[i] = sentence[i] - 32; // Convert to uppercase
        }
    }
}

int main(int argc, char** argv) {
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char sentence[MAX_SENTENCE_LENGTH];

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(32000);
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Register the signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigint_handler);

    printf("CSP server is running. Press Ctrl+C to terminate.\n");

    // Wait for the client to initialize the connection
    int num_sentences;
    len = sizeof(cliaddr);
    recvfrom(sockfd, &num_sentences, sizeof(num_sentences), 0, (struct sockaddr *)&cliaddr, &len);
    sendto(sockfd, "ack", 3, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

    for (int i = 0; i < num_sentences; ++i) {
        // Receive sentence from the client
        len = sizeof(cliaddr);
        int n = recvfrom(sockfd, sentence, sizeof(sentence), 0, (struct sockaddr *)&cliaddr, &len);

        capitalizeSentence(sentence);

        sendto(sockfd, sentence, n, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    }

    close(sockfd);

    return 0;
}
