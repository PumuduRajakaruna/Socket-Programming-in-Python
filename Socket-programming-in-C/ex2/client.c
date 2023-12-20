#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 

#define BUFSIZE 1000

int main(int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char buffer[BUFSIZE];
    long file_size;

    if (argc != 2) {
        printf("usage: ./%s <IP address>\n", argv[0]);
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(32000);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Receive file size from the server
    recv(sockfd, &file_size, sizeof(file_size), 0);
    printf("Receiving file of size: %ld bytes\n", file_size);

    // Open file for writing
    FILE *file = fopen("serverfile.txt", "wb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Receive and write file content
    while (file_size > 0) {
        n = recv(sockfd, buffer, sizeof(buffer), 0);
        fwrite(buffer, 1, n, file);
        file_size -= n;
    }

    printf("File received successfully.\n");

    fclose(file);
    close(sockfd);

    return 0;
}
