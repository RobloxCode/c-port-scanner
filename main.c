#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int top;
    int bottom;
} Range;

bool is_port_used(int port);
void scan_port_range(Range r);

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "USAGE: ./a [bottom] [top]\n");
        return EXIT_FAILURE;
    }

    int bottom = 0;
    int top = 0;

    bottom = (int)strtol(argv[1], NULL, 10);
    top = (int)strtol(argv[2], NULL, 10);

    scan_port_range((Range){top, bottom});

    return EXIT_SUCCESS;
}

bool is_port_used(int port) {
    int sockfd;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return false;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(sockfd);
        return true;
    }

    close(sockfd);
    return false;
}

void scan_port_range(Range r) {
    for (int i = r.bottom; i < r.top; ++i) {
        if (is_port_used(i)) {
            printf("the port: %d, is used\n", i);
        } else {
            printf("the port: %d, is not used\n", i);
        }
    }
}
