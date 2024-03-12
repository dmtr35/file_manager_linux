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
    set_bool.hidden_files_bool = 1;
    set_bool.command_bool = 0;
    set_bool.menu_bool = 0;
    set_bool.help_bool = 0;
    set_bool.out_bool = 0;
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
    coords.offset_left = 0;
    coords.offset_right = 0;

    int cursor_left = 1;
    int cursor_right = 1;

    char arr_coorsor[20] = {0};

    struct file_data *all_files_left = (struct file_data *)malloc(500 * sizeof(struct file_data));
    struct file_data *all_files_right = (struct file_data *)malloc(500 * sizeof(struct file_data));
    struct user_data *ptr_user_data = malloc(sizeof(struct user_data));
    int res_check = check_func(ptr_user_data);
    strcpy(previous_path_left, ptr_user_data->left_path);
    strcpy(previous_path_right, ptr_user_data->right_path);

    while (1)
    {
        int coords_cursor_y_menu = 3;
        int i = coords.cursor_y + (check_side ? coords.offset_left : coords.offset_right) - 1;
        arr_coorsor[0] = i + 1;
        // arr_coorsor[1] = 5;
        // arr_coorsor[2] = 7;
        // arr_coorsor[3] = 9;
        size_t leng_arr_coorsor = sizeof(arr_coorsor) / sizeof(*arr_coorsor);
        active ? strcpy(ptr_user_data->coorsor_file, all_files_left[i].name) : strcpy(ptr_user_data->coorsor_file, all_files_right[i].name);
        size_t leng_path = active ? strlen(ptr_user_data->left_path) + strlen(ptr_user_data->coorsor_file) + 4 : strlen(ptr_user_data->right_path) + strlen(ptr_user_data->coorsor_file) + 4;
        size_t width_menu = leng_path < coords.width / 3 ? coords.width / 3 : leng_path;

        // int quantity_lines_left;
        // int quantity_lines_right;
        getmaxyx(stdscr, coords.height, coords.width);
        win_left = newwin(coords.height, coords.width / 2, 0, 0);
        // win_right = newwin(coords.height, coords.width / 2, 0, coords.width / 2);
        win_right = newwin(coords.height, coords.width % 2 ? (coords.width / 2) + 1 : coords.width / 2, 0, coords.width / 2);
        win_menu = newwin(10, width_menu, (coords.height / 2) - 5, coords.width / 2 - width_menu / 2);

        if (!set_bool.command_bool && !set_bool.help_bool && !set_bool.menu_bool)
        {
            if (active)
            {
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor, win_right);
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor, win_left);
            }
            else
            {
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor, win_left);
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor, win_right);
            }
        }
        else if (set_bool.command_bool)
        {
            if (active)
            {
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor, win_right);
                render_comm_line(ptr_user_data, all_files_right, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor, win_left, win_right);
            }
            else
            {
                render_comm_line(ptr_user_data, all_files_right, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor, win_left, win_right);
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor, win_right);
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
                render_help(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, arr_coorsor, leng_arr_coorsor, win_right);
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor, win_left);
            }
            else
            {
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor, win_left);
                render_help(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, arr_coorsor, leng_arr_coorsor, win_right);
            }
        }
        else if (set_bool.menu_bool)
        {
            _Bool turn_render_ls = true;
            if (active)
            {
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor, win_right);
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor, win_left);
                render_menu(ptr_user_data, all_files_left, all_files_right, &set_bool, &coords_cursor_y_menu, &coords, active, check_side, turn_render_ls, arr_coorsor, leng_arr_coorsor, win_menu, win_right, win_left);
            }
            else
            {
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor, win_left);
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor, win_right);
                render_menu(ptr_user_data, all_files_left, all_files_right, &set_bool, &coords_cursor_y_menu, &coords, active, check_side, !turn_render_ls, arr_coorsor, leng_arr_coorsor, win_menu, win_right, win_left);
            }
            if (set_bool.out_bool)
            {
                break;
            }
            if (!set_bool.menu_bool)
            {
                continue;
            }
        }

        int ch;
        if (active)
        {
            ch = wgetch(win_left);
        }
        else
        {
            ch = wgetch(win_right);
        }

        if (ch == 'q')
        {
            break;
        }
        else if (ch == '\t')
        {
            if (active)
            {
                cursor_left = coords.cursor_y;
            }
            else
            {
                cursor_right = coords.cursor_y;
            }
            active = !active;
            if (active)
            {
                if (cursor_left <= (coords.height_win - 4))
                {
                    coords.cursor_y = cursor_left;
                }
                else
                {
                    coords.cursor_y = coords.height_win - 4;
                }
            }
            else
            {
                if (cursor_right <= (coords.height_win - 4))
                {
                    coords.cursor_y = cursor_right;
                }
                else
                {
                    coords.cursor_y = coords.height_win - 4;
                }
            }
        }
        else if (ch == '\n')
        {
            if (active)
            {
                click_on_file(ptr_user_data->left_path, all_files_left, &coords, previous_path_left, check_side);
            }
            else
            {
                click_on_file(ptr_user_data->right_path, all_files_right, &coords, previous_path_right, !check_side);
            }
        }
        else if (ch == 27)
        {
            int next1 = wgetch(stdscr);
            int next2 = wgetch(stdscr);
            if (next1 == '[' && next2 == 'A')
            {
                int offset_to_increment = active ? coords.offset_left : coords.offset_right; // вверх
                int quantity_lines = (active ? coords.quantity_lines_left : coords.quantity_lines_right);
                int hidden_row = quantity_lines - coords.cursor_y;
                if (coords.cursor_y > 1)
                {
                    coords.cursor_y--;
                }
                else if (coords.cursor_y == 1 && offset_to_increment > 0)
                {
                    if (active)
                    {
                        coords.offset_left--;
                    }
                    else
                    {
                        coords.offset_right--;
                    }
                }
            }
            else if (next1 == '[' && next2 == 'B')
            {
                int offset_to_increment = active ? coords.offset_left : coords.offset_right; // вниз
                int quantity_lines = (active ? coords.quantity_lines_left : coords.quantity_lines_right);
                int hidden_row = quantity_lines - coords.cursor_y;
                if (coords.cursor_y < quantity_lines && coords.cursor_y < coords.height_win - 4)
                {
                    coords.cursor_y++;
                }
                else if ((coords.cursor_y >= coords.height_win - 4) && hidden_row > offset_to_increment)
                {
                    active ? coords.offset_left++ : coords.offset_right++;
                }
            }
            else if (next1 == '[' && next2 == 'C')
            {
                int offset_to_increment = active ? coords.offset_left : coords.offset_right;
                int quantity_lines = (active ? coords.quantity_lines_left : coords.quantity_lines_right);
                int hidden_row = quantity_lines - (coords.height_win - 4) - offset_to_increment;
                if (quantity_lines <= coords.height_win - 4)
                {
                    coords.cursor_y = quantity_lines;
                }
                else
                {
                    if (active)
                    {
                        coords.offset_left += hidden_row;
                    }
                    else
                    {
                        coords.offset_right += hidden_row;
                    }
                    coords.cursor_y = coords.height_win - 4;
                }
            }
            else if (next1 == '[' && next2 == 'D')
            {
                int offset_to_increment = active ? coords.offset_left : coords.offset_right;

                if (active)
                {
                    coords.offset_left = 0;
                }
                else
                {
                    coords.offset_right = 0;
                }
                coords.cursor_y = 1;
            }
        }
        else if (ch == 127 || ch == KEY_BACKSPACE)
        {
            if (active)
            {
                // click_on_file(ptr_user_data->left_path, all_files_left, &coords, previous_path_left, check_side);
                backspace(ptr_user_data->left_path, all_files_left, &coords, previous_path_left, check_side);
            }
            else
            {
                // click_on_file(ptr_user_data->right_path, all_files_right, &coords, previous_path_right, !check_side);
                backspace(ptr_user_data->right_path, all_files_right, &coords, previous_path_right, !check_side);
            }
        }
        else if (ch == 16)
        { // Ctrl + p - переход на сохраненный ранее путь
            // else if (ch == 'p') {                                      // Ctrl + p - переход на сохраненный ранее путь
            if (active)
            {
                char temp[1024];
                strcpy(temp, ptr_user_data->left_path);
                strcpy(ptr_user_data->left_path, previous_path_left);
                strcpy(previous_path_left, temp);
            }
            else
            {
                char temp[1024];
                strcpy(temp, ptr_user_data->right_path);
                strcpy(ptr_user_data->right_path, previous_path_right);
                strcpy(previous_path_right, temp);
            }
        }
        else if (ch == ('V' & 0x1F))
        { // Обработка нажатия Ctrl + V
            // // else if (ch == 'v') {                            // Обработка нажатия Ctrl + V

            if (active)
            {
                open_in_vim(ptr_user_data->left_path, all_files_left, &coords, check_side, win_left);
            }
            else
            {
                open_in_vim(ptr_user_data->right_path, all_files_right, &coords, !check_side, win_right);
            }
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
        // else if (ch == 1) {                                           // ctrl + a   терминал, приостановлено
        // // else if (ch == 'a') {
        //     set_bool.command_bool = !set_bool.command_bool;
        // }
        else if (ch == 'w')
        { // ctrl + a   терминал, приостановлено
            int quantity_lines = active ? coords.quantity_lines_left : coords.quantity_lines_right;

            set_bool.hidden_files_bool = !set_bool.hidden_files_bool;

            coords.offset_left = 0;
            coords.offset_right = 0;
            coords.cursor_y = 1;
        }
    }

    endwin();
    free(ptr_user_data);
    free(all_files_left);
    free(all_files_right);
    return 0;
}
