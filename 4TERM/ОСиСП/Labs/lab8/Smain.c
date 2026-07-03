#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>

#define MAX_LEN_BUFFER 1024
#define LEN_DATA 40
#define ECHO_QUERY "ECHO"
#define QUIT_QUERY "QUIT"
#define INFO_QUERY "INFO"
#define LIST_QUERY "LIST"
#define CD_QUERY "CD"
#define LINK_TO_FILE " --> "
#define LINK_TO_LINK " -->> "
#define UNKNOWN_QUERY "UNKNOWN"
#define INIT_CNT_FILE 10
#define START_MESSAGE "myclient server.AF_INET\n"
#define EXIT_MESSAGE "The session has ended. Goodbye.\n"
#define WRONG_PARAMETER "You entered an incorrect parameter.\nList of available parameters: [CD, ECHO, LIST, QUIT, INFO].\n"

char QUERY[MAX_LEN_BUFFER];
char CURRENT_DIR[MAX_LEN_BUFFER];
char LIST_DIR[MAX_LEN_BUFFER];

enum LEVEL_LOGGER {
    __FATAL = 0,
    __ERROR = 1,
    __WARN = 2,
    __INFO = 3,
    __DEBUG = 4,
    __TRACE = 5
};

bool is_new_dir = false;

void execute_dir_args(DIR *__d) {
    struct dirent *dir = NULL;
    struct stat sb;
    memset(LIST_DIR, '\0', strlen(LIST_DIR));
    //strncat(LIST_DIR, "/", strlen("/"));
    strncat(LIST_DIR, CURRENT_DIR, strlen(CURRENT_DIR));
    strncat(LIST_DIR, "\n", strlen("\n"));
    while ((dir = readdir(__d))) {
        char fullpath[MAX_LEN_BUFFER];
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }
        sprintf(fullpath, "%s/%s", CURRENT_DIR, dir->d_name);
        if (lstat(fullpath, &sb) == -1) {
            continue;
        }
        if ((sb.st_mode & __S_IFMT) == __S_IFDIR) {
            strncat(LIST_DIR, dir->d_name, strlen(dir->d_name));
            strncat(LIST_DIR, "/", strlen("/"));
        } else if ((sb.st_mode & __S_IFMT) == __S_IFREG) {
            strncat(LIST_DIR, dir->d_name, strlen(dir->d_name));
        } else if ((sb.st_mode & __S_IFMT) == __S_IFLNK) {
            char target_path[MAX_LEN_BUFFER];
            ssize_t bytes = readlink(fullpath, target_path, sizeof(target_path) - 1);
            if (bytes != -1) {
                lstat(target_path, &sb);
                if ((sb.st_mode & __S_IFMT) == __S_IFDIR) {
                    strncat(LIST_DIR, dir->d_name, strlen(dir->d_name));
                    strncat(LIST_DIR, LINK_TO_FILE, strlen(LINK_TO_FILE));
                    strncat(LIST_DIR, target_path, strlen(target_path));
                } else if ((sb.st_mode & __S_IFMT) == __S_IFDIR) {
                    strncat(LIST_DIR, dir->d_name, strlen(dir->d_name));
                    strncat(LIST_DIR, LINK_TO_LINK, strlen(LINK_TO_LINK));
                    strncat(LIST_DIR, target_path, strlen(target_path));
                }
            }
        }
        strncat(LIST_DIR, "\n", strlen("\n"));
    }
    closedir(__d);
}

void execute_file_args(const char *__filename, char **__buffer, size_t *__size) {
    FILE *file = fopen(__filename, "r");
    if (file == NULL) {
        printf("The file was not found. Check the data is correct.\n");
        return;
    }
    if (__buffer == NULL) {
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_END);
    *__size = ftell(file);
    fseek(file, 0, SEEK_SET);
    *__buffer = (char *) realloc(*__buffer, *__size * sizeof(char));
    fread(*__buffer, sizeof(char), *__size, file);
    fclose(file);
}

