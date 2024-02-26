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





void click_on_file(char *path, struct file_data *all_files, struct coordinates *coords, char *previous_path)
{
    int i = coords->cursor_y - 1;
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
    } else if ((strcmp(all_files[i].name, "..") == 0)) {
        strcpy(previous_path, path);
        char *parent_dir = dirname(path);
        strcpy(path, parent_dir);
        coords->cursor_y = 1;
    } 
    // else if ((strchr(all_files[i].permissions, '-') == 0) || (strchr(all_files[i].permissions, 'l') == 0)) {




    // }

}

void open_in_vim(char *path, struct file_data *all_files, struct coordinates *coords, WINDOW *win)
{
    int i = coords->cursor_y - 1;
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

