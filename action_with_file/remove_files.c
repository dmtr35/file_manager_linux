#define _GNU_SOURCE // realpath (полный путь к файлу)
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




void remove_files(char *path, int *arr_coorsor, _Bool active, struct file_data *all_files, struct coordinates *coords)
{
    int quantity_lines = active ? coords->quantity_lines_left : coords->quantity_lines_right;

    for(int i = 0; i < quantity_lines; ++i) {
        for(int j = 0; j < coords->leng_arr_coorsor; ++j) {
            if(all_files[i].file_id == arr_coorsor[j]) {
                int tt;
                tt = all_files[i].file_id;
                size_t leng_file_name = strlen(all_files[i].name) + 1;
                char file_name[leng_file_name];
                strcpy(file_name, all_files[i].name);
                remove_directory_recursive(path, file_name);
            }
        }
    }
}


void remove_directory_recursive(char *path, char *file_name)
{
    size_t size_new_path = strlen(path) + strlen(file_name) + 3;
    char full_path[size_new_path];
    snprintf(full_path, size_new_path, "%s/%s", path, file_name);

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
            remove_directory_recursive(full_path, entry->d_name);
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





// void save_file(char *absolute_path, char *dir_name, char *file_name, struct user_data *ptr_user_data)
// {
//     char dir_or_file[4];
//     int res = is_directory(absolute_path);
//     if (res) {
//         strcpy(dir_or_file, "dir");
//     } else {
//         strcpy(dir_or_file, "fil");
//     }

//     char *time = get_current_datatime();
//     char *new_path = replace_slashes_dash(dir_name);


//     size_t length_tar = strlen(file_name) + strlen(time) + strlen(new_path) + strlen(dir_or_file) + 14;
//     char name_tar[length_tar];
//     snprintf(name_tar, length_tar, "%s____%s_%s.%s.tar.gz", file_name, time, new_path, dir_or_file);
//     size_t length_command_tar = strlen(name_tar) + strlen(absolute_path) + strlen(ptr_user_data->trash_directory) + 41;
//     char command_tar[length_command_tar];
//     snprintf(command_tar, length_command_tar, "tar -czf %s/%s --absolute-names %s 2>/dev/null", ptr_user_data->trash_directory, name_tar, absolute_path);

//     system(command_tar);
// }


// void remove_files(char *arr_files, struct user_data *ptr_user_data)
// {
//     char *token = strtok(arr_files, " "); // разбивка строки на подстроки

//     while (token != NULL)  {
//         char *absolute_path = malloc(1024);
//         realpath(token, absolute_path);
//         char *file_name = token;

//         char *copy_absolute_path = strdup(absolute_path);
//         char *dir_name = dirname(copy_absolute_path);

//         // if (flag_s) {
//         //     save_file(absolute_path, dir_name, file_name, ptr_user_data);
//         // }
//         free(copy_absolute_path);

//         if (is_directory(absolute_path)) {
//             remove_directory_recursive(absolute_path, file_name);
//         } else {
//             remove_one_file(absolute_path, file_name);
//         }

//         free(absolute_path);
//         token = strtok(NULL, " ");
//     }
// }



// void restore(char *arr_files, struct user_data *ptr_user_data, _Bool flag_v, _Bool flag_s)
// {
//     char *token = strtok(arr_files, " ");

//     while (token != NULL) {
//         char *file_name = token;
//         char file_path[strlen(token)];
//         extractFileNameAndPath(file_name, file_path);
//         char *new_path = replace_slashes_dash(file_path);

//         size_t len_absolute_path = strlen(ptr_user_data->trash_directory) + strlen(file_name) + 2;
//         char *absolute_path = malloc(len_absolute_path);
//         snprintf(absolute_path, len_absolute_path, "%s/%s", ptr_user_data->trash_directory, file_name);

//         size_t length = strlen(ptr_user_data->trash_directory) + strlen(file_name) + strlen(new_path) + 32;
//         char command[length];
//         snprintf(command, length, "tar -xzf %s/%s --absolute-names -C %s", ptr_user_data->trash_directory, file_name, new_path);
//         if(system(command) == 0) {
//             remove_one_file(absolute_path, file_name, flag_v, flag_s);
//         }
        
//         token = strtok(NULL, " ");
//     };
// }