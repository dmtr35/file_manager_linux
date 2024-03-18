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





void click_on_file(char *path, struct file_data *all_files, struct coordinates *coords, char *previous_path, _Bool check_side)
{
    int i = coords->cursor_y + (check_side ? coords->offset_left : coords->offset_right) - 1;
    char *file_name = all_files[i].name;
    char height_win;
    char width_win;


    if ((strchr(all_files[i].permissions, 'd') != NULL)) {
        strcpy(previous_path, path);
        size_t size_new_path = strlen(path) + strlen(file_name) + 2;
        char new_path[size_new_path];
        snprintf(new_path, size_new_path, "%s/%s", path, file_name);
        if ((strstr(new_path, "//") != NULL)) {
            remove_first_char(new_path);
        }
        strcpy(path, new_path);
        coords->cursor_y = 1;

        if (check_side) {
            coords->offset_left = 0;
        } else {
            coords->offset_right = 0;
        }
    } else if ((strcmp(all_files[i].name, "..") == 0)) {
        strcpy(previous_path, path);
        char *parent_dir = dirname(path);
        strcpy(path, parent_dir);
        coords->cursor_y = 1;

        if (check_side) {
            coords->offset_left = 0;
        } else {
            coords->offset_right = 0;
        }
    }
}

void backspace(char *path, struct file_data *all_files, struct coordinates *coords, struct set_bool *set_bool, char *previous_path, _Bool check_side) {
    if (check_side) {
        coords->offset_left = 0;
    } else {
        coords->offset_right = 0;
    }
    int quantity_lines = 0;
    int height_win = coords->height_win - 4;

    size_t leng_path = strlen(path) + 1;
    char path_copy[leng_path];
    strcpy(path_copy, path);
    strcpy(previous_path, path_copy);
    char *parent_dir = dirname(path);
    const char *dirname = basename(path_copy);

    struct file_data *backspace_files = (struct file_data *)malloc(500 * sizeof(struct file_data));
    ls_list(path, backspace_files, set_bool->hidden_files_bool, &quantity_lines);

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
            coords->offset_left = (count + 1) - height_win;
        } else {
            coords->offset_right = (count + 1) - height_win;
        }
        coords->cursor_y = height_win;
    } else {
        coords->cursor_y = (count + 1);
    }

    free(backspace_files);
}

void open_in_vim(char *path, struct file_data *all_files, struct coordinates *coords, _Bool check_side, WINDOW *win)
{
    // int i = coords->cursor_y - 1;
    int i = coords->cursor_y + (check_side ? coords->offset_left : coords->offset_right) - 1;
    
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







