#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>

#include <ncurses.h>
#include <signal.h>

#include "func.h"

void render_menu(struct user_data *ptr_user_data, struct file_data *all_files, struct coordinates *coords, _Bool active, WINDOW *win)
{
    start_color();
    // init_color(COLOR_BLUE, 0, 0, 650);
    // init_color(COLOR_WHITE, 1000, 1000, 1000);
    // init_color(COLOR_MAGENTA, 300, 300, 300);

    init_pair(15, COLOR_RED, COLOR_WHITE); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(2, COLOR_CYAN, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне.       // files // hide folder
    init_pair(3, COLOR_WHITE, COLOR_CYAN); // Цветовая пара 2: Красный текст на темносинем фоне       // link 
    init_pair(4, COLOR_GREEN, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне       // ex      
    init_pair(5, COLOR_RED, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне.        // zip
    init_pair(6, COLOR_BLACK, COLOR_WHITE);

    init_pair(11, COLOR_WHITE, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(12, COLOR_CYAN, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(13, COLOR_YELLOW, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(14, COLOR_GREEN, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(15, COLOR_RED, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(16, COLOR_BLACK, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    wbkgd(win, COLOR_PAIR(3));

    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    int height_win;
    int width_win;
    getmaxyx(win, height_win, width_win);

    char path[256];
    strcpy(path, active ? ptr_user_data->left_path : ptr_user_data->right_path);
    char *file_name = ptr_user_data->coorsor_file;

    size_t size_new_path = strlen(path) + strlen(file_name) + 3;
    char new_path[size_new_path];

    snprintf(new_path, size_new_path, "%s/%s", path, file_name);


    wattron(win, A_BOLD);
    // wattron(win, COLOR_PAIR(6));
    // mvwprintw(win, 1, width_win > , path);
    mvwprintw(win, 1, 1, new_path);
    // wattroff(win, COLOR_PAIR(6));
    // wprintw(win, "%s", path);
    wattroff(win, A_BOLD);

    //     char screen_buffer[10000] = {0};
    //     int buffer_pos = 0;

    //     size_t size_user_data_home = strlen(ptr_user_data->home_path);
    //     size_t size_user_data_current = strlen(ptr_user_data->left_path) + 1;
    //     char current_path_comm[size_user_data_current];
    //     char *substr_start = strstr(ptr_user_data->left_path, ptr_user_data->home_path);

    //     if (substr_start != NULL) {
    //         char *start_of_path = substr_start + size_user_data_home;
    //         sprintf(current_path_comm, "~%s", start_of_path);
    //     } else {
    //         strcpy(current_path_comm, ptr_user_data->left_path);
    //     }
    //     size_t cursor_coords = strlen(ptr_user_data->user) + strlen(current_path_comm) + 3;

    //     // wattron(win_left, A_BOLD);
    //     wprintw(win_left, "%s:%s$", ptr_user_data->user, current_path_comm);
    //     // wattroff(win_left, A_BOLD);
    //     save_to_buffer(ptr_user_data->user, screen_buffer, &buffer_pos);
    //     save_to_buffer(":", screen_buffer, &buffer_pos);
    //     save_to_buffer(current_path_comm, screen_buffer, &buffer_pos);
    //     save_to_buffer("$ ", screen_buffer, &buffer_pos);

    //     wmove(win_left, 0, cursor_coords);

    //     bool is_enter_pressed = false;
    //     int row;

    //     if (active) {
    //         while (!is_enter_pressed) {
    //             // printf("%s\n", screen_buffer);
    //             int ch = wgetch(win_left);

    //             getmaxyx(stdscr, coords->height, coords->width);
    //             // win_left = newwin(coords->height, coords->width / 2, 0, 0);
    //             win_right = newwin(coords->height, coords->width % 2 ? (coords->width / 2) + 1 : coords->width / 2, 0, coords->width / 2);

    //             if (ch != ERR) {
    //                 if (ch == 1 || ch == 'a') {
    //                     is_enter_pressed = true;
    //                     *bool_win_command = false;
    //                 } else if (ch == 'z') {
    //                     restore_from_buffer(win_left, screen_buffer);
    //                 } else if (ch == '\n') {
    //                     save_to_buffer("\n", screen_buffer, &buffer_pos);
    //                     save_to_buffer(ptr_user_data->user, screen_buffer, &buffer_pos);
    //                     save_to_buffer(":", screen_buffer, &buffer_pos);
    //                     save_to_buffer(current_path_comm, screen_buffer, &buffer_pos);
    //                     save_to_buffer("$ ", screen_buffer, &buffer_pos);
    //                     render_ls(ptr_user_data->right_path, all_files, coords, quantity_lines_right, flag_hidden_files, !active, offset, win_right);
    //                     row++;

    //                     count_row++;

    //                     if (count_row >= coords->height) {
    //                         int offset = (count_row - coords->height) + 1;
    //                         restore_from_buffer_offset(win_left, screen_buffer, offset);
    //                     } else {
    //                         int offset = -5;
    //                         restore_from_buffer_offset(win_left, screen_buffer, offset);
    //                         // restore_from_buffer(win_left, screen_buffer);
    //                     }

    //                 } else if (ch == KEY_BACKSPACE || ch == 127) {
    //                     remove_char_from_command_line(win_left, cursor_coords, screen_buffer, &buffer_pos);
    //                 } else if (ch == KEY_RESIZE) {
    //                     render_ls(ptr_user_data->right_path, all_files, coords, quantity_lines_right, flag_hidden_files, !active, offset, win_right);                                                                                           // Обновление окна
    //                     getmaxyx(stdscr, coords->height, coords->width);
    //                     win_left = newwin(coords->height, coords->width / 2, 0, 0);
    //                     restore_from_buffer(win_left, screen_buffer);

    //                 } else {
    //                     add_char_to_command_line(win_left, ch, screen_buffer, &buffer_pos);
    //                         // printf("%ld:%d\n", cursor_coords, coords->width / 2);
    //                     // if (cursor_coords >= coords->width / 2) {
    //                     //     row++;
    //                     // }
    //                     // else {
    //                         // row++;
    //                     // }
    //                     // printf("ch: %c", ch);
    //                     // printf("x: %d, y: %d", coords->cursor_y, coords->cursor_x);
    //                 }
    //             }
    //         }
    //     }
        wrefresh(win);
}
