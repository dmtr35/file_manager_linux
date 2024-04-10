#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <libgen.h>

#include "../func.h"





void cp_mv_file(struct user_data *ptr_user_data, char *path, char *file_name, _Bool active)
{
    char first_path[512];
    char second_path[512];
    if (active) {
        strcpy(first_path, ptr_user_data->left_path);
        strcpy(second_path, ptr_user_data->right_path);
    } else {
        strcpy(first_path, ptr_user_data->right_path);
        strcpy(second_path, ptr_user_data->left_path);
    }

    size_t size_absolete_first_path = strlen(first_path) + strlen(file_name) + 2;
    size_t size_absolete_second_path = strlen(second_path) + strlen(file_name) + 2;
    char absolete_first_path[size_absolete_first_path];
    char absolete_second_path[size_absolete_second_path];

    snprintf(absolete_first_path, size_absolete_first_path, "%s/%s", first_path, file_name);
    snprintf(absolete_second_path, size_absolete_second_path, "%s/%s", second_path, file_name);


    if (is_directory(absolete_first_path)) {
        copy_directory(absolete_first_path, absolete_second_path);
    } else {
        copy_file(absolete_first_path, absolete_second_path);
    }
}




void copy_directory(const char *source_directory, const char *destination_directory)
{
    struct stat st = {0};
    if(stat(destination_directory, &st) == -1) {
        mkdir(destination_directory, 0700);
    }
    struct stat source_st;                                                                  // Получаем разрешения (mode) исходной директории
    if (stat(source_directory, &source_st) == -1) {
        perror("Ошибка получения разрешений для исходной директории");
        return;
    }
    if (chmod(destination_directory, source_st.st_mode) == -1) {                            // Устанавливаем разрешения для целевой директории
        perror("Ошибка установки разрешений для целевой директории");
    }


    DIR *dir = opendir(source_directory);
    if (dir == NULL){
        return;
    }

    struct dirent *entry;                                                                   // прочитать содержимое директории

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        size_t size_absolute_path_source = strlen(source_directory) + strlen(entry->d_name) + 2;
        size_t size_absolute_path_destination = strlen(destination_directory) + strlen(entry->d_name) + 2;
        char absolute_path_source[size_absolute_path_source];
        char absolute_path_destination[size_absolute_path_destination];
        snprintf(absolute_path_source, size_absolute_path_source, "%s/%s", source_directory, entry->d_name);
        snprintf(absolute_path_destination, size_absolute_path_destination, "%s/%s", destination_directory, entry->d_name);

        if (is_directory(absolute_path_source)) {
            copy_directory(absolute_path_source, absolute_path_destination);
        } else {
            copy_file(absolute_path_source, absolute_path_destination);
        }
    }

    closedir(dir);
}

void copy_file(const char *source_file, const char *destination_file)
{
    FILE *first_file = fopen(source_file, "rb");
    FILE *second_file = fopen(destination_file, "wb");
    if (first_file && second_file) {
        int ch;
        while ((ch = fgetc(first_file)) != EOF) {
            fputc(ch, second_file);
        }
        // printf("Файл скопирован успешно.\n");

        // Получаем разрешения (mode) исходного файла
        struct stat source_st;
        if (stat(source_file, &source_st) == -1) {
            perror("Ошибка получения разрешений для исходного файла");
            return;
        }

        // Устанавливаем разрешения для копии файла
        if (fchmod(fileno(second_file), source_st.st_mode) == -1) {
            perror("Ошибка установки разрешений для копии файла");
        }
    } else {
        perror("Ошибка открытия файлов");
    }
}
