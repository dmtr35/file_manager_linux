#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <utime.h>



#include <ncurses.h>
#include <signal.h>

#include "../func.h"



void mkdir_p(char *file_name, char *path) {
    size_t new_path_size = strlen(path) + strlen(file_name) + 2;
    char new_path[new_path_size];
    strcpy(new_path, path);
    char *token = strtok(file_name, "/");

    while (token != NULL) {

        size_t new_len = strlen(new_path) + strlen(token) + 2;
        char *full_path = malloc(new_len * sizeof(char));
        snprintf(full_path, new_len, "%s/%s", new_path, token);

        struct stat st = {0};
        if (stat(full_path, &st) == -1) {
            if (mkdir(full_path, S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
                perror("mkdir failed");
            }
        }

        struct timeval times[2];
        gettimeofday(&times[0], NULL);
        times[1] = times[0];

        if (utimes(full_path, times) == -1)
        {
            perror("utimes");
            free(full_path);
            return;
        }

        strcpy(new_path, full_path);
        free(full_path);
        token = strtok(NULL, "/");
    }
}

void touch_file(char *file_name, char *path) {
    size_t new_path_size = strlen(path) + strlen(file_name) + 2;
    char *full_path = malloc(new_path_size * sizeof(char));
    snprintf(full_path, new_path_size, "%s/%s", path, file_name);

    int fd = open(full_path, O_CREAT | O_WRONLY, 0666);
    if (fd == -1) {
        perror("open failed");
        free(full_path);
        return;
    }

    close(fd);


    struct timeval times[2];
        gettimeofday(&times[0], NULL);
        times[1] = times[0];

    if (utimes(full_path, times) == -1)
    {
        perror("utimes");
        free(full_path);
        return;
    }

    free(full_path);
}


// void create_link(char *file_name, char *path) {
//     size_t new_path_size = strlen(path) + strlen(file_name) + 2;
//     char *full_path = malloc(new_path_size * sizeof(char));
//     snprintf(full_path, new_path_size, "%s/%s", path, file_name);

//     int fd = open(full_path, O_CREAT | O_WRONLY, 0666);
//     if (fd == -1) {
//         perror("open failed");
//         free(full_path);
//         return;
//     }

//     close(fd);

//     struct stat st;
//     if (stat(full_path, &st) == -1) {
//         perror("stat");
//         return;
//     }

//     // Обновляем время доступа и модификации файла
//     struct timeval times[2];
//     times[0].tv_sec = st.st_atime;
//     times[0].tv_usec = 0;
//     times[1].tv_sec = time(NULL);                            // Текущее время
//     times[1].tv_usec = 0;
//     if (utimes(full_path, times) == -1) {
//         perror("utimes");
//         return;
//     }

//     free(full_path);
// }