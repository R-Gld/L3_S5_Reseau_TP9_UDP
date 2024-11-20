#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_LENGTH 100010
#define CHUNK_SIZE 1000

int main(int argc, char **argv) {
    if(argc != 4) {
        fprintf(stderr, "Usage: %s <addressServer> <portServer> <pi_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *addressServer = argv[1];
    int portServer = atoi(argv[2]);

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

    char pi_content[MAX_LENGTH];

    FILE *pi_file = fopen(argv[3], "r");
    if(pi_file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    if(fgets(pi_content, sizeof(pi_content), pi_file) == NULL) {
        perror("fgets");
        fclose(pi_file);
        return EXIT_FAILURE;
    }

    fclose(pi_file);

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    size_t total_length = strlen(pi_content);
    size_t progress = 0;

    while(progress < total_length) {
        size_t chunk_size = (progress + CHUNK_SIZE < total_length) ? CHUNK_SIZE : total_length - progress;

        if(sendto(sock_fd, pi_content + progress, chunk_size, 0, (struct sockaddr *)&server, sizeof(server)) == -1) {
            perror("sendto");
            close(sock_fd);
            return EXIT_FAILURE;
        }

        progress += chunk_size;
        printf("Sent %zu/%zu bytes\n", progress, total_length);
    }

    close(sock_fd);

    return EXIT_SUCCESS;
}
