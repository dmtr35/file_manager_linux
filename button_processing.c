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

    if ((strchr(all_files[i].permissions, 'd') != NULL)) {
        strcpy(previous_path, path);
        char *folder_name = all_files[i].name;
        size_t size_new_path = strlen(path) + strlen(folder_name) + 2;
        char new_path[size_new_path];
        snprintf(new_path, size_new_path, "%s/%s", path, folder_name);
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
}





