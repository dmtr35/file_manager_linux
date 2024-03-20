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

WINDOW *win_left;
WINDOW *win_right;
WINDOW *win_menu;

int main()
{
    setlocale(LC_ALL, "en_US.UTF-8");
    initscr();             // Инициализация ncurses
    nodelay(stdscr, TRUE); // Set non-blocking mode for standard screen
    noecho();              // Отключаем вывод введенных символов

    keypad(stdscr, TRUE); // Включаем обработку функциональных клавиш
    // printw("%i\n", wgetch(curscr));
    // printw ("%i\n%i\n%i\n", getch (), getch (), getch ());
    curs_set(0);

    struct set_bool set_bool;
    set_bool.hidden_left_bool = 1;
    set_bool.hidden_right_bool = 1;
    set_bool.command_bool = 0;
    set_bool.menu_bool = 0;
    set_bool.help_bool = 0;
    set_bool.out_bool = 0;
    set_bool.save_files = 0;
    _Bool active = 1;
    _Bool check_side = 1;

    char previous_path_left[1024];
    char previous_path_right[1024];

    struct coordinates coords;
    coords.height = 0;
    coords.width = 0;
    coords.height_win = 0;
    coords.width_win = 0;
    coords.height_menu = 0;
    coords.width_menu = 0;
    coords.cursor_x = 1;
    coords.cursor_y = 1;
    coords.quantity_lines_left = 0;
    coords.quantity_lines_right = 0;
    coords.leng_arr_coorsor = 0;
    coords.offset_left = 0;
    coords.offset_right = 0;

    int cursor_left = 1;
    int cursor_right = 1;

    int arr_coorsor[20] = {0};

    struct file_data *all_files_left = (struct file_data *)malloc(500 * sizeof(struct file_data));
    struct file_data *all_files_right = (struct file_data *)malloc(500 * sizeof(struct file_data));
    struct user_data *ptr_user_data = malloc(sizeof(struct user_data));
    int res_check = check_func(ptr_user_data);
    strcpy(previous_path_left, ptr_user_data->left_path);
    strcpy(previous_path_right, ptr_user_data->right_path);

    while (1)
    {
        
        int coords_cursor_y_menu = 3;
        int *offset = active ? &coords.offset_left : &coords.offset_right;
        int *quantity_lines = active ? &coords.quantity_lines_left : &coords.quantity_lines_right;
        int i = coords.cursor_y + *offset - 1;
        int leng_arr_coorsor_full = sizeof(arr_coorsor) / sizeof(*arr_coorsor);
        removeDuplicates(arr_coorsor, leng_arr_coorsor_full);
        active ? strcpy(ptr_user_data->coorsor_file, all_files_left[i].name) : strcpy(ptr_user_data->coorsor_file, all_files_right[i].name);
        size_t leng_path = active ? strlen(ptr_user_data->left_path) + strlen(ptr_user_data->coorsor_file) + 4 : strlen(ptr_user_data->right_path) + strlen(ptr_user_data->coorsor_file) + 4;
        size_t width_menu = leng_path < coords.width / 3 ? coords.width / 3 : leng_path;

        getmaxyx(stdscr, coords.height, coords.width);
        win_left = newwin(coords.height, coords.width / 2, 0, 0);
        win_right = newwin(coords.height, coords.width % 2 ? (coords.width / 2) + 1 : coords.width / 2, 0, coords.width / 2);
        win_menu = newwin(10, width_menu, (coords.height / 2) - 5, coords.width / 2 - width_menu / 2);

        if (!set_bool.command_bool && !set_bool.help_bool && !set_bool.menu_bool)
        {
            if (active)
            {
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
            }
            else
            {
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
            }
        }
        else if (set_bool.command_bool)
        {
            if (active)
            {
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                render_comm_line(ptr_user_data, all_files_right, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left, win_right);
            }
            else
            {
                render_comm_line(ptr_user_data, all_files_right, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left, win_right);
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
            }

            if (active && !set_bool.command_bool)
            {
                continue;
            }
        }
        else if (set_bool.help_bool)
        {
            if (active)
            {
                render_help(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, arr_coorsor, leng_arr_coorsor_full, win_right);
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
            }
            else
            {
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                render_help(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, arr_coorsor, leng_arr_coorsor_full, win_right);
            }
        }
        else if (set_bool.menu_bool)
        {
            _Bool turn_render_ls = true;
            if (active)
            {
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                render_menu(ptr_user_data, all_files_left, all_files_right, &set_bool, &coords_cursor_y_menu, &coords, active, check_side, turn_render_ls, arr_coorsor, leng_arr_coorsor_full, win_menu, win_right, win_left);
            }
            else
            {
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                render_menu(ptr_user_data, all_files_left, all_files_right, &set_bool, &coords_cursor_y_menu, &coords, active, check_side, !turn_render_ls, arr_coorsor, leng_arr_coorsor_full, win_menu, win_right, win_left);
            }
            if (set_bool.out_bool) {
                break;
            } if (!set_bool.menu_bool) {
                continue;
            }
        }

        int ch;
        if (active) {
            ch = wgetch(win_left);
        } else {
            ch = wgetch(win_right);
        }

        if (ch == 'q') {
            break;
        } 
        else if (ch == 'r' || ch == KEY_RESIZE) {
            int new_height, new_width;
            getmaxyx(stdscr, new_height, new_width);
            int cursor_position = coords.cursor_y + *offset;
            if (new_height > coords.height) {
                if (*offset > 0) {
                    (*offset)--;
                    coords.cursor_y++;
                }
            } else if(new_height < coords.height){
                if (*offset <= 0 && coords.cursor_y == coords.height_win - 4) {
                    (*offset)++;
                    coords.cursor_y--;
                } else if (*offset > 0 && coords.cursor_y == coords.height_win - 4) {
                    (*offset)++;
                    coords.cursor_y--;
                } 
            } 
        }
        else if (ch == '\t') {
            if (active) {
                cursor_left = coords.cursor_y;
            } else {
                cursor_right = coords.cursor_y;
            }
            active = !active;
            fillWithZeros(arr_coorsor, &coords, leng_arr_coorsor_full);                   // очистить массив с отметками строк
            if (active) {
                if (cursor_left <= (coords.height_win - 4)) {
                    coords.cursor_y = cursor_left;
                } else {
                    coords.cursor_y = coords.height_win - 4;
                }
            } else {
                if (cursor_right <= (coords.height_win - 4)) {
                    coords.cursor_y = cursor_right;
                } else {
                    coords.cursor_y = coords.height_win - 4;
                }
            }
        }
        else if (ch == '\n') {
            if (active) {
                click_on_file(ptr_user_data->left_path, all_files_left, &coords, previous_path_left, check_side);
                fillWithZeros(arr_coorsor, &coords, leng_arr_coorsor_full);                   // очистить массив с отметками строк
            } else {
                click_on_file(ptr_user_data->right_path, all_files_right, &coords, previous_path_right, !check_side);
                fillWithZeros(arr_coorsor, &coords, leng_arr_coorsor_full);                   // очистить массив с отметками строк
            }
        }
        else if (ch == 27)
        {
            int next1 = wgetch(stdscr);
            int next2 = wgetch(stdscr);
            if (next1 == '[' && next2 == 'A') {
                int hidden_row = *quantity_lines - coords.cursor_y;
                if (coords.cursor_y > 1) {
                    coords.cursor_y--;
                } else if (coords.cursor_y == 1 && *offset > 0) {
                    (*offset)--;
                }
            }
            else if (next1 == '[' && next2 == 'B') {
                int hidden_row = *quantity_lines - coords.cursor_y;
                if (coords.cursor_y < *quantity_lines && coords.cursor_y < coords.height_win - 4) {
                    coords.cursor_y++;
                } else if ((coords.cursor_y >= coords.height_win - 4) && hidden_row > *offset) {
                    (*offset)++;
                }
            } else if (next1 == '[' && next2 == 'C') {
                int hidden_row = *quantity_lines - (coords.height_win - 4) - *offset;
                if (*quantity_lines <= coords.height_win - 4) {
                    coords.cursor_y = *quantity_lines;
                } 
                else {
                    *offset += hidden_row;
                    coords.cursor_y = coords.height_win - 4;
                }
            }
            else if (next1 == '[' && next2 == 'D') {
                *offset = 0;
                coords.cursor_y = 1;
            }
        }
        else if (ch == 127 || ch == KEY_BACKSPACE) {
            if (active) {
                backspace(ptr_user_data->left_path, all_files_left, &coords, &set_bool, previous_path_left, check_side, active);
                fillWithZeros(arr_coorsor, &coords, leng_arr_coorsor_full);                   // очистить массив с отметками строк
            } else {
                backspace(ptr_user_data->right_path, all_files_right, &coords, &set_bool, previous_path_right, !check_side, active);
                fillWithZeros(arr_coorsor, &coords, leng_arr_coorsor_full);                   // очистить массив с отметками строк
            }
        }
        else if (ch == 16) { // Ctrl + p - переход на сохраненный ранее путь
            // else if (ch == 'p') {                                      // Ctrl + p - переход на сохраненный ранее путь
            if (active) {
                char temp[1024];
                strcpy(temp, ptr_user_data->left_path);
                strcpy(ptr_user_data->left_path, previous_path_left);
                strcpy(previous_path_left, temp);
            }
            else {
                char temp[1024];
                strcpy(temp, ptr_user_data->right_path);
                strcpy(ptr_user_data->right_path, previous_path_right);
                strcpy(previous_path_right, temp);
            }
            fillWithZeros(arr_coorsor, &coords, leng_arr_coorsor_full);                   // очистить массив с отметками строк
        }
        else if (ch == ('V' & 0x1F)) { // Обработка нажатия Ctrl + V
            // // else if (ch == 'v') {                            // Обработка нажатия Ctrl + V

            if (active) {
                open_in_vim(ptr_user_data->left_path, all_files_left, &coords, check_side, win_left);
            } else {
                open_in_vim(ptr_user_data->right_path, all_files_right, &coords, !check_side, win_right);
            }
            fillWithZeros(arr_coorsor, &coords, leng_arr_coorsor_full);                   // очистить массив с отметками строк
        }

        else if (ch == '\b' || ch == 'h')
        { // ctrl + h  // help_bool
            // else if (ch == 'a') {
             set_bool.help_bool = !set_bool.help_bool;
        }
        else if (ch == 'm')
        { // ctrl + h  // help_bool
            // else if (ch == 'a') {
            set_bool.menu_bool = !set_bool.menu_bool;
        }
        // else if (ch == 1) {                                           // ch == 1 -> ctrl + a   терминал, приостановлено
        // // else if (ch == 'a') {
        //     set_bool.command_bool = !set_bool.command_bool;
        // }
        else if (ch == 'w')
        { // ctrl + a   терминал, приостановлено
            if(active) {
                set_bool.hidden_left_bool = !set_bool.hidden_left_bool;
            } else {
                set_bool.hidden_right_bool = !set_bool.hidden_right_bool;
            }

            *offset = 0;
            coords.cursor_y = 1;
        }
        if (ch == ' ') {
            if (strcmp(ptr_user_data->coorsor_file, "..") != 0) {
                bool contains = containsElement(arr_coorsor, leng_arr_coorsor_full, i);
                if(contains) {
                    removeFromArr(arr_coorsor, leng_arr_coorsor_full, i);
                    coords.leng_arr_coorsor--;
                } else {
                    addToArr(arr_coorsor, leng_arr_coorsor_full, i);
                    coords.leng_arr_coorsor++;
                }
            }

        }
    }

    endwin();
    free(ptr_user_data);
    free(all_files_left);
    free(all_files_right);
    return 0;
}



