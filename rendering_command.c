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




void rendering_comm(struct user_data *ptr_user_data, struct coordinates *coords, _Bool left_of_right, _Bool active, _Bool *bool_win_command, WINDOW *win)
{

    size_t size_user_data_home = strlen(ptr_user_data->home_path);
    size_t size_user_data_current = strlen(ptr_user_data->left_path) + 1;
    char current_path_comm[size_user_data_current];
    char *substr_start = strstr(ptr_user_data->left_path, ptr_user_data->home_path);

    if (substr_start != NULL) {
        char *start_of_path = substr_start + size_user_data_home;
        sprintf(current_path_comm, "~%s", start_of_path);
    } else {
        strcpy(current_path_comm, ptr_user_data->left_path);
    }
    size_t cursor_coords = strlen(ptr_user_data->user) + strlen(current_path_comm) + 3;

    wattron(win, A_BOLD);
    wprintw(win, "%s:%s$", ptr_user_data->user, current_path_comm);
    wattroff(win, A_BOLD);

    wmove(win, 0, cursor_coords);

    bool is_enter_pressed = false;
    int row;

    if (active) {
        while (!is_enter_pressed) {
            int ch = wgetch(win);
            if (ch != ERR) {
                if (ch == '\t') {
                    is_enter_pressed = true;
                    bool_win_command = false;
                } else if (ch == '\n') {
                    row++;
                    wattron(win, A_BOLD);
                    wprintw(win, "\n%s:%s$ ", ptr_user_data->user, current_path_comm);
                    wattroff(win, A_BOLD);
                    wmove(win, row, cursor_coords);
                } else if (ch == KEY_BACKSPACE || ch == 127) {
                    remove_char_from_command_line(win, cursor_coords);
                } else {
                    add_char_to_command_line(win, ch);
                }
            }
        }
    }

    wrefresh(win);
}


void add_char_to_command_line(WINDOW *win, char c) {
    waddch(win, c);
    wrefresh(win);
}

void remove_char_from_command_line(WINDOW *win, size_t cursor_coords) {
    int y, x;
    getyx(win, y, x);
    if (x > cursor_coords) {
        mvwaddch(win, y, x - 1, ' ');                                           // Заменяем предыдущий символ пробелом
        wmove(win, y, x - 1);                                                   // Перемещаем курсор на предыдущий символ
        wrefresh(win);
    }
}