#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_LENGTH 2048

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    if(port < 1 || port > 65535) {
        fprintf(stderr, "The port should be an int between 1 and 65535\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = INADDR_ANY;
    memset(&local.sin_zero, 0, sizeof(local.sin_zero));

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    if(bind(sock_fd, (struct sockaddr *)&local, sizeof(local)) == -1) {
        perror("bind");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    char message[MAX_LENGTH];
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

    ssize_t received = recvfrom(sock_fd, message, MAX_LENGTH, 0, (struct sockaddr *)&client, &client_len);
    if(received == -1) {
        perror("recvfrom");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    puts(message);

    close(sock_fd);

    return EXIT_SUCCESS;
}
