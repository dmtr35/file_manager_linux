#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/time.h>


#include <ncurses.h>
#include <signal.h>

#include "../header.h"

void render_create(user_data *ptr_user_data, file_data *all_files_left, file_data *all_files_right, _Bool active, _Bool check_side, _Bool turn_render_ls, WINDOW *win_menu, WINDOW *win_right, WINDOW *win_left)
{
    start_color();

    init_pair(21, COLOR_WHITE, COLOR_CYAN); // Цветовая пара 2: Красный текст на темносинем фоне       // link 
    init_pair(22, COLOR_YELLOW, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder

    init_pair(15, COLOR_RED, COLOR_WHITE); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(2, COLOR_CYAN, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне.       // files // hide folder
    init_pair(4, COLOR_GREEN, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне       // ex      
    init_pair(5, COLOR_RED, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне.        // zip
    init_pair(6, COLOR_BLACK, COLOR_WHITE);

    init_pair(11, COLOR_WHITE, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(12, COLOR_CYAN, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(14, COLOR_GREEN, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(15, COLOR_RED, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(16, COLOR_BLACK, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    wbkgd(win_menu, COLOR_PAIR(21));

    int height_win;
    int width_win;
    char screen_buffer[64] = {0};
    int buffer_pos = 0;

    char *path = active ? ptr_user_data->left_path : ptr_user_data->right_path;


    // int check_empty = check_int_arr(ptr_user_data->arr_coorsor_struct.arr, ptr_user_data->arr_coorsor_struct.size);
    int count_item_arr = count_non_zero_elements(ptr_user_data->arr_coorsor_struct.arr, ptr_user_data->arr_coorsor_struct.size);
    int *offset = active ? &ptr_user_data->coordinates.offset_left : &ptr_user_data->coordinates.offset_right;
    int *quantity_lines = active ? &ptr_user_data->coordinates.quantity_lines_left : &ptr_user_data->coordinates.quantity_lines_right;
    int *height = &ptr_user_data->coordinates.height;
    int *width = &ptr_user_data->coordinates.width;

    char *file_name = ptr_user_data->coorsor_file;
    char *right_path = ptr_user_data->right_path;
    char *left_path = ptr_user_data->left_path;
    char *trash_directory = ptr_user_data->trash_directory;
    
    _Bool *menu_bool = &ptr_user_data->set_bool.menu_bool;
    _Bool *save_path_bool = &ptr_user_data->set_bool.save_path_bool;
    _Bool *create_bool = &ptr_user_data->set_bool.create_bool;
    _Bool *enter_name_bool = &ptr_user_data->set_bool.enter_name_bool;
    _Bool *check_active_tab_bool = &ptr_user_data->set_bool.check_active_tab_bool;

    _Bool *out_bool = &ptr_user_data->set_bool.out_bool;
    _Bool *delete_files = &ptr_user_data->set_bool.delete_files;
    _Bool *restore_files = &ptr_user_data->set_bool.restore_files;
    _Bool *copy_files = &ptr_user_data->set_bool.copy_files;
    _Bool *move_files = &ptr_user_data->set_bool.move_files;

    int *arr_coorsor_coorsor = ptr_user_data->arr_coorsor_struct.arr;
    size_t *arr_coorsor_size = &ptr_user_data->arr_coorsor_struct.size;
    int *cursor_y = &ptr_user_data->coordinates.cursor_y;
    int *leng_arr_coorsor = &ptr_user_data->coordinates.leng_arr_coorsor;
    int *cursor_left = &ptr_user_data->coordinates.cursor_left;
    int *cursor_right = &ptr_user_data->coordinates.cursor_right;

    file_data *all_files_ptr = active ? all_files_left : all_files_right;
    char *name_file_row = all_files_ptr[*cursor_y - 1].name;
    
    size_t size_new_path = strlen(path) + strlen(file_name) + 3;
    char new_path[size_new_path];
    _Bool is_enter_pressed = true;
    _Bool save_files = 0;

    while (is_enter_pressed) {
        int *coords_cursor_y_menu = &(ptr_user_data->coordinates.coords_cursor_y_menu);

        getmaxyx(win_menu, height_win, width_win);
        size_t leng_path = active ? strlen(left_path) + strlen(file_name) + 4 : strlen(right_path) + strlen(file_name) + 4;
        size_t width_menu = leng_path < *width / 3 ? *width / 3 : leng_path;
        win_menu = newwin(10, width_menu, (*height / 2) - 5, *width / 2 - width_menu / 2);

        wborder(win_menu, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
        snprintf(new_path, size_new_path, "%s/%s", path, file_name);

        int offset_menu = 0;
        int i = 0 + offset_menu;
        int row = 3;
        for (; i < 5 && row <= 7; ++i, ++row) {
            if (row == *coords_cursor_y_menu) {
                wattron(win_menu, A_BOLD);
                wattron(win_menu, COLOR_PAIR(22));                       // Включаем цветовую пару для всей строки
                mvwhline(win_menu, row, 1, ' ', width_win - 2); // Заполняем строку пробелами для очистки ее содержимого

                mvwprintw(win_menu, 1, 1, "%s", new_path);
                mvwprintw(win_menu, 3, width_win / 2 - 8, "create directory");
                mvwprintw(win_menu, 4, width_win / 2 - 6, "create touch");
                mvwprintw(win_menu, 5, width_win / 2 - 6, "create link");
                mvwprintw(win_menu, 6, width_win / 2 - 3, "rename");
                mvwprintw(win_menu, 7, width_win / 2 - 5, "permission");

                if (*enter_name_bool) {
                    mvwprintw(win_menu, 8, 1, "Enter name: %s", screen_buffer);
                }

                wattroff(win_menu, COLOR_PAIR(22)); // Отключаем цветовую пару
                wattroff(win_menu, A_BOLD);

            } 
    }

        int ch = wgetch(win_menu);

        

        if (ch != ERR) {
            if (ch == 1 || ch == 'c') {                 
                is_enter_pressed = false;
                *create_bool = false;
                *coords_cursor_y_menu = 3;
                *enter_name_bool = false;
            } 
            else if (ch == 1 || ch == 'p') {                 
                is_enter_pressed = false;
                *create_bool = false;
                *save_path_bool = true;
                *coords_cursor_y_menu = 3;
                *enter_name_bool = false;
            } 
            else if (ch == 1 || ch == 'm') {                 
                is_enter_pressed = false;
                *create_bool = false;
                *menu_bool = true;
                *coords_cursor_y_menu = 3;
                *enter_name_bool = false;
            } 
            else if (ch == '\t') {
                if (active) {
                    *cursor_left = *cursor_y;
                } else {
                    *cursor_right = *cursor_y;
                }
                *check_active_tab_bool = true;
                fillWithZeros(arr_coorsor_coorsor, *arr_coorsor_size);                   // очистить массив с отметками строк
                *leng_arr_coorsor = 0;
                if (active) {
                    if (*cursor_left <= (height_win - 4)) {
                        *cursor_y = *cursor_left;
                    } else {
                        *cursor_y = height_win - 4;
                    }
                } else {
                    if (*cursor_right <= (height_win - 4)) {
                        *cursor_y = *cursor_right;
                    } else {
                        *cursor_y = height_win - 4;
                    }
                }
                is_enter_pressed = false;
                *create_bool = false;
                *coords_cursor_y_menu = 3;
            } 
            else if (ch == 'q') {
                is_enter_pressed = false;
                *create_bool = false;
                *out_bool = true;
                break;
            }
            else if (ch == '\n')
            {
                if (*coords_cursor_y_menu == 3) {                                   // create dir
                    if(buffer_pos){
                        char *new_file_name = screen_buffer;
                        mkdir_p(new_file_name, path);
                        
                        is_enter_pressed = false;
                        *create_bool = false;
                        *coords_cursor_y_menu = 3;
                        *enter_name_bool = false;
                    } else {
                        *enter_name_bool = true;
                        render_create(ptr_user_data, all_files_left, all_files_right, active, check_side, turn_render_ls, win_menu, win_right, win_left);
                    }
                } else if (*coords_cursor_y_menu == 4) {                            // create touch
                    if(buffer_pos){
                        char *new_file_name = screen_buffer;
                        touch_file(new_file_name, path);
                        
                        is_enter_pressed = false;
                        *create_bool = false;
                        *coords_cursor_y_menu = 3;
                        *enter_name_bool = false;
                    } else {
                        *enter_name_bool = true;
                        render_create(ptr_user_data, all_files_left, all_files_right, active, check_side, turn_render_ls, win_menu, win_right, win_left);
                    }
                } else if (*coords_cursor_y_menu == 5) {                            // create link
                    if(buffer_pos){
                        char *new_file_name = screen_buffer;
                        
                        size_t size_path_to_file_name = strlen(path) + strlen(name_file_row) + 2;
                        char *path_to_file_name = malloc(size_path_to_file_name * sizeof(char));
                        snprintf(path_to_file_name, size_path_to_file_name, "%s/%s", path, name_file_row);

                        create_link(new_file_name, path, path_to_file_name);
                        
                        free(path_to_file_name);
                        
                        is_enter_pressed = false;
                        *create_bool = false;
                        *coords_cursor_y_menu = 3;
                        *enter_name_bool = false;
                    } else {
                        *enter_name_bool = true;
                        render_create(ptr_user_data, all_files_left, all_files_right, active, check_side, turn_render_ls, win_menu, win_right, win_left);
                    }
                } else if (*coords_cursor_y_menu == 6) {                            // rename
                    if(buffer_pos){
                        char *new_file_name = screen_buffer;

                        rename_file(new_file_name, path, name_file_row);

                        is_enter_pressed = false;
                        *create_bool = false;
                        *coords_cursor_y_menu = 3;
                        *enter_name_bool = false;
                    } else {
                        *enter_name_bool = true;
                        render_create(ptr_user_data, all_files_left, all_files_right, active, check_side, turn_render_ls, win_menu, win_right, win_left);
                    }
                } else if (*coords_cursor_y_menu == 7) {                            // permission
                    if(buffer_pos){
                        char *new_permission = screen_buffer;
                        change_permission(new_permission, path, name_file_row);

                        is_enter_pressed = false;
                        *create_bool = false;
                        *coords_cursor_y_menu = 3;
                        *enter_name_bool = false;
                    } else {
                        *enter_name_bool = true;
                        render_create(ptr_user_data, all_files_left, all_files_right, active, check_side, turn_render_ls, win_menu, win_right, win_left);
                    }

                }

                is_enter_pressed = false;
                *create_bool = false;
            } else if (ch == KEY_RESIZE) {
            // } else if (ch == 'r' || ch == KEY_RESIZE) {
                render_ls_and_create(ptr_user_data, all_files_left, all_files_right, turn_render_ls, active, check_side, &is_enter_pressed, coords_cursor_y_menu, win_menu, win_right, win_left);
            }
            else if (ch == 27) {
                int next1 = wgetch(stdscr);
                int next2 = wgetch(stdscr);
                if (next1 == '[' && next2 == 'A') {
                    if (*coords_cursor_y_menu > 3) {
                        (*coords_cursor_y_menu)--;
                    } else if (*coords_cursor_y_menu == 3) {
                        *coords_cursor_y_menu = 7;
                    }
                    *enter_name_bool = false;
                    render_ls_and_create(ptr_user_data, all_files_left, all_files_right, turn_render_ls, active, check_side, &is_enter_pressed, coords_cursor_y_menu, win_menu, win_right, win_left);
                } else if (next1 == '[' && next2 == 'B') {
                    if (*coords_cursor_y_menu < 7) {
                        (*coords_cursor_y_menu)++;
                    } else if (*coords_cursor_y_menu == 7) {
                        *coords_cursor_y_menu = 3;
                    }
                    *enter_name_bool = false;
                    render_ls_and_create(ptr_user_data, all_files_left, all_files_right, turn_render_ls, active, check_side, &is_enter_pressed, coords_cursor_y_menu, win_menu, win_right, win_left);
                } 
                else if (next1 == '[' && next2 == 'C') {                                      // -> на последнюю
                    *coords_cursor_y_menu = 7;
                    *enter_name_bool = false;
                    render_ls_and_create(ptr_user_data, all_files_left, all_files_right, turn_render_ls, active, check_side, &is_enter_pressed, coords_cursor_y_menu, win_menu, win_right, win_left);
                } else if (next1 == '[' && next2 == 'D') {                                    // <- на первую
                    *coords_cursor_y_menu = 3;
                    *enter_name_bool = false;
                    render_ls_and_create(ptr_user_data, all_files_left, all_files_right, turn_render_ls, active, check_side, &is_enter_pressed, coords_cursor_y_menu, win_menu, win_right, win_left);
                }
            }
            else if (ch == KEY_BACKSPACE || ch == 127) {
                    delete_char_from_enter_name(win_menu, screen_buffer, &buffer_pos);
            }
            else {
                    add_char_to_enter_name(win_menu, ch, screen_buffer, &buffer_pos);
            }
        }
    }
    wrefresh(win_menu);
}



void render_ls_and_create(user_data *ptr_user_data, file_data *all_files_left, file_data *all_files_right, _Bool turn_render_ls, _Bool active, _Bool check_side, _Bool *is_enter_pressed, int *coords_cursor_y_menu, WINDOW *win_menu, WINDOW *win_right, WINDOW *win_left)
{
    int *height = &ptr_user_data->coordinates.height;
    int *width = &ptr_user_data->coordinates.width;

    getmaxyx(stdscr, *height, *width);
    win_left = newwin(*height, *width / 2, 0, 0);
    win_right = newwin(*height, *width % 2 ? (*width / 2) + 1 : *width / 2, 0, *width / 2);
    render_ls(ptr_user_data, all_files_left, turn_render_ls ? 1 : 0, check_side, win_left);
    render_ls(ptr_user_data, all_files_right, turn_render_ls ? 0 : 1, !check_side, win_right);
    render_create(ptr_user_data, all_files_left, all_files_right, active, check_side, turn_render_ls, win_menu, win_right, win_left);
    *is_enter_pressed = false;
}




