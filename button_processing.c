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





void click_on_file(struct user_data *ptr_user_data, struct file_data *all_files, _Bool active,_Bool check_side)
{
    char path[256];
    strcpy(path, check_side ? ptr_user_data->left_path : ptr_user_data->right_path);
    int i = ptr_user_data->coordinates.cursor_y + (check_side ? ptr_user_data->coordinates.offset_left : ptr_user_data->coordinates.offset_right) - 1;
    char *file_name = all_files[i].name;


    if ((strchr(all_files[i].permissions, 'd') != NULL)) {
        strcpy(active ? ptr_user_data->previous_path_left : ptr_user_data->previous_path_right, path);
        size_t size_new_path = strlen(path) + strlen(file_name) + 2;
        char new_path[size_new_path];
        snprintf(new_path, size_new_path, "%s/%s", path, file_name);
        if ((strstr(new_path, "//") != NULL)) {
            remove_first_char(new_path);
        }
        strcpy(path, new_path);
        ptr_user_data->coordinates.cursor_y = 1;

        if (check_side) {
            ptr_user_data->coordinates.offset_left = 0;
        } else {
            ptr_user_data->coordinates.offset_right = 0;
        }
    } else if ((strcmp(all_files[i].name, "..") == 0)) {
        strcpy(active ? ptr_user_data->previous_path_left : ptr_user_data->previous_path_right, path);
        char *parent_dir = dirname(path);
        strcpy(path, parent_dir);
        ptr_user_data->coordinates.cursor_y = 1;

        if (check_side) {
            ptr_user_data->coordinates.offset_left = 0;
        } else {
            ptr_user_data->coordinates.offset_right = 0;
        }
    }
    strcpy(check_side ? ptr_user_data->left_path : ptr_user_data->right_path, path);
}

void backspace(struct user_data *ptr_user_data, struct file_data *all_files, _Bool active, _Bool check_side)
{
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

    struct file_data *backspace_files = (struct file_data *)malloc(500 * sizeof(struct file_data));
    _Bool *hidden_files = active ? &ptr_user_data->set_bool.hidden_right_bool : &ptr_user_data->set_bool.hidden_right_bool;
    ls_list(path, backspace_files, hidden_files, &quantity_lines);

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
        ptr_user_data->coordinates.cursor_y = height_win;
    } else {
        ptr_user_data->coordinates.cursor_y = (count + 1);
    }

    strcpy(check_side ? ptr_user_data->left_path : ptr_user_data->right_path, path);
    free(backspace_files);
}

void open_in_vim(struct user_data *ptr_user_data, struct file_data *all_files, _Bool active, _Bool check_side, WINDOW *win)
{
    int i = ptr_user_data->coordinates.cursor_y + (check_side ? ptr_user_data->coordinates.offset_left : ptr_user_data->coordinates.offset_right) - 1;
    
    char *file_name = all_files[i].name;
    char height_win;
    char width_win;

    if ((strchr(all_files[i].permissions, '-') == 0) || (strchr(all_files[i].permissions, 'l') == 0)) {
        getmaxyx(win, height_win, width_win);
        char command[256];
        snprintf(command, sizeof(command), "vim -c \"edit %s\" -c \"wincmd w\" -c \"resize %d\" -c \"wincmd w\" -c \"resize %d\"", file_name, height_win, width_win);

        system(command);
        curs_set(1);
        curs_set(0);

        wclear(win);
        wnoutrefresh(win);
    }
}







