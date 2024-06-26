#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <utime.h>
#include <errno.h>


#include <ncurses.h>
#include <signal.h>

#include "../header.h"



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



void create_link(char *file_name_link, char *path, char *path_to_file_name) {
    size_t size_path_to_name_link = strlen(path) + strlen(file_name_link) + 2;
    char *full_path_file_name_link = malloc(size_path_to_name_link * sizeof(char));

    if (full_path_file_name_link == NULL) {
        perror("Memory allocation failed");
        return;
    }

    snprintf(full_path_file_name_link, size_path_to_name_link, "%s/%s", path, file_name_link);

    if (symlink(path_to_file_name, full_path_file_name_link) == -1) {
        fprintf(stderr, "Failed to create symbolic link: %s\n", strerror(errno));
        free(full_path_file_name_link);
        return;
    }


    free(full_path_file_name_link);
}



void rename_file(char *new_file_name, char *path, char *name_file_row)
{
    size_t size_old_directory = strlen(path) + strlen(name_file_row) + 2;
    char *old_directory = malloc(size_old_directory * sizeof(char));
    snprintf(old_directory, size_old_directory, "%s/%s", path, name_file_row);

    size_t size_new_directory = strlen(path) + strlen(new_file_name) + 2;
    char *new_directory = malloc(size_new_directory * sizeof(char));
    snprintf(new_directory, size_new_directory, "%s/%s", path, new_file_name);

    rename(old_directory, new_directory);

    free(old_directory);
    free(new_directory);
}


