#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <libgen.h>

#include <ncurses.h>
#include <locale.h>
#include <signal.h>

#include "header.h"

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

    _Bool active = 1;
    _Bool check_side = 1;
    int cursor_left = 1;
    int cursor_right = 1;
    
    user_data *ptr_user_data = malloc(sizeof(user_data));
    file_data *all_files_left = (file_data *)malloc(500 * sizeof(file_data));
    file_data *all_files_right = (file_data *)malloc(500 * sizeof(file_data));
    int res_check = check_func(ptr_user_data);

    ptr_user_data->set_bool.hidden_left_bool = 1;
    ptr_user_data->set_bool.hidden_right_bool = 1;
    ptr_user_data->set_bool.command_bool = 0;
    ptr_user_data->set_bool.menu_bool = 0;
    ptr_user_data->set_bool.save_path_bool = 0;
    ptr_user_data->set_bool.create_bool = 0;
    ptr_user_data->set_bool.enter_name_bool = 0;
    ptr_user_data->set_bool.help_bool = 0;
    ptr_user_data->set_bool.out_bool = 0;
    ptr_user_data->set_bool.restore_files = 0;
    ptr_user_data->set_bool.move_files = 0;
    ptr_user_data->set_bool.copy_files = 0;
    ptr_user_data->set_bool.delete_files = 0;
    ptr_user_data->set_bool.check_active_tab_bool = 0;

    ptr_user_data->coordinates.height = 0;
    ptr_user_data->coordinates.width = 0;
    ptr_user_data->coordinates.height_win = 0;
    ptr_user_data->coordinates.width_win = 0;
    ptr_user_data->coordinates.height_menu = 0;

    ptr_user_data->coordinates.width_menu = 0;
    ptr_user_data->coordinates.cursor_x = 1;
    ptr_user_data->coordinates.cursor_y = 1;
    ptr_user_data->coordinates.cursor_left = 1;
    ptr_user_data->coordinates.cursor_right = 1;
    ptr_user_data->coordinates.quantity_lines_left = 0;
    ptr_user_data->coordinates.quantity_lines_right = 0;
    ptr_user_data->coordinates.leng_arr_coorsor = 0;
    ptr_user_data->coordinates.offset_left = 0;
    ptr_user_data->coordinates.offset_right = 0;
    ptr_user_data->coordinates.coords_cursor_y_menu = 3;

    int arr_coorsor[MAX_ARR_SIZE] = {0};                                                                                    // Инициализация массива
    ptr_user_data->arr_coorsor_struct.size = sizeof(arr_coorsor) / sizeof(*arr_coorsor);                                   // Размер массива
    memcpy(ptr_user_data->arr_coorsor_struct.arr, arr_coorsor, ptr_user_data->arr_coorsor_struct.size * sizeof(int));     // Копирование массива в структуру

    strncpy(ptr_user_data->save_paths.save_paths_arr[0], ptr_user_data->home_path, MAX_PATH_LENGTH - 1);
    ptr_user_data->save_paths.save_paths_arr[0][MAX_PATH_LENGTH - 1] = '\0';
    strncpy(ptr_user_data->save_paths.save_paths_arr[1], ptr_user_data->trash_directory, MAX_PATH_LENGTH - 1);
    ptr_user_data->save_paths.save_paths_arr[0][MAX_PATH_LENGTH - 1] = '\0';
    // strncpy(ptr_user_data->save_paths.save_paths_arr[2], "/home/dm/XXXXX/lint_to_d", MAX_PATH_LENGTH - 1);
    // ptr_user_data->save_paths.save_paths_arr[0][MAX_PATH_LENGTH - 1] = '\0';
    for(int i = 2; i < MAX_ARR_SIZE_SAVE_PATH; ++i){
        strncpy(ptr_user_data->save_paths.save_paths_arr[i], "/", MAX_PATH_LENGTH - 1);
        ptr_user_data->save_paths.save_paths_arr[i][MAX_PATH_LENGTH - 1] = '\0';
    }




    while (1)
    {
        file_data *all_files_ptr = active ? all_files_left : all_files_right;
        int *offset = active ? &(ptr_user_data->coordinates.offset_left) : &(ptr_user_data->coordinates.offset_right);
        int *quantity_lines = active ? &(ptr_user_data->coordinates.quantity_lines_left) : &(ptr_user_data->coordinates.quantity_lines_right);
        int *height = &ptr_user_data->coordinates.height;
        int *width = &ptr_user_data->coordinates.width;
        int *height_win = &ptr_user_data->coordinates.height_win;
        int *cursor_y = &ptr_user_data->coordinates.cursor_y;
        int *leng_arr_coorsor = &ptr_user_data->coordinates.leng_arr_coorsor;
        int *cursor_left = &ptr_user_data->coordinates.cursor_left;
        int *cursor_right = &ptr_user_data->coordinates.cursor_right;

        int *arr_coorsor_coorsor = ptr_user_data->arr_coorsor_struct.arr;
        size_t *arr_coorsor_size = &ptr_user_data->arr_coorsor_struct.size;

        _Bool *command_bool = &ptr_user_data->set_bool.command_bool;
        _Bool *help_bool = &ptr_user_data->set_bool.help_bool;
        _Bool *menu_bool = &ptr_user_data->set_bool.menu_bool;
        _Bool *save_path_bool = &ptr_user_data->set_bool.save_path_bool;
        _Bool *create_bool = &ptr_user_data->set_bool.create_bool;
        _Bool *out_bool = &ptr_user_data->set_bool.out_bool;
        _Bool *hidden_left_bool = &ptr_user_data->set_bool.hidden_left_bool;
        _Bool *hidden_right_bool = &ptr_user_data->set_bool.hidden_right_bool;
        _Bool *check_active_tab_bool = &ptr_user_data->set_bool.check_active_tab_bool;

        int item = *cursor_y + *offset - 1;

        removeDuplicates(arr_coorsor_coorsor, *arr_coorsor_size);
        active ? strcpy(ptr_user_data->coorsor_file, all_files_left[item].name) : strcpy(ptr_user_data->coorsor_file, all_files_right[item].name);
        size_t leng_path = active ? strlen(ptr_user_data->left_path) + strlen(ptr_user_data->coorsor_file) + 4 : strlen(ptr_user_data->right_path) + strlen(ptr_user_data->coorsor_file) + 4;
        size_t width_menu = leng_path < *width / 3 ? *width / 3 : leng_path;

        getmaxyx(stdscr, *height, *width);
        win_left = newwin(*height, *width / 2, 0, 0);
        win_right = newwin(*height, *width % 2 ? (*width / 2) + 1 : *width / 2, 0, *width / 2);
        win_menu = newwin(10, width_menu, (*height / 2) - 5, *width / 2 - width_menu / 2);

        if (!*command_bool && !*help_bool && !*menu_bool && !*save_path_bool && !*create_bool) {
            if (active) {
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
            } else {
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
            }
        } else if (*command_bool) {
            if (active) {
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
                render_comm_line(ptr_user_data, all_files_right, active, check_side, win_left, win_right);
            } else {
                render_comm_line(ptr_user_data, all_files_right, active, check_side, win_left, win_right);
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
            }

            if (active && !*command_bool) {
                continue;
            }
        }
        else if (*help_bool) {
            if (active) {
                render_help(ptr_user_data, all_files_right, !active, win_right);
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
            } else {
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
                render_help(ptr_user_data, all_files_right, !active, win_right);
            }
        } 
        else if (*menu_bool) {
            _Bool turn_render_ls = true;
            if (active) {
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
                render_menu(ptr_user_data, all_files_left, all_files_right, active, check_side, turn_render_ls, win_menu, win_right, win_left);
            } else {
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
                render_menu(ptr_user_data, all_files_left, all_files_right, active, check_side, !turn_render_ls, win_menu, win_right, win_left);
            }
            if (*out_bool) {
                break;
            } else if(*check_active_tab_bool){
                active = !active;
                *check_active_tab_bool = false;
            }
            if (!*menu_bool) {
                continue;
            }
        }
        else if (*save_path_bool) {
            _Bool turn_render_ls = true;
            if (active) {
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
                render_save_path(ptr_user_data, all_files_left, all_files_right, active, check_side, turn_render_ls, win_menu, win_right, win_left);
            } else {
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
                render_save_path(ptr_user_data, all_files_left, all_files_right, active, check_side, !turn_render_ls, win_menu, win_right, win_left);
            }
            if (*out_bool) {
                break;
            } else if(*check_active_tab_bool){
                active = !active;
                *check_active_tab_bool = false;
            } 
            if (!*save_path_bool) {
                continue;
            }
        }
        else if (*create_bool) {
            _Bool turn_render_ls = true;
            if (active) {
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
                render_create(ptr_user_data, all_files_left, all_files_right, active, check_side, turn_render_ls, win_menu, win_right, win_left);
            } else {
                render_ls(ptr_user_data, all_files_left, active, check_side, win_left);
                render_ls(ptr_user_data, all_files_right, !active, !check_side, win_right);
                render_create(ptr_user_data, all_files_left, all_files_right, active, check_side, !turn_render_ls, win_menu, win_right, win_left);
            }
            if (*out_bool) {
                break;
            } else if(*check_active_tab_bool){
                active = !active;
                *check_active_tab_bool = false;
            }
            if (!*create_bool) {
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
        // else if (ch == 'r' || ch == KEY_RESIZE) {
            int new_height, new_width;
            getmaxyx(stdscr, new_height, new_width);
            int cursor_position = all_files_ptr[item].file_id;
            if (new_height > *height) {
                if (*offset > 0) {
                    (*offset)--;
                    *cursor_y++;
                }
            } else if(new_height < *height){
                if (*offset <= 0 && *cursor_y == *height_win - 4) {
                    (*offset)++;
                    *cursor_y = cursor_position - *offset + 1;
                } else if (*offset > 0 && *cursor_y == *height_win - 4) {
                    (*offset)++;
                    *cursor_y = cursor_position - *offset + 1;
                } 
            } 
        }
        else if (ch == '\t') {
            if (active) {
                *cursor_left = *cursor_y;
            } else {
                *cursor_right = *cursor_y;
            }
            active = !active;
            fillWithZeros(arr_coorsor_coorsor, *arr_coorsor_size);                   // очистить массив с отметками строк
            *leng_arr_coorsor = 0;
            if (active) {
                if (*cursor_left <= (*height_win - 4)) {
                    *cursor_y = *cursor_left;
                } else {
                    *cursor_y = *height_win - 4;
                }
            } else {
                if (*cursor_right <= (*height_win - 4)) {
                    *cursor_y = *cursor_right;
                } else {
                    *cursor_y = *height_win - 4;
                }
            }
        }
        else if (ch == '\n') {
            if (active) {
                click_on_file(ptr_user_data, all_files_left, active, check_side);
                fillWithZeros(arr_coorsor_coorsor, *arr_coorsor_size);                   // очистить массив с отметками строк
            } else {
                click_on_file(ptr_user_data, all_files_right, !active, !check_side);
                fillWithZeros(arr_coorsor_coorsor, *arr_coorsor_size);                   // очистить массив с отметками строк
            }
            *leng_arr_coorsor = 0;
        }
        else if (ch == 27) {
            int next1 = wgetch(stdscr);
            int next2 = wgetch(stdscr);
            if (next1 == '[' && next2 == 'A') {
                int hidden_row = *quantity_lines - *cursor_y;
                if (*cursor_y > 1) {
                    (*cursor_y)--;
                } else if (*cursor_y == 1 && *offset > 0) {
                    (*offset)--;
                }
            }
            else if (next1 == '[' && next2 == 'B') {
                int hidden_row = *quantity_lines - *cursor_y;
                if (*cursor_y < *quantity_lines && *cursor_y < *height_win - 4) {
                    (*cursor_y)++;
                } else if ((*cursor_y >= *height_win - 4) && hidden_row > *offset) {
                    (*offset)++;
                }
            } else if (next1 == '[' && next2 == 'C') {
                int hidden_row = *quantity_lines - (*height_win - 4) - *offset;
                if (*quantity_lines <= *height_win - 4) {
                    *cursor_y = *quantity_lines;
                } 
                else {
                    *offset += hidden_row;
                    *cursor_y = *height_win - 4;
                }
            }
            else if (next1 == '[' && next2 == 'D') {
                *offset = 0;
                *cursor_y = 1;
            }
        }
        else if (ch == 127 || ch == KEY_BACKSPACE) {
            if (active) {
                backspace(ptr_user_data, all_files_left, active, check_side);
                fillWithZeros(arr_coorsor_coorsor, *arr_coorsor_size);                   // очистить массив с отметками строк
            } else {
                backspace(ptr_user_data, all_files_right, !active, !check_side);
                fillWithZeros(arr_coorsor_coorsor, *arr_coorsor_size);                   // очистить массив с отметками строк
            }
            *leng_arr_coorsor = 0;
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
            fillWithZeros(arr_coorsor_coorsor, *arr_coorsor_size);                   // очистить массив с отметками строк
            *leng_arr_coorsor = 0;
        }
        else if (ch == ('V' & 0x1F) || ch == 'v') { // Обработка нажатия Ctrl + V
            // // else if (ch == 'v') {                            // Обработка нажатия Ctrl + V

            if (active) {
                open_in_vim(ptr_user_data, all_files_left, check_side, win_left);
            } else {
                open_in_vim(ptr_user_data, all_files_right, !check_side, win_right);
            }
            fillWithZeros(arr_coorsor_coorsor, *arr_coorsor_size);                   // очистить массив с отметками строк
            *leng_arr_coorsor = 0;
        }

        else if (ch == '\b' || ch == 'h') { // ctrl + h  // help_bool
            // else if (ch == 'a') {
             *help_bool = !*help_bool;
        }
        else if (ch == 'm') { // ctrl + h  // help_bool
            // else if (ch == 'a') {
            *menu_bool = !*menu_bool;
        }
        else if (ch == 'p') { // ctrl + h  // help_bool
            // else if (ch == 'a') {
            *save_path_bool = !*save_path_bool;
        }
        else if (ch == 'c') { // ctrl + h  // help_bool
            // else if (ch == 'a') {
            *create_bool = !*create_bool;
        }
        // else if (ch == 1) {                                           // ch == 1 -> ctrl + a   терминал, приостановлено
        // else if (ch == 'a') {
            // *command_bool = !*command_bool;
        // }
        else if (ch == 1 || ch == 'a') {                                           // ch == 1 -> ctrl + a   выделить все
            // _Bool contains = containsAnyElement(arr_coorsor_coorsor, *arr_coorsor_size, (*quantity_lines - 1));
                // if(contains) {
                    if ((*quantity_lines - 1) > *leng_arr_coorsor) {
                        addAllToArr(arr_coorsor_coorsor, *arr_coorsor_size, (*quantity_lines - 1));
                        *leng_arr_coorsor = *quantity_lines - 1;
                    } else {
                        fillWithZeros(arr_coorsor_coorsor, *arr_coorsor_size);
                        *leng_arr_coorsor = 0;
                    }
                // } else {
                    // if ((*quantity_lines - 1) == *leng_arr_coorsor) {
                    // }
                // }
        }
        else if (ch == 'w')
        { // ctrl + a   терминал, приостановлено
            if(active) {
                *hidden_left_bool = !*hidden_left_bool;
            } else {
                *hidden_right_bool = !*hidden_right_bool;
            }

            *offset = 0;
            *cursor_y = 1;
        }
        if (ch == ' ') {
            if (strcmp(ptr_user_data->coorsor_file, "..") != 0) {
                _Bool contains = containsElement(arr_coorsor_coorsor, *arr_coorsor_size, item);
                if(contains) {
                    if (*leng_arr_coorsor > 0) {
                        removeFromArr(arr_coorsor_coorsor, *arr_coorsor_size, item);
                        (*leng_arr_coorsor)--;
                    }
                } else {
                    if (*leng_arr_coorsor < *arr_coorsor_size - 1) {
                        addToArr(arr_coorsor_coorsor, *arr_coorsor_size, item);
                        (*leng_arr_coorsor)++;
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



