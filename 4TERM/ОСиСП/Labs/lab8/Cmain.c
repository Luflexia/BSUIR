#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#define MAX_LEN_BUFFER 1024
#define QUIT_QUERY "QUIT"

bool is_from_file = false;
char BUFFER[MAX_LEN_BUFFER];
char CORRECT_BUFFER[MAX_LEN_BUFFER];
char QUERY[MAX_LEN_BUFFER];


bool is_end_session() {
    if (strncasecmp(QUERY, QUIT_QUERY, strlen(QUIT_QUERY)) == 0) {
        return true;
    }
    return false;
}

void proccesing_query(int __fd_client, bool* flag_end) {
    write(__fd_client, QUERY, MAX_LEN_BUFFER);
    *flag_end = is_end_session();
    ssize_t bytes_read = recv(__fd_client, QUERY, MAX_LEN_BUFFER, 0);
    QUERY[bytes_read] = '\0';
    if (bytes_read > 0) {
        fputs(QUERY, stdout);
    } else {
        fprintf(stderr, "The server dropped the connection");
        close(__fd_client);
        exit(EXIT_FAILURE);
    }
}

void processing_query_from_file(int __fd_client, bool* flag_end) {
    FILE* file = fopen(QUERY + 1, "r");
    if (file == NULL) {
        printf("File is not found.\n");
        return;
    }
    while(!feof(file)) {
      //  memset(QUERY, '\0', strlen(QUERY));
        fgets(QUERY, MAX_LEN_BUFFER, file);
        printf("> %s", QUERY);
        proccesing_query(__fd_client, flag_end);
        if (*flag_end == true) {
            return;
        }
    }
}


int main(int argc, char* argv[])
{

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints = {0};
    struct addrinfo* result = NULL;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = 0;

    int s = getaddrinfo(NULL, argv[1], &hints, &result);

    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    int fd_client = socket(hints.ai_family, hints.ai_socktype, hints.ai_addrlen);
    int flag_connect = connect(fd_client, result->ai_addr, result->ai_addrlen);

    if (flag_connect == -1) {
        printf("Server is close\n");
        exit(EXIT_FAILURE);
    }

    read(fd_client, BUFFER, MAX_LEN_BUFFER);
    printf("%s", BUFFER);

    bool flag_end = false;
    while(flag_end == false) {
        printf("> ");
        fgets(QUERY,MAX_LEN_BUFFER, stdin);
        if (QUERY[0] == '@') {
            QUERY[strlen(QUERY) - 1] = '\0';
            processing_query_from_file(fd_client, &flag_end);
        } else {
            proccesing_query(fd_client, &flag_end);
        }
    }

    close(fd_client);
    return 0;
}
