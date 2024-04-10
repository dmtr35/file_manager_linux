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

// #define MAX_ARR_SIZE 100

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
    
    struct user_data *ptr_user_data = malloc(sizeof(struct user_data));
    struct file_data *all_files_left = (struct file_data *)malloc(500 * sizeof(struct file_data));
    struct file_data *all_files_right = (struct file_data *)malloc(500 * sizeof(struct file_data));

    ptr_user_data->set_bool.hidden_left_bool = 1;
    ptr_user_data->set_bool.hidden_right_bool = 1;
    ptr_user_data->set_bool.command_bool = 0;
    ptr_user_data->set_bool.menu_bool = 0;
    ptr_user_data->set_bool.help_bool = 0;
    ptr_user_data->set_bool.out_bool = 0;
    ptr_user_data->set_bool.restore_files = 0;
    ptr_user_data->set_bool.move_files = 0;
    ptr_user_data->set_bool.copy_files = 0;
    ptr_user_data->set_bool.delete_files = 0;

    ptr_user_data->coordinates.height = 0;
    ptr_user_data->coordinates.width = 0;
    ptr_user_data->coordinates.height_win = 0;
    ptr_user_data->coordinates.width_win = 0;
    ptr_user_data->coordinates.height_menu = 0;
    ptr_user_data->coordinates.width_menu = 0;
    ptr_user_data->coordinates.cursor_x = 1;
    ptr_user_data->coordinates.cursor_y = 1;
    ptr_user_data->coordinates.quantity_lines_left = 0;
    ptr_user_data->coordinates.quantity_lines_right = 0;
    ptr_user_data->coordinates.leng_arr_coorsor = 0;
    ptr_user_data->coordinates.offset_left = 0;
    ptr_user_data->coordinates.offset_right = 0;

    int arr_coorsor[MAX_ARR_SIZE] = {0};                                                                                    // Инициализация массива
    ptr_user_data->arr_coorsor_struct.size = sizeof(arr_coorsor) / sizeof(*arr_coorsor);                                   // Размер массива
    memcpy(ptr_user_data->arr_coorsor_struct.arr, arr_coorsor, ptr_user_data->arr_coorsor_struct.size * sizeof(int));     // Копирование массива в структуру


    _Bool active = 1;
    _Bool check_side = 1;
    // char previous_path_left[1024];
    // char previous_path_right[1024];
    int cursor_left = 1;
    int cursor_right = 1;


    int res_check = check_func(ptr_user_data);
    strcpy(ptr_user_data->previous_path_left, ptr_user_data->left_path);
    strcpy(ptr_user_data->previous_path_right, ptr_user_data->right_path);

    while (1)
    {
        struct file_data *all_files_ptr = active ? all_files_left : all_files_right;
        int coords_cursor_y_menu = 3;
        int *offset = active ? &(ptr_user_data->coordinates.offset_left) : &(ptr_user_data->coordinates.offset_right);
        int *quantity_lines = active ? &(ptr_user_data->coordinates.quantity_lines_left) : &(ptr_user_data->coordinates.quantity_lines_right);
        int i = ptr_user_data->coordinates.cursor_y + *offset - 1;
        removeDuplicates(ptr_user_data->arr_coorsor_struct.arr, ptr_user_data->arr_coorsor_struct.size);
        active ? strcpy(ptr_user_data->coorsor_file, all_files_left[i].name) : strcpy(ptr_user_data->coorsor_file, all_files_right[i].name);
        size_t leng_path = active ? strlen(ptr_user_data->left_path) + strlen(ptr_user_data->coorsor_file) + 4 : strlen(ptr_user_data->right_path) + strlen(ptr_user_data->coorsor_file) + 4;
        size_t width_menu = leng_path < ptr_user_data->coordinates.width / 3 ? ptr_user_data->coordinates.width / 3 : leng_path;

        getmaxyx(stdscr, ptr_user_data->coordinates.height, ptr_user_data->coordinates.width);
        win_left = newwin(ptr_user_data->coordinates.height, ptr_user_data->coordinates.width / 2, 0, 0);
        win_right = newwin(ptr_user_data->coordinates.height, ptr_user_data->coordinates.width % 2 ? (ptr_user_data->coordinates.width / 2) + 1 : ptr_user_data->coordinates.width / 2, 0, ptr_user_data->coordinates.width / 2);
        win_menu = newwin(10, width_menu, (ptr_user_data->coordinates.height / 2) - 5, ptr_user_data->coordinates.width / 2 - width_menu / 2);

        if (!ptr_user_data->set_bool.command_bool && !ptr_user_data->set_bool.help_bool && !ptr_user_data->set_bool.menu_bool)
        {
            if (active)
            {
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);

                // render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                // render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
            }
            else
            {
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
                // render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                // render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
            }
        }
        else if (ptr_user_data->set_bool.command_bool)
        {
            if (active)
            {
                // render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                // render_comm_line(ptr_user_data, all_files_right, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left, win_right);
            }
            else
            {
                // render_comm_line(ptr_user_data, all_files_right, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left, win_right);
                // render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
            }

            if (active && !ptr_user_data->set_bool.command_bool)
            {
                continue;
            }
        }
        else if (ptr_user_data->set_bool.help_bool)
        {
            if (active)
            {
                // render_help(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, arr_coorsor, leng_arr_coorsor_full, win_right);
                // render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
            }
            else
            {
                // render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                // render_help(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, arr_coorsor, leng_arr_coorsor_full, win_right);
            }
        }
        else if (ptr_user_data->set_bool.menu_bool)
        {
            _Bool turn_render_ls = true;
            if (active)
            {
                // render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                // render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                // render_menu(ptr_user_data, all_files_left, all_files_right, &set_bool, &coords_cursor_y_menu, &coords, active, check_side, turn_render_ls, arr_coorsor, leng_arr_coorsor_full, win_menu, win_right, win_left);
            }
            else
            {
                // render_ls(ptr_user_data->left_path, all_files_left, &coords, &set_bool, active, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                // render_ls(ptr_user_data->right_path, all_files_right, &coords, &set_bool, !active, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                // render_menu(ptr_user_data, all_files_left, all_files_right, &set_bool, &coords_cursor_y_menu, &coords, active, check_side, !turn_render_ls, arr_coorsor, leng_arr_coorsor_full, win_menu, win_right, win_left);
            }
            if (ptr_user_data->set_bool.out_bool) {
                break;
            } if (!ptr_user_data->set_bool.menu_bool) {
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
            int cursor_position = all_files_ptr[i].file_id;
            if (new_height > ptr_user_data->coordinates.height) {
                if (*offset > 0) {
                    (*offset)--;
                    ptr_user_data->coordinates.cursor_y++;
                }
            } else if(new_height < ptr_user_data->coordinates.height){
                if (*offset <= 0 && ptr_user_data->coordinates.cursor_y == ptr_user_data->coordinates.height_win - 4) {
                    (*offset)++;
                    ptr_user_data->coordinates.cursor_y = cursor_position - *offset + 1;
                } else if (*offset > 0 && ptr_user_data->coordinates.cursor_y == ptr_user_data->coordinates.height_win - 4) {
                    (*offset)++;
                    ptr_user_data->coordinates.cursor_y = cursor_position - *offset + 1;
                } 
            } 
        }
        else if (ch == '\t') {
            if (active) {
                cursor_left = ptr_user_data->coordinates.cursor_y;
            } else {
                cursor_right = ptr_user_data->coordinates.cursor_y;
            }
            active = !active;
            fillWithZeros(ptr_user_data);                   // очистить массив с отметками строк
            if (active) {
                if (cursor_left <= (ptr_user_data->coordinates.height_win - 4)) {
                    ptr_user_data->coordinates.cursor_y = cursor_left;
                } else {
                    ptr_user_data->coordinates.cursor_y = ptr_user_data->coordinates.height_win - 4;
                }
            } else {
                if (cursor_right <= (ptr_user_data->coordinates.height_win - 4)) {
                    ptr_user_data->coordinates.cursor_y = cursor_right;
                } else {
                    ptr_user_data->coordinates.cursor_y = ptr_user_data->coordinates.height_win - 4;
                }
            }
        }
        else if (ch == '\n') {
            if (active) {
                click_on_file(ptr_user_data, all_files_left, active, check_side);
                fillWithZeros(ptr_user_data);                   // очистить массив с отметками строк
            } else {
                click_on_file(ptr_user_data, all_files_right, !active, !check_side);
                fillWithZeros(ptr_user_data);                   // очистить массив с отметками строк
            }
        }
        else if (ch == 27) {
            int next1 = wgetch(stdscr);
            int next2 = wgetch(stdscr);
            if (next1 == '[' && next2 == 'A') {
                int hidden_row = *quantity_lines - ptr_user_data->coordinates.cursor_y;
                if (ptr_user_data->coordinates.cursor_y > 1) {
                    ptr_user_data->coordinates.cursor_y--;
                } else if (ptr_user_data->coordinates.cursor_y == 1 && *offset > 0) {
                    (*offset)--;
                }
            }
            else if (next1 == '[' && next2 == 'B') {
                int hidden_row = *quantity_lines - ptr_user_data->coordinates.cursor_y;
                if (ptr_user_data->coordinates.cursor_y < *quantity_lines && ptr_user_data->coordinates.cursor_y < ptr_user_data->coordinates.height_win - 4) {
                    ptr_user_data->coordinates.cursor_y++;
                } else if ((ptr_user_data->coordinates.cursor_y >= ptr_user_data->coordinates.height_win - 4) && hidden_row > *offset) {
                    (*offset)++;
                }
            } else if (next1 == '[' && next2 == 'C') {
                int hidden_row = *quantity_lines - (ptr_user_data->coordinates.height_win - 4) - *offset;
                if (*quantity_lines <= ptr_user_data->coordinates.height_win - 4) {
                    ptr_user_data->coordinates.cursor_y = *quantity_lines;
                } 
                else {
                    *offset += hidden_row;
                    ptr_user_data->coordinates.cursor_y = ptr_user_data->coordinates.height_win - 4;
                }
            }
            else if (next1 == '[' && next2 == 'D') {
                *offset = 0;
                ptr_user_data->coordinates.cursor_y = 1;
            }
        }
        else if (ch == 127 || ch == KEY_BACKSPACE) {
            if (active) {
                backspace(ptr_user_data, all_files_left, active, check_side);
                fillWithZeros(ptr_user_data);                   // очистить массив с отметками строк
            } else {
                backspace(ptr_user_data, all_files_right, !active, !check_side);
                fillWithZeros(ptr_user_data);                   // очистить массив с отметками строк
            }
        }
        else if (ch == 16) { // Ctrl + p - переход на сохраненный ранее путь
            // else if (ch == 'p') {                                      // Ctrl + p - переход на сохраненный ранее путь
            if (active) {
                char temp[1024];
                strcpy(temp, ptr_user_data->left_path);
                strcpy(ptr_user_data->left_path, ptr_user_data->previous_path_left);
                strcpy(ptr_user_data->previous_path_left, temp);
            }
            else {
                char temp[1024];
                strcpy(temp, ptr_user_data->right_path);
                strcpy(ptr_user_data->right_path, ptr_user_data->previous_path_right);
                strcpy(ptr_user_data->previous_path_right, temp);
            }
            fillWithZeros(ptr_user_data);                   // очистить массив с отметками строк
        }
        else if (ch == ('V' & 0x1F)) { // Обработка нажатия Ctrl + V
            // // else if (ch == 'v') {                            // Обработка нажатия Ctrl + V

            if (active) {
                open_in_vim(ptr_user_data, all_files_left, active, check_side, win_left);
            } else {
                open_in_vim(ptr_user_data, all_files_right, !active, !check_side, win_right);
            }
            fillWithZeros(ptr_user_data);                   // очистить массив с отметками строк
        }

        else if (ch == '\b' || ch == 'h')
        { // ctrl + h  // help_bool
            // else if (ch == 'a') {
             ptr_user_data->set_bool.help_bool = !ptr_user_data->set_bool.help_bool;
        }
        else if (ch == 'm')
        { // ctrl + h  // help_bool
            // else if (ch == 'a') {
            ptr_user_data->set_bool.menu_bool = !ptr_user_data->set_bool.menu_bool;
        }
        // else if (ch == 1) {                                           // ch == 1 -> ctrl + a   терминал, приостановлено
        // // else if (ch == 'a') {
        //     set_bool.command_bool = !set_bool.command_bool;
        // }
        else if (ch == 'w')
        { // ctrl + a   терминал, приостановлено
            if(active) {
                ptr_user_data->set_bool.hidden_left_bool = !ptr_user_data->set_bool.hidden_left_bool;
            } else {
                ptr_user_data->set_bool.hidden_right_bool = !ptr_user_data->set_bool.hidden_right_bool;
            }

            *offset = 0;
            ptr_user_data->coordinates.cursor_y = 1;
        }
        if (ch == ' ') {
            if (strcmp(ptr_user_data->coorsor_file, "..") != 0) {
                bool contains = containsElement(ptr_user_data->arr_coorsor_struct.arr, ptr_user_data->arr_coorsor_struct.size, i);
                if(contains) {
                    if (ptr_user_data->coordinates.leng_arr_coorsor > 0) {
                        removeFromArr(ptr_user_data->arr_coorsor_struct.arr, ptr_user_data->arr_coorsor_struct.size, i);
                        ptr_user_data->coordinates.leng_arr_coorsor--;
                    }
                } else {
                    if (ptr_user_data->coordinates.leng_arr_coorsor < ptr_user_data->arr_coorsor_struct.size - 1) {
                        addToArr(ptr_user_data->arr_coorsor_struct.arr, ptr_user_data->arr_coorsor_struct.size, i);
                        ptr_user_data->coordinates.leng_arr_coorsor++;
                    }
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



