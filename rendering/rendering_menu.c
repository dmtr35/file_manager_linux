#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>

#include <ncurses.h>
#include <signal.h>

#include "../func.h"

void render_menu(struct user_data *ptr_user_data, struct file_data *all_files_left, struct file_data *all_files_right, struct set_bool *set_bool, int *coords_cursor_y_menu, struct coordinates *coords, _Bool active, _Bool check_side, _Bool turn_render_ls, int *arr_coorsor, int leng_arr_coorsor_full, WINDOW *win_menu, WINDOW *win_right, WINDOW *win_left)
{
    start_color();
    // init_color(COLOR_BLUE, 0, 0, 650);
    // init_color(COLOR_WHITE, 1000, 1000, 1000);
    // init_color(COLOR_MAGENTA, 300, 300, 300);

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

    char path[256];
    strcpy(path, active ? ptr_user_data->left_path : ptr_user_data->right_path);
    char *file_name = ptr_user_data->coorsor_file;

    size_t size_new_path = strlen(path) + strlen(file_name) + 3;
    char new_path[size_new_path];


    int *offset = active ? &coords->offset_left : &coords->offset_right;
    int *quantity_lines = active ? &coords->quantity_lines_left : &coords->quantity_lines_right;
    struct file_data *all_files_ptr = active ? all_files_left : all_files_right;
    bool is_enter_pressed = true;
    int row;

    while (is_enter_pressed) {
        getmaxyx(win_menu, height_win, width_win);
        size_t leng_path = active ? strlen(ptr_user_data->left_path) + strlen(ptr_user_data->coorsor_file) + 4 : strlen(ptr_user_data->right_path) + strlen(ptr_user_data->coorsor_file) + 4;
        size_t width_menu = leng_path < coords->width / 3 ? coords->width / 3 : leng_path;
        win_menu = newwin(10, width_menu, (coords->height / 2) - 5, coords->width / 2 - width_menu / 2);

        wborder(win_menu, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
        snprintf(new_path, size_new_path, "%s/%s", path, file_name);
        // printf("%s\n", screen_buffer);
        // wattron(win_menu, A_BOLD);
        // mvwprintw(win_menu, 1, 1, new_path);
        // mvwprintw(win_menu, 3, width_win / 2 - 2, "Copy");
        // mvwprintw(win_menu, 4, width_win / 2 - 2, "Move");
        // mvwprintw(win_menu, 5, width_win / 2 - 3, "Delete");
        // mvwprintw(win_menu, 6, width_win / 2 - 6, "Delete & save");
        // wattroff(win_menu, A_BOLD);

        int offset_menu = 0;
        int i = 0 + offset_menu;
        int row = 3;
        for (; i < 4 && row <= 6; ++i, ++row) {
            if (row == *coords_cursor_y_menu) {
                wattron(win_menu, A_BOLD);
                wattron(win_menu, COLOR_PAIR(22));                       // Включаем цветовую пару для всей строки
                mvwhline(win_menu, row, 1, ' ', width_win - 2); // Заполняем строку пробелами для очистки ее содержимого

                mvwprintw(win_menu, 1, 1, new_path);
                mvwprintw(win_menu, 3, width_win / 2 - 2, "Copy");
                mvwprintw(win_menu, 4, width_win / 2 - 2, "Move");
                mvwprintw(win_menu, 5, width_win / 2 - 3, "Delete");
                mvwprintw(win_menu, 6, width_win / 2 - 6, "Delete & save");

                wattroff(win_menu, COLOR_PAIR(22)); // Отключаем цветовую пару
                wattroff(win_menu, A_BOLD);

            } 
    }

        int ch = wgetch(win_menu);

        

        if (ch != ERR) {
            if (ch == 1 || ch == 'm') {                 
                is_enter_pressed = false;
                set_bool->menu_bool = false;
            } else if (ch == 'z') {
                // restore_from_buffer(win_left, screen_buffer);

            } else if (ch == 'q') {
                is_enter_pressed = false;
                set_bool->menu_bool = false;
                set_bool->out_bool = true;
            break;
            } else if (ch == '\n') {
                if (*coords_cursor_y_menu == 3) {
                    printf("here1");
                } else if (*coords_cursor_y_menu == 4) {
                    printf("here2");
                } else if (*coords_cursor_y_menu == 5) {                                    // Delete
                    remote_or_remove_save(arr_coorsor, coords, leng_arr_coorsor_full, path, file_name, quantity_lines, offset, active, all_files_ptr, set_bool, ptr_user_data);

                    if(offset < 0){
                        offset = 0;
                    }
                } else if (*coords_cursor_y_menu == 6) {                                    // Delete & save
                    set_bool->save_files = 1;
                    remote_or_remove_save(arr_coorsor, coords, leng_arr_coorsor_full, path, file_name, quantity_lines, offset, active, all_files_ptr, set_bool, ptr_user_data);
                    set_bool->save_files = 0;
                }




                is_enter_pressed = false;
                set_bool->menu_bool = false;
            } else if (ch == 'r' || ch == KEY_RESIZE) {
                getmaxyx(stdscr, coords->height, coords->width);
                win_left = newwin(coords->height, coords->width / 2, 0, 0);
                win_right = newwin(coords->height, coords->width % 2 ? (coords->width / 2) + 1 : coords->width / 2, 0, coords->width / 2);
                render_ls(ptr_user_data->left_path, all_files_left, coords, set_bool, turn_render_ls ? 1 : 0, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                render_ls(ptr_user_data->right_path, all_files_right, coords, set_bool, turn_render_ls ? 0 : 1, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                render_menu(ptr_user_data, all_files_left, all_files_right, set_bool, coords_cursor_y_menu, coords, active, check_side, turn_render_ls, arr_coorsor, leng_arr_coorsor_full, win_menu, win_right, win_left);
                is_enter_pressed = false;
            }
            else if (ch == 27) {
                int next1 = wgetch(stdscr);
                int next2 = wgetch(stdscr);
                if (next1 == '[' && next2 == 'A') {
                    if (*coords_cursor_y_menu > 3) {
                        (*coords_cursor_y_menu)--;
                    }
                getmaxyx(stdscr, coords->height, coords->width);
                win_left = newwin(coords->height, coords->width / 2, 0, 0);
                win_right = newwin(coords->height, coords->width % 2 ? (coords->width / 2) + 1 : coords->width / 2, 0, coords->width / 2);
                render_ls(ptr_user_data->left_path, all_files_left, coords, set_bool, turn_render_ls ? 1 : 0, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                render_ls(ptr_user_data->right_path, all_files_right, coords, set_bool, turn_render_ls ? 0 : 1, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                render_menu(ptr_user_data, all_files_left, all_files_right, set_bool, coords_cursor_y_menu, coords, active, check_side, turn_render_ls, arr_coorsor, leng_arr_coorsor_full, win_menu, win_right, win_left);
                is_enter_pressed = false;
                } 
                else if (next1 == '[' && next2 == 'B') {
                    if (*coords_cursor_y_menu < 6) {
                        (*coords_cursor_y_menu)++;
                    }
                getmaxyx(stdscr, coords->height, coords->width);
                win_left = newwin(coords->height, coords->width / 2, 0, 0);
                win_right = newwin(coords->height, coords->width % 2 ? (coords->width / 2) + 1 : coords->width / 2, 0, coords->width / 2);
                render_ls(ptr_user_data->left_path, all_files_left, coords, set_bool, turn_render_ls ? 1 : 0, check_side, arr_coorsor, leng_arr_coorsor_full, win_left);
                render_ls(ptr_user_data->right_path, all_files_right, coords, set_bool, turn_render_ls ? 0 : 1, !check_side, arr_coorsor, leng_arr_coorsor_full, win_right);
                render_menu(ptr_user_data, all_files_left, all_files_right, set_bool, coords_cursor_y_menu, coords, active, check_side, turn_render_ls, arr_coorsor, leng_arr_coorsor_full, win_menu, win_right, win_left);
                is_enter_pressed = false;
                }
            }
        }
    }
    wrefresh(win_menu);
}


void remote_or_remove_save(int *arr_coorsor, struct coordinates *coords, int leng_arr_coorsor_full, char *path, char *file_name, int *quantity_lines, int *offset, _Bool active, struct file_data *all_files_ptr, struct set_bool *set_bool, struct user_data *ptr_user_data)
{
    int check_empty = check_int_arr(arr_coorsor, leng_arr_coorsor_full);
    int count_item_arr = count_non_zero_elements(arr_coorsor, leng_arr_coorsor_full);
    if (check_empty) {
        if(!(strcmp(file_name, "..") == 0)) {
            remove_directory_recursive(path, file_name, set_bool, ptr_user_data);
            if (*quantity_lines == (coords->cursor_y + *offset)) {
                coords->cursor_y--;
            }
        }
    } 
    else {
        remove_files(path, arr_coorsor, active, all_files_ptr, coords, set_bool, ptr_user_data);
        fillWithZeros(arr_coorsor, coords, leng_arr_coorsor_full);
        if (coords->cursor_y + *offset > *quantity_lines - count_item_arr && *offset != 0) {
            *offset -= count_item_arr;
        } else if (coords->cursor_y + *offset > *quantity_lines - count_item_arr && *offset == 0) {
            coords->cursor_y = *quantity_lines - count_item_arr - *offset;
        }
    }
}
