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

#include "../func.h"

char screen_buffer[10000] = {0};
int buffer_pos = 0;
int count_row = 0;


void render_comm_line(struct user_data *ptr_user_data, struct file_data *all_files, _Bool active, _Bool check_side, WINDOW *win_left, WINDOW *win_right)
{
    int *height = &ptr_user_data->coordinates.height;
    int *width = &ptr_user_data->coordinates.width;
    char *user = ptr_user_data->user;
    char *left_path = ptr_user_data->left_path;
    char *home_path = ptr_user_data->home_path;

    char screen_buffer[10000] = {0};
    int buffer_pos = 0;

    size_t size_user_data_home = strlen(home_path);
    size_t size_user_data_current = strlen(left_path) + 1;
    char current_path_comm[size_user_data_current];
    char *substr_start = strstr(left_path, home_path);


    if (substr_start != NULL) {
        char *start_of_path = substr_start + size_user_data_home;
        sprintf(current_path_comm, "~%s", start_of_path);
    } else {
        strcpy(current_path_comm, left_path);
    }
    size_t cursor_coords = strlen(user) + strlen(current_path_comm) + 3;

    // wattron(win_left, A_BOLD);
    wprintw(win_left, "%s:%s$", user, current_path_comm);
    // wattroff(win_left, A_BOLD);
    save_to_buffer(user, screen_buffer, &buffer_pos);
    save_to_buffer(":", screen_buffer, &buffer_pos);
    save_to_buffer(current_path_comm, screen_buffer, &buffer_pos);
    save_to_buffer("$ ", screen_buffer, &buffer_pos);


    wmove(win_left, 0, cursor_coords);

    bool is_enter_pressed = false;
    int row;

    if (active) {
        while (!is_enter_pressed) {
            // printf("%s\n", screen_buffer);
            int ch = wgetch(win_left);

            getmaxyx(stdscr, *height, *width);
            // win_left = newwin(coords->height, coords->width / 2, 0, 0);
            win_right = newwin(*height, *width % 2 ? (*width / 2) + 1 : *width / 2, 0, *width / 2);

            if (ch != ERR) {
                if (ch == 1 || ch == 'a') {
                    is_enter_pressed = true;
                    ptr_user_data->set_bool.command_bool = false;
                } else if (ch == 'z') {
                    restore_from_buffer(win_left, screen_buffer);
                } else if (ch == '\n') {
                    save_to_buffer("\n", screen_buffer, &buffer_pos);
                    save_to_buffer(user, screen_buffer, &buffer_pos);
                    save_to_buffer(":", screen_buffer, &buffer_pos);
                    save_to_buffer(current_path_comm, screen_buffer, &buffer_pos);
                    save_to_buffer("$ ", screen_buffer, &buffer_pos);
                    render_ls(ptr_user_data, all_files, !active, !check_side, win_right);
                    row++;
                    // wattron(win_left, A_BOLD);
                    // wprintw(win_left, "\n%s:%s$ ", user, current_path_comm);
                    // wattroff(win_left, A_BOLD);
                    // wmove(win_left, row, cursor_coords);

                    count_row++;
                    
                    if (count_row >= *height) {
                        int offset = (count_row - *height) + 1;
                        restore_from_buffer_offset(win_left, screen_buffer, offset);
                    } else {
                        int offset = -5;
                        restore_from_buffer_offset(win_left, screen_buffer, offset);
                        // restore_from_buffer(win_left, screen_buffer);
                    }
                    
                } else if (ch == KEY_BACKSPACE || ch == 127) {
                    remove_char_from_command_line(win_left, cursor_coords, screen_buffer, &buffer_pos);
                } else if (ch == KEY_RESIZE) {
                    render_ls(ptr_user_data, all_files, !active, check_side, win_right);                                                                                           // Обновление окна
                    getmaxyx(stdscr, *height, *width);
                    win_left = newwin(*height, *width / 2, 0, 0);                    
                    restore_from_buffer(win_left, screen_buffer);
                    
                } else {
                    add_char_to_command_line(win_left, ch, screen_buffer, &buffer_pos);
                        // printf("%ld:%d\n", cursor_coords, coords->width / 2);
                    // if (cursor_coords >= coords->width / 2) {
                    //     row++;
                    // } 
                    // else {
                        // row++;
                    // }
                    // printf("ch: %c", ch);
                    // printf("x: %d, y: %d", coords->cursor_y, coords->cursor_x);
                } 
            }
        }
    }
    wrefresh(win_left);
}

