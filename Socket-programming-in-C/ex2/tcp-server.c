#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FILENAME "serverfile.txt"
#define BUFSIZE 1000

int main() {
    int listenfd, connfd, n;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    char buffer[BUFSIZE];
    FILE *file;

    listenfd = socket(PF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(32000);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 0);

    printf("Server listening on port 32000...\n");

    clilen = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

    printf("Client connected.\n");

    // Open the file to send
    file = fopen(FILENAME, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Send file size to the client
    send(connfd, &file_size, sizeof(file_size), 0);

    // Send the file content
    while ((n = fread(buffer, 1, BUFSIZE, file)) > 0) {
        send(connfd, buffer, n, 0);
    }

    printf("File sent successfully.\n");

    fclose(file);
    close(connfd);
    close(listenfd);

    return 0;
}
