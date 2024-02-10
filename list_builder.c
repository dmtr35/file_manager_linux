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


int ls_list(char *path, struct file_data *directories, int dir_count, _Bool flag_hidden_files)
{
    char symb[2];
    
    DIR *dir = opendir(path);
    if (dir == NULL){
        perror("Error opening directory");
        return 1;
    }

    struct dirent *entry;
    struct stat file_info;

    struct file_data *files = (struct file_data *)malloc(500 * sizeof(struct file_data));
    int file_count = 0;

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
                strcpy(symb, "l");

                file_data(&current_file, link_name, &file_info, symb);

                files[file_count++] = current_file;
            } else {
                strcpy(symb, (S_ISDIR(file_info.st_mode)) ? "d" : "-");
                file_data(&current_file, entry->d_name, &file_info, symb);

                if (S_ISDIR(file_info.st_mode)) {
                    directories[dir_count++] = current_file;
                } else {
                    files[file_count++] = current_file;
                }
            }
        } 
    }
    closedir(dir);

    for(int i = 0; i < file_count; ++i) {
        directories[dir_count++] = files[i];
    }

    free(files);
    return dir_count;
}


void file_data(struct file_data *current_file, char *file_name, struct stat *file_info, char *symb)
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


// int rendering(struct file_data *directories, int total_count)
// {
//     // printf("total_count: %u\n", total_count);
//     for (int i = 0; i < total_count; i++) {
//         printf("Directory Name: %s\n", directories[i].name);
//         printf("Size: %s\n", directories[i].size);
//         printf("Permissions: %s\n", directories[i].permissions);
//         printf("Modification Time: %s\n", directories[i].time);
//         printf("\n");
//     }

//     // for (int i = 0; i < file_count; i++) {
//     //     printf("File Name: %s\n", files[i].name);
//     //     printf("Size: %s\n", files[i].size);
//     //     printf("Permissions: %s\n", files[i].permissions);
//     //     printf("Modification Time: %s\n", files[i].time);
//     //     printf("\n");
//     // }

//         // mvwprintw(win_left, row_to_highlight, 1, "file_manager");

//         // mvwprintw(win_left, row_l1++, 1, "file_manager.c");
//         // mvwprintw(win_left, row_l1++, 1, "q_info.txt");
//         // mvwprintw(win_left, row_l1++, 1, "test.c");

//         // mvwprintw(win_left, row_l2++, width / 2 - 32, " size");
//         // mvwprintw(win_left, row_l2++, width / 2 - 32, "55,3K");
//         // mvwprintw(win_left, row_l2++, width / 2 - 32, "55,3K");
//         // mvwprintw(win_left, row_l2++, width / 2 - 32, "55130");
//         // mvwprintw(win_left, row_l2++, width / 2 - 32, "55,3K");
        
        
//         // mvwprintw(win_left, row_l3++, width / 2 - 26, "     data");
//         // mvwprintw(win_left, row_l3++, width / 2 - 26, "31.01.24 17:52");
//         // mvwprintw(win_left, row_l3++, width / 2 - 26, "31.01.24 17:53");
//         // mvwprintw(win_left, row_l3++, width / 2 - 26, "29.01.24 18:25");
//         // mvwprintw(win_left, row_l3++, width / 2 - 26, "30.01.24 18:07");
        
//         // mvwprintw(win_left, row_l4++, width / 2 - 11, "permission");
//         // mvwprintw(win_left, row_l4++, width / 2 - 11, "-rwxrwxr-x");
//         // mvwprintw(win_left, row_l4++, width / 2 - 11, "-rw-rw-r--");
//         // mvwprintw(win_left, row_l4++, width / 2 - 11, "-rw-rw-r--");
//         // mvwprintw(win_left, row_l4++, width / 2 - 11, "-rw-rw-r--");


//     return 0;
// }

