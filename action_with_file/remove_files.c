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





void remove_directory_recursive(char *path, char *file_name, struct set_bool *set_bool, struct user_data *ptr_user_data, _Bool *save_files)
{
    _Bool save_files_value = false;
    size_t size_new_path = strlen(path) + strlen(file_name) + 3;
    char full_path[size_new_path];
    snprintf(full_path, size_new_path, "%s/%s", path, file_name);

    if (*save_files && strcmp(file_name, ".my_trash") != 0) {
        save_file(path, file_name, ptr_user_data);                                   // save_file
    }
    
    struct stat statbuf;
    if (lstat(full_path, &statbuf) == -1) {
        char original_full_path[size_new_path];
        strcpy(original_full_path, full_path);
        char *ptr = strstr(original_full_path, " ->");
            if (ptr != NULL) {
            *ptr = '\0';
        }
        remove_one_file(original_full_path);
        perror("Error getting file status");
        return;
    }

    if (stat(full_path, &statbuf) == -1) {
        perror("Error getting file status");
        return;
    }


    if (!S_ISDIR(statbuf.st_mode)) {
        remove_one_file(full_path);
        return;
    }


    DIR *dir = opendir(full_path);
    if (dir == NULL){
        return;
    }

    struct dirent *entry; // прочитать содержимое директории

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        size_t size_absolute_path = strlen(full_path) + strlen(entry->d_name) + 2;
        char absolute_path[size_absolute_path];
        snprintf(absolute_path, size_absolute_path, "%s/%s", full_path, entry->d_name);

        if (is_directory(absolute_path)) {
            remove_directory_recursive(full_path, entry->d_name, set_bool, ptr_user_data, &save_files_value);
        } else {
            remove_one_file(absolute_path);
        }
    }

    closedir(dir);
    remove_one_file(full_path);
}


void remove_one_file(char *path)
{
    remove(path);
}





void save_file(char *path, char *file_name, struct user_data *ptr_user_data)
{
    char dir_or_file[4];
    size_t size_absolute_path = strlen(path) + strlen(file_name) + 2;
    char absolute_path[size_absolute_path];
    snprintf(absolute_path, size_absolute_path, "%s/%s", path, file_name);

    int res = is_directory(absolute_path);
    if (res) {
        strcpy(dir_or_file, "dir");
    } else {
        strcpy(dir_or_file, "fil");
    }

    char *time = get_current_datatime();    
    char *new_path = replace_slashes_dash(path);

    size_t length_tar = strlen(file_name) + strlen(time) + strlen(new_path) + strlen(dir_or_file) + 14;
    char name_tar[length_tar];
    snprintf(name_tar, length_tar, "%s____%s_%s.%s.tar.gz", file_name, time, new_path, dir_or_file);
    size_t length_command_tar = strlen(name_tar) + strlen(path) + strlen(ptr_user_data->trash_directory)+ strlen(file_name) + 45;
    char command_tar[length_command_tar];
    snprintf(command_tar, length_command_tar, "tar -czf %s/%s --absolute-names -C %s %s 2>/dev/null", ptr_user_data->trash_directory, name_tar, path, file_name);

    system(command_tar);
}




void restore(char *path, char *file_name, struct user_data *ptr_user_data, struct coordinates *coords, _Bool active)
{
    int quantity_lines = active ? coords->quantity_lines_left : coords->quantity_lines_right;
    extractFileNameAndPath(file_name, path);
    char *restore_path = replace_slashes_dash(path);
    size_t len_absolute_path = strlen(ptr_user_data->trash_directory) + strlen(file_name) + 2;
    char absolute_path[len_absolute_path];

    snprintf(absolute_path, len_absolute_path, "%s/%s", ptr_user_data->trash_directory, file_name);

    size_t lenCommand = strlen(ptr_user_data->trash_directory) + strlen(file_name) + strlen(restore_path) + 32;
    char command[lenCommand];

    snprintf(command, lenCommand, "tar -xzf %s/%s --absolute-names -C %s", ptr_user_data->trash_directory, file_name, restore_path);



    if(system(command) == 0) {
        remove_one_file(absolute_path);
    }
        
}