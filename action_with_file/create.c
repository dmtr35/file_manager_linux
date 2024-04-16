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


void create_link(char *file_name_link, char *path, char *file_name) {
    // size_t new_path_file_name_link_size = strlen(path) + strlen(file_name_link) + 2;
    // size_t new_path_file_name_size = strlen(path) + strlen(file_name) + 2;
    // char *full_path_file_name_link = malloc(new_path_file_name_link_size * sizeof(char));
    // char *full_path_file_name = malloc(new_path_file_name_size * sizeof(char));

    // if (full_path_file_name == NULL || full_path_file_name_link == NULL) {
    //     perror("Memory allocation failed");
    //     return;
    // }

    // snprintf(full_path_file_name_link, new_path_file_name_link_size, "%s/%s", path, file_name_link);
    // snprintf(full_path_file_name, new_path_file_name_size, "%s/%s", path, file_name);

    // if (symlink(full_path_file_name, full_path_file_name_link) == -1) {
    //     perror("Failed to create symbolic link");
    //     free(full_path_file_name);
    //     free(full_path_file_name_link);
    //     return;
    // }

    // struct stat st;
    // if (stat(full_path, &st) == -1) {
    //     perror("stat");
    //     return;
    // }

    // // Обновляем время доступа и модификации файла
    // struct timeval times[2];
    // times[0].tv_sec = st.st_atime;
    // times[0].tv_usec = 0;
    // times[1].tv_sec = time(NULL);                            // Текущее время
    // times[1].tv_usec = 0;
    // if (utimes(full_path, times) == -1) {
    //     perror("utimes");
    //     return;
    // }

    // free(full_path_file_name);
    // free(full_path_file_name_link);
}

// void create_link(char *file_name_link, char *path, char *file_name) {
//     // Вычисляем размеры для полных путей
//     size_t path_len = strlen(path);
//     size_t file_name_len = strlen(file_name);
//     size_t file_name_link_len = strlen(file_name_link);
//     size_t full_path_file_name_size = path_len + file_name_len + 2;
//     size_t full_path_file_name_link_size = path_len + file_name_link_len + 2;

//     // Выделяем память под строки
//     char *full_path_file_name = malloc(full_path_file_name_size * sizeof(char));
//     char *full_path_file_name_link = malloc(full_path_file_name_link_size * sizeof(char));

//     // Проверяем, была ли память выделена успешно
//     if (full_path_file_name == NULL || full_path_file_name_link == NULL) {
//         perror("Memory allocation failed");
//         return;
//     }

//     // Создаем полные пути к файлам
//     snprintf(full_path_file_name, full_path_file_name_size, "%s/%s", path, file_name);
//     snprintf(full_path_file_name_link, full_path_file_name_link_size, "%s/%s", path, file_name_link);

//     // Создаем символическую ссылку
//     if (symlink(full_path_file_name, full_path_file_name_link) == -1) {
//         perror("Failed to create symbolic link");
//         // Освобождаем память перед выходом из функции в случае ошибки
//         free(full_path_file_name);
//         free(full_path_file_name_link);
//         return;
//     }

//     // Освобождаем выделенную память
//     free(full_path_file_name);
//     free(full_path_file_name_link);
// }