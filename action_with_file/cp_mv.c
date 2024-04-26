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

#include "../header.h"





void cp_mv_file(user_data *ptr_user_data, file_data *all_files, char *path, char *file_name, _Bool active)
{
    char *left_path = ptr_user_data->left_path;
    char *right_path = ptr_user_data->right_path;

    char first_path[1024];
    char second_path[1024];
    if (active) {
        strcpy(first_path, left_path);
        strcpy(second_path, right_path);
    } else {
        strcpy(first_path, right_path);
        strcpy(second_path, left_path);
    }

    size_t size_absolete_first_path = strlen(first_path) + strlen(file_name) + 2;
    size_t size_absolete_second_path = strlen(second_path) + strlen(file_name) + 2;
    char *absolete_first_path = malloc(1024);
    char *absolete_second_path = malloc(1024);

    snprintf(absolete_first_path, size_absolete_first_path, "%s/%s", first_path, file_name);
    snprintf(absolete_second_path, size_absolete_second_path, "%s/%s", second_path, file_name);


    if (is_directory(absolete_first_path)) {
        copy_directory(ptr_user_data, all_files, absolete_first_path, absolete_second_path, active);
    } else {
        copy_file(ptr_user_data, all_files, absolete_first_path, absolete_second_path, active);
    }
    free(absolete_first_path);
    free(absolete_second_path);
}




void copy_directory(user_data *ptr_user_data, file_data *all_files, char *source_directory, char *destination_directory, _Bool active)
{

    struct stat file_info;
    if (lstat(source_directory, &file_info) == 0) {
        if(S_ISLNK(file_info.st_mode)){
            struct stat targetInfo;
            if (stat(source_directory, &targetInfo) == 0) {
                if (S_ISDIR(targetInfo.st_mode)) {
                    split_and_creation_link(source_directory, destination_directory);
                } 
            }
        } else if (S_ISDIR(file_info.st_mode)) {
            if (stat(destination_directory, &file_info) == -1) {
                mkdir(destination_directory, 0700);
            } struct stat source_st;                                        // Получаем разрешения (mode) исходной директории
            if (stat(source_directory, &source_st) == -1) {
                perror("Ошибка получения разрешений для исходной директории");
                return;
            }
            if (chmod(destination_directory, source_st.st_mode) == -1) { // Устанавливаем разрешения для целевой директории
                perror("Ошибка установки разрешений для целевой директории");
            }


            DIR *dir = opendir(source_directory);
            if (dir == NULL) {
                return;
            }

            struct dirent *entry; // прочитать содержимое директории

            while ((entry = readdir(dir)) != NULL) {
                char *file_name = entry->d_name;
                if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0) {
                    continue;
                }
                size_t size_absolute_path_source = strlen(source_directory) + strlen(file_name) + 2;
                size_t size_absolute_path_destination = strlen(destination_directory) + strlen(file_name) + 2;
                char *absolute_path_source = malloc(size_absolute_path_source);
                char *absolute_path_destination = malloc(size_absolute_path_destination);
                snprintf(absolute_path_source, size_absolute_path_source, "%s/%s", source_directory, file_name);
                snprintf(absolute_path_destination, size_absolute_path_destination, "%s/%s", destination_directory, file_name);

                // ======================================================================================================
                struct stat file_info;
                if (lstat(absolute_path_source, &file_info) == 0) {
                    if(S_ISLNK(file_info.st_mode)){
                        struct stat targetInfo;
                        if (stat(absolute_path_source, &targetInfo) == 0) {
                            if (S_ISDIR(targetInfo.st_mode)) {
                                split_and_creation_link(absolute_path_source, absolute_path_destination);
                            } else {
                                copy_file(ptr_user_data, all_files, absolute_path_source, absolute_path_destination, active);
                            }
                        }
                    } else {
                        if (is_directory(absolute_path_source)) {
                            copy_directory(ptr_user_data, all_files, absolute_path_source, absolute_path_destination, active);
                        } else {
                            copy_file(ptr_user_data, all_files, absolute_path_source, absolute_path_destination, active);
                        }
                    }
                    free(absolute_path_source);
                    free(absolute_path_destination);
                } 
            }

            closedir(dir);
        }
    } 
}


void copy_file(user_data *ptr_user_data, file_data *all_files, char *source_file, char *destination_file, _Bool active)
{
    struct stat file_info;
    if (lstat(source_file, &file_info) == -1) {
        perror("Error getting file status");
        return;
    } 
    
    if (S_ISLNK(file_info.st_mode)) {
        split_and_creation_link(source_file, destination_file);
    } else {
        FILE *first_file = fopen(source_file, "rb");
        FILE *second_file = fopen(destination_file, "wb");
        if (first_file && second_file) {
            int ch;
            while ((ch = fgetc(first_file)) != EOF) {
                fputc(ch, second_file);
            }

            // Получаем разрешения (mode) исходного файла
            struct stat source_st;
            if (lstat(source_file, &source_st) == -1) {
                perror("Ошибка получения разрешений для исходного файла");
                return;
            }

            // Устанавливаем разрешения для копии файла
            if (fchmod(fileno(second_file), source_st.st_mode) == -1) {
                perror("Ошибка установки разрешений для копии файла");
            }

            fclose(first_file);
            fclose(second_file);
        } 
    }



}


void split_and_creation_link(char *source_directory, char *destination_directory)
{
    link_to_file(source_directory);

    char *last_slash = strrchr(destination_directory, '/');
    size_t path_size = last_slash - destination_directory + 1; // +1 для нулевого символа завершения строки

    // Выделяем память для пути и имени файла
    char *path = malloc(path_size);
    char *file_name = malloc(strlen(last_slash + 1));

    strncpy(path, destination_directory, path_size);
    path[path_size - 1] = '\0';
    strcpy(file_name, last_slash + 1);

    create_link(file_name, path, source_directory);

    free(path);
    free(file_name);

}