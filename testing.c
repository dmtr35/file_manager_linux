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


void render_comm_XXX(struct user_data *ptr_user_data, struct file_data *all_files, struct coordinates *coords, int *quantity_lines_right, _Bool *bool_win_command, _Bool flag_hidden_files, _Bool active, int offset, WINDOW *win_left, WINDOW *win_right)
{
    // initscr();
    
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

    wattron(win_left, A_BOLD);
    wprintw(win_left, "%s:%s$", ptr_user_data->user, current_path_comm);
    wattroff(win_left, A_BOLD);

    wmove(win_left, 0, cursor_coords);

    bool is_enter_pressed = false;
    int row;

    if (active) {
        while (!is_enter_pressed) {
            int ch = wgetch(win_left);

            getmaxyx(stdscr, coords->height, coords->width);
            // win_left = newwin(coords->height, coords->width / 2, 0, 0);
            win_right = newwin(coords->height, coords->width % 2 ? (coords->width / 2) + 1 : coords->width / 2, 0, coords->width / 2);

            if (ch != ERR) {
                if (ch == 1 || ch == 'a') {
                    is_enter_pressed = true;
                    *bool_win_command = false;
                } else if (ch == '\n') {
                    render_ls(ptr_user_data->right_path, all_files, coords, quantity_lines_right, flag_hidden_files, !active, offset, win_right);
                    row++;
                    wattron(win_left, A_BOLD);
                    wprintw(win_left, "\n%s:%s$ ", ptr_user_data->user, current_path_comm);
                    wattroff(win_left, A_BOLD);
                    wmove(win_left, row, cursor_coords);
                } else if (ch == KEY_BACKSPACE || ch == 127) {
                    remove_char_from_command_line(win_left, cursor_coords);
                } else if (ch == KEY_RESIZE) {
                    render_ls(ptr_user_data->right_path, all_files, coords, quantity_lines_right, flag_hidden_files, !active, offset, win_right);                                                                                           // Обновление окна
                } else {
                    add_char_to_command_line(win_left, ch);
                } 
            }
        }
    }
    wrefresh(win_left);
}


void add_char_to_command_line(WINDOW *win_left, char c) {
    waddch(win_left, c);
    wrefresh(win_left);
}

void remove_char_from_command_line(WINDOW *win_left, size_t cursor_coords) {
    int y, x;
    getyx(win_left, y, x);
    if (x > cursor_coords) {
        mvwaddch(win_left, y, x - 1, ' ');                                           // Заменяем предыдущий символ пробелом
        wmove(win_left, y, x - 1);                                                   // Перемещаем курсор на предыдущий символ
        wrefresh(win_left);
    }
}