#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <libgen.h>

#include <ncurses.h>
#include <locale.h>
#include <signal.h>

#include "func.h"





void click_on_file(user_data *ptr_user_data, file_data *all_files, _Bool active,_Bool check_side)
{
    char *path = check_side ? ptr_user_data->left_path : ptr_user_data->right_path;
    int *cursor_y = &ptr_user_data->coordinates.cursor_y;
    char *previous_path = active ? ptr_user_data->previous_path_left : ptr_user_data->previous_path_right;
    int i = *cursor_y + (check_side ? ptr_user_data->coordinates.offset_left : ptr_user_data->coordinates.offset_right) - 1;
    char *file_name = all_files[i].name;
    
    

    if ((strchr(all_files[i].permissions, 'd') != NULL)) {
        strcpy(previous_path, path);
        size_t size_new_path = strlen(path) + strlen(file_name) + 2;
        char new_path[size_new_path];
        snprintf(new_path, size_new_path, "%s/%s", path, file_name);
        if ((strstr(new_path, "//") != NULL)) {
            remove_first_char(new_path);
        }
        strcpy(path, new_path);
        *cursor_y = 1;

        if (check_side) {
            ptr_user_data->coordinates.offset_left = 0;
        } else {
            ptr_user_data->coordinates.offset_right = 0;
        }
    } else if ((strcmp(all_files[i].name, "..") == 0)) {
        strcpy(previous_path, path);
        char *parent_dir = dirname(path);
        strcpy(path, parent_dir);
        *cursor_y = 1;

        if (check_side) {
            ptr_user_data->coordinates.offset_left = 0;
        } else {
            ptr_user_data->coordinates.offset_right = 0;
        }
    } else if (strchr(all_files[i].permissions, 'l') != 0) {
        size_t full_path_size = strlen(path) + strlen(all_files[i].name) + 2;
        char *full_path = malloc(1024);
        snprintf(full_path, full_path_size, "%s/%s", path, all_files[i].name);


        if (is_directory(full_path)) {
            strcpy(path, full_path);

            if (check_side) {
                ptr_user_data->coordinates.offset_left = 0;
            } else {
                ptr_user_data->coordinates.offset_right = 0;
            }
        }

        *cursor_y = 1;
        free(full_path);
    }
    strcpy(check_side ? ptr_user_data->left_path : ptr_user_data->right_path, path);
}


void backspace(user_data *ptr_user_data, file_data *all_files, _Bool active, _Bool check_side)
{
    int *cursor_y = &ptr_user_data->coordinates.cursor_y;
    char path[256];
    if (check_side) {
        ptr_user_data->coordinates.offset_left = 0;
        strcpy(path, ptr_user_data->left_path);
    } else {
        ptr_user_data->coordinates.offset_right = 0;
        strcpy(path, ptr_user_data->right_path);
    }
    int quantity_lines = 0;
    int height_win = ptr_user_data->coordinates.height_win - 4;

    size_t leng_path = strlen(path) + 1;
    char path_copy[leng_path];
    strcpy(path_copy, path);
    strcpy(active ? ptr_user_data->previous_path_left : ptr_user_data->previous_path_right, path_copy);
    char *parent_dir = dirname(path);
    const char *dirname = basename(path_copy);

    file_data *backspace_files = (file_data *)malloc(500 * sizeof(file_data));
    _Bool *hidden_files = active ? &ptr_user_data->set_bool.hidden_right_bool : &ptr_user_data->set_bool.hidden_right_bool;
    ls_list(ptr_user_data, backspace_files, check_side, hidden_files, &quantity_lines);

    int count = 0;
    for(; count < quantity_lines; ++count){
        char rrr[250];
        strcpy(rrr, backspace_files->name);
        if((strstr(backspace_files[count].name, dirname)) != 0) {
            break;
        }
    }

    if(height_win <= count) {
        if (check_side) {
            ptr_user_data->coordinates.offset_left = (count + 1) - height_win;
        } else {
            ptr_user_data->coordinates.offset_right = (count + 1) - height_win;
        }
        *cursor_y = height_win;
    } else {
        *cursor_y = (count + 1);
    }

    strcpy(check_side ? ptr_user_data->left_path : ptr_user_data->right_path, path);
    free(backspace_files);
}


void open_in_vim(user_data *ptr_user_data, file_data *all_files, _Bool check_side, WINDOW *win)
{
    int *cursor_y = &ptr_user_data->coordinates.cursor_y;
    int *offset_left = &ptr_user_data->coordinates.offset_left;
    int *offset_right = &ptr_user_data->coordinates.offset_right;
    char *path = check_side ? ptr_user_data->left_path : ptr_user_data->right_path;
    int i = *cursor_y + (check_side ? *offset_left : *offset_right) - 1;
    char *file_name = all_files[i].name;

    size_t full_path_size = strlen(path) + strlen(file_name) + 2;
    char *full_path = malloc(1024);
    snprintf(full_path, full_path_size, "%s/%s", path, file_name);

    struct stat file_info;
    if (lstat(full_path, &file_info) == 0) {
        if (S_ISLNK(file_info.st_mode)) {
            link_to_file(full_path);                                // достать ссылку на файл из ссылки
        } else if (S_ISDIR(file_info.st_mode)) {
            return;
        }
    }


    

    
    char height_win;
    char width_win;
    getmaxyx(win, height_win, width_win);
    char command[256];

    snprintf(command, sizeof(command), "vim -c \"edit %s\" -c \"wincmd w\" -c \"resize %d\" -c \"wincmd w\" -c \"resize %d\"", full_path, height_win, width_win);

    system(command);
    curs_set(1);
    curs_set(0);

    wclear(win);
    wnoutrefresh(win);

    free(full_path);
}







