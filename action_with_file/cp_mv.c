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





void cp_mv_file(user_data *ptr_user_data, file_data *all_files, char *path, char *file_name, _Bool active)
{
    char *left_path = ptr_user_data->left_path;
    char *right_path = ptr_user_data->right_path;

    char first_path[512];
    char second_path[512];
    if (active) {
        strcpy(first_path, left_path);
        strcpy(second_path, right_path);
    } else {
        strcpy(first_path, right_path);
        strcpy(second_path, left_path);
    }

    size_t size_absolete_first_path = strlen(first_path) + strlen(file_name) + 2;
    size_t size_absolete_second_path = strlen(second_path) + strlen(file_name) + 2;
    char absolete_first_path[size_absolete_first_path];
    char absolete_second_path[size_absolete_second_path];

    snprintf(absolete_first_path, size_absolete_first_path, "%s/%s", first_path, file_name);
    snprintf(absolete_second_path, size_absolete_second_path, "%s/%s", second_path, file_name);


    if (is_directory(absolete_first_path)) {
        copy_directory(ptr_user_data, all_files, absolete_first_path, absolete_second_path, active);
    } else {
        copy_file(ptr_user_data, all_files, absolete_first_path, absolete_second_path, active);
    }
}




void copy_directory(user_data *ptr_user_data, file_data *all_files, char *source_directory, char *destination_directory, _Bool active)
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
        char *file_name = entry->d_name;
        if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0) {
            continue;
        }

        size_t size_absolute_path_source = strlen(source_directory) + strlen(file_name) + 2;
        size_t size_absolute_path_destination = strlen(destination_directory) + strlen(file_name) + 2;
        char absolute_path_source[size_absolute_path_source];
        char absolute_path_destination[size_absolute_path_destination];
        snprintf(absolute_path_source, size_absolute_path_source, (strlen(source_directory) == 1) ? "%s%s" : "%s/%s", source_directory, file_name);
        snprintf(absolute_path_destination, size_absolute_path_destination, (strlen(destination_directory) == 1) ? "%s%s" : "%s/%s", destination_directory, file_name);

        struct stat st;
        if (lstat(absolute_path_source, &st) == -1) {
            perror("Ошибка получения информации о файле");
            return;
        }

        if (S_ISLNK(st.st_mode)) {

            char link_target[1024];
            size_t target_length = readlink(absolute_path_source, link_target, sizeof(link_target));
            link_target[target_length] = '\0';

            size_t length_link_name = strlen(file_name) + strlen(link_target) + 5;
            char link_name[length_link_name];

            snprintf(link_name, (strlen(source_directory) == 1) ? length_link_name + 1 : length_link_name, (strlen(source_directory) == 1) ? "%s -> /%s" : "%s -> %s", entry->d_name, link_target);
            // snprintf(full_path, length_full_path, (strlen(path) == 1) ? "%s%s" : "%s/%s", path, entry->d_name);
//             char *full_path_link = malloc(1024);
//             snprintf(absolute_path_source, size_absolute_path_source, "%s", target_path);
//             // ===================================
// size_t length_full_path = strlen(path) + strlen(entry->d_name) + 2;
//         char full_path[length_full_path];
//         snprintf(full_path, length_full_path, (strlen(path) == 1) ? "%s%s" : "%s/%s", path, entry->d_name);

//         file_data current_file;
//         if (lstat(full_path, &st) == 0) {
//             if (S_ISLNK(file_info.st_mode)) {
//                 char link_target[1024];
//                 size_t target_length = readlink(full_path, link_target, sizeof(link_target));
//                 link_target[target_length] = '\0';
//                 size_t length_link_name = strlen(entry->d_name) + strlen(link_target) + 5;
//                 char link_name[target_length];
                
//                 snprintf(link_name, (strlen(path) == 1) ? length_link_name + 1 : length_link_name, (strlen(path) == 1) ? "%s -> /%s" : "%s -> %s", entry->d_name, link_target);
//                 // ===========================================
            // free(link_target);
        } else {
            perror("Ошибка получения пути к файлу, на который указывает символическая ссылка");
            return;
        }


        if (is_directory(absolute_path_source)) {
            copy_directory(ptr_user_data, all_files, absolute_path_source, absolute_path_destination, active);
        } else {
            copy_file(ptr_user_data, all_files, absolute_path_source, absolute_path_destination, active);
        }
    }

    closedir(dir);
}


void copy_file(user_data *ptr_user_data, file_data *all_files, char *source_file, char *destination_file, _Bool active)
{
    char *current_directory_from = active ? ptr_user_data->left_path : ptr_user_data->right_path;
    char *current_directory_to = active ? ptr_user_data->right_path : ptr_user_data->left_path;
    int *offset = active ? &(ptr_user_data->coordinates.offset_left) : &(ptr_user_data->coordinates.offset_right);
    int *cursor_y = &ptr_user_data->coordinates.cursor_y;
    int i = item(ptr_user_data, active);

    size_t size_source_file = strlen(source_file) + 1;
      
    char *file_name = all_files[i].name;
    
    if((strchr(all_files[i].permissions, 'l') != NULL)) {
        char *path_link = malloc(size_source_file * sizeof(char));
        char *name_link = malloc(size_source_file * sizeof(char));
        split_link(file_name, path_link, name_link);

        size_t size_destination_file = strlen(current_directory_to) + strlen(name_link) + 2;

        snprintf(destination_file, size_destination_file, "%s/%s", current_directory_to, name_link);


        if (symlink(path_link, destination_file) == -1) {
            perror("Ошибка создания символической ссылки");
            free(path_link);
            free(name_link);
            return;
        }
        free(path_link);
        free(name_link);
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
            if (stat(source_file, &source_st) == -1) {
                perror("Ошибка получения разрешений для исходного файла");
                return;
            }

            // Устанавливаем разрешения для копии файла
            if (fchmod(fileno(second_file), source_st.st_mode) == -1) {
                perror("Ошибка установки разрешений для копии файла");
            }

            fclose(first_file);
            fclose(second_file);
        } else {
            perror("Ошибка открытия файлов");
        }
    }


}
