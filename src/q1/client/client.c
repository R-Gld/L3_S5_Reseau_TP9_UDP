#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_LENGTH 2048

int main(int argc, char **argv) {
    if(argc != 4) {
        fprintf(stderr, "Usage: %s <addressServer> <portServer> <message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *addressServer = argv[1];
    int portServer = atoi(argv[2]);
    char *message = argv[3];

    if(portServer < 1 || portServer > 65535) {
        fprintf(stderr, "The port should be an int between 1 and 65535\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(portServer);

    if(inet_aton(addressServer, &server.sin_addr) == 0) {
        fprintf(stderr, "The address %s can't be parsed as an IPv4 address\n", addressServer);
        return EXIT_FAILURE;
    }

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    if(sendto(sock_fd, message, strlen(message) + 1, 0, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("sendto");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    printf("Message sent.");

    close(sock_fd);

    return EXIT_SUCCESS;
}
