#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <stdint.h>


#include "func.h"


int ls_list(user_data *ptr_user_data, file_data *all_files, char *path, _Bool check_side, _Bool *flag_hidden_files, int *quantity_lines)
{
    char *current_directory = check_side ? ptr_user_data->left_path : ptr_user_data->right_path;
    size_t len_current_directory = strlen(current_directory);

    char symb[2];
    
    DIR *dir = opendir(path);
    if (dir == NULL){
        perror("Error opening directory");
        return 1;
    }

    struct dirent *entry;
    struct stat file_info;

    file_data *files = (file_data *)malloc(500 * sizeof(file_data));
    int file_count = 0;

    if (strlen(path) == 1) {
        strcpy(all_files[*quantity_lines].name, "/");
    } else {
        strcpy(all_files[*quantity_lines].name, "..");
    }
    strcpy(all_files[*quantity_lines].size, " size");
    strcpy(all_files[*quantity_lines].time, "     data");
    strcpy(all_files[*quantity_lines].permissions, "permission");
    (*quantity_lines)++;

    int file_id = 0;
    while ((entry = readdir(dir)) != NULL) {
        if ((strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)){
            continue;
        }
        if ((strchr(entry->d_name, '.') == entry->d_name) && *flag_hidden_files){
            continue;
        }

        size_t length_full_path = strlen(path) + strlen(entry->d_name) + 2;
        char full_path[length_full_path];
        snprintf(full_path, length_full_path, (strlen(path) == 1) ? "%s%s" : "%s/%s", path, entry->d_name);

        file_data current_file;
        if (lstat(full_path, &file_info) == 0) {
            if (S_ISLNK(file_info.st_mode)) {
                char link_target[1024];
                size_t target_length = readlink(full_path, link_target, sizeof(link_target));
                link_target[target_length] = '\0';
                size_t length_link_name = strlen(entry->d_name) + strlen(link_target) + 5;
                char link_name[target_length];
                
                snprintf(link_name, (strlen(path) == 1) ? length_link_name + 1 : length_link_name, (strlen(path) == 1) ? "%s -> /%s" : "%s -> %s", entry->d_name, link_target);
                strcpy(symb, "l");

                form_current_file(&current_file, link_name, &file_info, symb, file_id);

                files[file_count++] = current_file;
            } else {
                strcpy(symb, (S_ISDIR(file_info.st_mode)) ? "d" : "-");
                form_current_file(&current_file, entry->d_name, &file_info, symb, file_id);

                if (S_ISDIR(file_info.st_mode)) {
                    all_files[(*quantity_lines)++] = current_file;
                } else {
                    files[file_count++] = current_file;
                }
            }
        }
        
    }
    closedir(dir);

    for(int i = 0; i < file_count; ++i) {
        all_files[*quantity_lines] = files[i];
        (*quantity_lines)++;
    }
    for(int i = 0; i < *quantity_lines; ++i) {
        all_files[i].file_id = i;
    }

    free(files);
}


void form_current_file(file_data *current_file, char *file_name, struct stat *file_info, char *symb, int file_id)
{
    char buf[20];

    strncpy(current_file->name, file_name, sizeof(current_file->name));
    strncpy(current_file->size, human_readable_size(file_info->st_size, buf), sizeof(current_file->size));
    strncpy(current_file->time, format_last_modification_time(file_info->st_mtime, buf), sizeof(current_file->time));
    sprintf(current_file->permissions, "%s%s%s%s%s%s%s%s%s%s",
            symb,
            (file_info->st_mode & S_IRUSR) ? "r" : "-",
            (file_info->st_mode & S_IWUSR) ? "w" : "-",
            (file_info->st_mode & S_IXUSR) ? (((file_info->st_mode & S_ISUID) ? "s" : "x")) : ((file_info->st_mode & S_ISUID) ? "S" : "-"),
            (file_info->st_mode & S_IRGRP) ? "r" : "-",
            (file_info->st_mode & S_IWGRP) ? "w" : "-",
            (file_info->st_mode & S_IXGRP) ? (((file_info->st_mode & S_ISGID) ? "s" : "x")) : ((file_info->st_mode & S_ISGID) ? "S" : "-"),
            (file_info->st_mode & S_IROTH) ? "r" : "-",
            (file_info->st_mode & S_IWOTH) ? "w" : "-",
            (file_info->st_mode & S_IXOTH) ? (((file_info->st_mode & S_ISVTX) ? "t" : "x")) : ((file_info->st_mode & S_ISVTX) ? "T" : "-"));
}

