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




void remove_directory_recursive(user_data *ptr_user_data, char *path, char *file_name, _Bool *save_files)
{
    check_and_create_trash(ptr_user_data);
    _Bool save_files_value = false;
    size_t size_new_path = strlen(path) + strlen(file_name) + 3;
    char *full_path = malloc(size_new_path * sizeof(char));
    snprintf(full_path, size_new_path, "%s/%s", path, file_name);

    
    struct stat file_info;
        if (lstat(full_path, &file_info) == -1) {
        perror("Error getting file status");
        return;
    }
    if (*save_files && strcmp(file_name, ".my_trash") != 0 && !S_ISLNK(file_info.st_mode)) {
        save_file(ptr_user_data, path, file_name, full_path);                                   // save_file
    }
    if (S_ISREG(file_info.st_mode) || S_ISLNK(file_info.st_mode)) {
        remove_one_file(full_path);
        return;
    } 


    DIR *dir = opendir(full_path);
    if (dir == NULL){
        return;
    }

    struct dirent *entry;                                                       // прочитать содержимое директории
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        size_t size_absolute_path = strlen(full_path) + strlen(entry->d_name) + 2;
        char absolute_path[size_absolute_path];
        snprintf(absolute_path, size_absolute_path, "%s/%s", full_path, entry->d_name);

        if (is_directory(absolute_path)) {
            remove_directory_recursive(ptr_user_data, full_path, entry->d_name, &save_files_value);
        } else {
            remove_one_file(absolute_path);
        }
    }

    closedir(dir);
    remove_one_file(full_path);
    free(full_path);
}


void remove_one_file(char *path)
{
    remove(path);
}





void save_file(user_data *ptr_user_data, char *path, char *file_name, char *ptr_full_path)
{   
    char *trash_directory = ptr_user_data->trash_directory;
    char dir_or_file[4];

    int res = is_directory(ptr_full_path);
    if (res) {
        strcpy(dir_or_file, "dir");
    } else {
        strcpy(dir_or_file, "fil");
    }

    char *time = get_current_datatime();    
    char *new_path = replace_slashes_dash(path);

    size_t length_tar = strlen(file_name) + strlen(time) + strlen(new_path) + strlen(dir_or_file) + 14;
    char *name_tar = malloc(length_tar * sizeof(char));
    snprintf(name_tar, length_tar, "%s____%s_%s.%s.tar.gz", file_name, time, new_path, dir_or_file);
    size_t length_command_tar = strlen(name_tar) + strlen(path) + strlen(trash_directory)+ strlen(file_name) + 51;
    char command_tar[length_command_tar];
    snprintf(command_tar, length_command_tar, "tar -czf \"%s/%s\" --absolute-names -C \"%s\" \"%s\" 2>/dev/null", trash_directory, name_tar, path, file_name);
    
    system(command_tar);
    free(name_tar);
}




void restore(user_data *ptr_user_data, char *path, char *file_name, _Bool active)
{
    char *trash_directory = ptr_user_data->trash_directory;
    int quantity_lines = active ? ptr_user_data->coordinates.quantity_lines_left : ptr_user_data->coordinates.quantity_lines_right;
    char * copy_path = malloc(strlen(path) + 1);
    extractFileNameAndPath(file_name, copy_path);

    char *restore_path = replace_slashes_dash(copy_path);
    if (is_directory(restore_path)){
        size_t len_absolute_path = strlen(trash_directory) + strlen(file_name) + 2;
        char *absolute_path = malloc(len_absolute_path * sizeof(char));
        if (absolute_path == NULL) {
            exit(EXIT_FAILURE);
        }

        snprintf(absolute_path, len_absolute_path, "%s/%s", trash_directory, file_name);

        size_t lenCommand = strlen(trash_directory) + strlen(file_name) + strlen(restore_path) + 36;
        char command[lenCommand];

        snprintf(command, lenCommand, "tar -xzf \"%s/%s\" --absolute-names -C \"%s\"", trash_directory, file_name, restore_path);

        if(system(command) == 0) {
            remove_one_file(absolute_path);
        }
        free(absolute_path);
    }
    free(copy_path);
}