void logging_console(int fd_client, const char *__command, const char *__result, unsigned int __loger_level) {
    const time_t date_logging = time(NULL);
    struct tm *u = localtime(&date_logging);
    char str_date[LEN_DATA];
    strftime(str_date, 40, "%d.%m.%Y %H:%M:%S ", u);
    switch (__loger_level) {
        case __FATAL: {
            printf("\033[1;31m%sFATAL\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        case __ERROR: {
            printf("\033[1;31m%sERROR\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        case __WARN: {
            printf("\033[1;33m%sWARN\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        case __INFO: {
            printf("\033[1;32m%sINFO\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        case __DEBUG: {
            printf("\033[1;34m%sDEBUG\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        case __TRACE: {
            printf("\033[1;36m%sTRACE\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        default: break;
    }
}

int main(int argc, char *argv[]) {


    if (argc != 4) {
        fprintf(stderr, "Usage: %s [port], [catalog], [filename with serverinfo]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strncpy(CURRENT_DIR, ".", strlen("."));
    DIR *d = opendir(argv[2]);
    if (d == NULL) {
        printf("The current directory could not be opened. "
            "Please check that the data is correct.");
        exit(EXIT_FAILURE);
    }

    getcwd(CURRENT_DIR, MAX_LEN_BUFFER);

    struct addrinfo hints = {0};
    struct addrinfo *result = NULL;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int soc = getaddrinfo(NULL, argv[1], &hints, &result);

    if (soc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(soc));
        exit(EXIT_FAILURE);
    }

    int socfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    bind(socfd, result->ai_addr, result->ai_addrlen);

    printf("myserver %s\n", argv[1]);
    printf("Ready.\n");

    listen(socfd, SOMAXCONN);
    int fd_client = accept(socfd, result->ai_addr, &result->ai_addrlen);

    execute_dir_args(d);

    const char *filename = argv[3];
    char *serverinfo = NULL;
    size_t file_size = 0;
    execute_file_args(filename, &serverinfo, &file_size);
    write(fd_client, serverinfo, file_size);

    while (true) {
        //memset(QUERY, '\0', strlen(QUERY));
        ssize_t bytes_read = recv(fd_client, QUERY, MAX_LEN_BUFFER, 0);
        size_t size_query = strlen(QUERY);
        if (bytes_read > 0) {
            if (strncasecmp(QUERY, ECHO_QUERY, strlen(ECHO_QUERY)) == 0) {
                logging_console(fd_client, ECHO_QUERY, QUERY, __DEBUG);
                write(fd_client, QUERY + strlen(ECHO_QUERY) + 1, size_query);
            } else if (strncasecmp(QUERY, INFO_QUERY, strlen(INFO_QUERY)) == 0) {
                logging_console(fd_client,INFO_QUERY, QUERY, __DEBUG);
                write(fd_client, serverinfo, file_size);
            } else if (strncasecmp(QUERY, CD_QUERY, strlen(CD_QUERY)) == 0) {
                logging_console(fd_client,CD_QUERY, QUERY, __DEBUG);
                QUERY[size_query - 1] = '\0';
                if ((d = opendir(QUERY + 3)) == NULL) {
                    write(fd_client, "", 1);
                } else {
                    strncpy(CURRENT_DIR, QUERY + 3, size_query);
                    chdir(CURRENT_DIR);
                    getcwd(CURRENT_DIR, MAX_LEN_BUFFER);
                    d = opendir(CURRENT_DIR);
                    QUERY[size_query - 1] = '\n';
                    write(fd_client, QUERY + 3, size_query - 3);
                    is_new_dir = true;
                }
            } else if (strncasecmp(QUERY, LIST_QUERY, strlen(LIST_QUERY)) == 0) {
                logging_console(fd_client,LIST_QUERY, QUERY, __DEBUG);
                if (is_new_dir == true) {
                    execute_dir_args(d);
                    is_new_dir = false;
                }
                write(fd_client, LIST_DIR, strlen(LIST_DIR));
            } else if (strncasecmp(QUERY, QUIT_QUERY, strlen(QUIT_QUERY)) == 0) {
                logging_console(fd_client,QUIT_QUERY, QUERY, __DEBUG);
                write(fd_client, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
            } else {
                logging_console(fd_client,UNKNOWN_QUERY, QUERY, __ERROR);
                write(fd_client, WRONG_PARAMETER, strlen(WRONG_PARAMETER));
            }
        } else if (bytes_read == 0) {
            printf("Client is closed connection\n");
            break;
        } else {
            fprintf(stderr, "Error of reading data.\n");
        }
    }

    free(serverinfo);

    close(fd_client);
    close(socfd);

    return 0;
}
