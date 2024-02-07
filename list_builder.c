#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <stdint.h>


#include "func.h"



int ls_list(char *path, _Bool flag_hidden_files) {
    printf("home: %s\n", path);
    printf("\n");

    char buf[20];
    
    DIR *dir = opendir(path);
    if (dir == NULL){
        perror("Error opening directory");
        return 1;
    }

    struct dirent *entry;
    struct stat file_info;

    struct file_data directories[100];
    struct file_data files[100];
    int dir_count = 0, file_count = 0;

    if (strlen(path) == 1) {
        strcpy(directories[dir_count].name, "/");
    } else {
        strcpy(directories[dir_count].name, "..");
    }
    strcpy(directories[dir_count].size, "size");
    strcpy(directories[dir_count].time, "date");
    strcpy(directories[dir_count].permissions, "permission");
    dir_count++;

    while ((entry = readdir(dir)) != NULL) {
        if ((strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)){
            continue;
        }
        if ((strchr(entry->d_name, '.') == entry->d_name) && flag_hidden_files){
            continue;
        }
        size_t length_full_path = strlen(path) + strlen(entry->d_name) + 2;
        char full_path[length_full_path];
        snprintf(full_path, length_full_path, (strlen(path) == 1) ? "%s%s" : "%s/%s", path, entry->d_name);

        struct file_data current_file;
        if (lstat(full_path, &file_info) == 0) {
            if (S_ISLNK(file_info.st_mode)) {
                char link_target[1024];
                size_t target_length = readlink(full_path, link_target, sizeof(link_target));
                link_target[target_length] = '\0';

                size_t length_link_name = strlen(entry->d_name) + strlen(link_target) + 5;
                char link_name[target_length];
                snprintf(link_name, length_link_name, "%s -> %s\n", entry->d_name, link_target);

                
                strncpy(current_file.name, link_name, sizeof(current_file.name));
                strncpy(current_file.size, human_readable_size(file_info.st_size, buf), sizeof(current_file.size));
                strncpy(current_file.time, format_last_modification_time(file_info.st_mtime, buf), sizeof(current_file.time));
                sprintf(current_file.permissions, "%s%s%s%s%s%s%s%s%s%s",
                    "l",
                    (file_info.st_mode & S_IRUSR) ? "r" : "-",
                    (file_info.st_mode & S_IWUSR) ? "w" : "-",
                    (file_info.st_mode & S_IXUSR) ? "x" : "-",
                    (file_info.st_mode & S_IRGRP) ? "r" : "-",
                    (file_info.st_mode & S_IWGRP) ? "w" : "-",
                    (file_info.st_mode & S_IXGRP) ? "x" : "-",
                    (file_info.st_mode & S_IROTH) ? "r" : "-",
                    (file_info.st_mode & S_IWOTH) ? "w" : "-",
                    (file_info.st_mode & S_IXOTH) ? "x" : "-");

                files[file_count++] = current_file;
            } else {
                strncpy(current_file.name, entry->d_name, sizeof(current_file.name));
                strncpy(current_file.size, human_readable_size(file_info.st_size, buf), sizeof(current_file.size));
                strncpy(current_file.time, format_last_modification_time(file_info.st_mtime, buf), sizeof(current_file.time));
                sprintf(current_file.permissions, "%s%s%s%s%s%s%s%s%s%s",
                    (S_ISDIR(file_info.st_mode)) ? "d" : "-",
                    (file_info.st_mode & S_IRUSR) ? "r" : "-",
                    (file_info.st_mode & S_IWUSR) ? "w" : "-",
                    (file_info.st_mode & S_IXUSR) ? "x" : "-",
                    (file_info.st_mode & S_IRGRP) ? "r" : "-",
                    (file_info.st_mode & S_IWGRP) ? "w" : "-",
                    (file_info.st_mode & S_IXGRP) ? "x" : "-",
                    (file_info.st_mode & S_IROTH) ? "r" : "-",
                    (file_info.st_mode & S_IWOTH) ? "w" : "-",
                    (file_info.st_mode & S_IXOTH) ? "x" : "-");

                if (S_ISDIR(file_info.st_mode)) {
                    directories[dir_count++] = current_file;
                } else {
                    files[file_count++] = current_file;
                }
            }
        } 
    }
    closedir(dir);

    rendering(directories, files, dir_count, file_count);
    return 0;
}


int rendering(struct file_data *directories, struct file_data *files, int dir_count, int file_count)
{

     for (int i = 0; i < dir_count; i++) {
        printf("Directory Name: %s\n", directories[i].name);
        printf("Size: %s\n", directories[i].size);
        printf("Permissions: %s\n", directories[i].permissions);
        printf("Modification Time: %s\n", directories[i].time);
        printf("\n");
    }

    for (int i = 0; i < file_count; i++) {
        printf("File Name: %s\n", files[i].name);
        printf("Size: %s\n", files[i].size);
        printf("Permissions: %s\n", files[i].permissions);
        printf("Modification Time: %s\n", files[i].time);
        printf("\n");
    }





    return 0;
}

