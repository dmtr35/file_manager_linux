#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>

#include <ncurses.h>
#include <signal.h>

#include <libgen.h>
#include <locale.h>

#include "../func.h"


void render_ls(char *path, struct file_data *all_files, struct coordinates *coords, struct set_bool *set_bool, _Bool active, _Bool check_side, int *arr_coorsor, int leng_arr_coorsor_full, WINDOW *win)
{
    getmaxyx(win, coords->height_win, coords->width_win);

    int offset = 0;
    int quantity_lines = 0;
    _Bool *hidden_files = check_side ? &set_bool->hidden_left_bool : &set_bool->hidden_right_bool;
    ls_list(path, all_files, hidden_files, &quantity_lines);
    if (check_side) {
        coords->quantity_lines_left = quantity_lines;
        offset = coords->offset_left;
    } else {
        coords->quantity_lines_right = quantity_lines;
        offset = coords->offset_right;
    }

    struct file_data *all_files_ls = (struct file_data *)malloc(500 * sizeof(struct file_data));
    if (all_files_ls != NULL) {
        for (int i = 0; i < quantity_lines; i++) {
            strcpy(all_files_ls[i].name, all_files[i].name);
            strcpy(all_files_ls[i].size, all_files[i].size);
            strcpy(all_files_ls[i].time, all_files[i].time);
            strcpy(all_files_ls[i].permissions, all_files[i].permissions);
        }
    }
    
    start_color();
    init_color(COLOR_BLUE, 0, 0, 650);
    init_color(COLOR_WHITE, 1000, 1000, 1000);
    init_color(COLOR_MAGENTA, 300, 300, 300);

    init_pair(1, COLOR_WHITE, COLOR_BLUE); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(2, COLOR_CYAN, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне.       // files // hide folder
    init_pair(3, COLOR_YELLOW, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне       // link 
    init_pair(4, COLOR_GREEN, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне       // ex      
    init_pair(5, COLOR_RED, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне.        // zip
    init_pair(6, COLOR_BLACK, COLOR_WHITE);

    init_pair(11, COLOR_WHITE, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(12, COLOR_CYAN, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(13, COLOR_YELLOW, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(14, COLOR_GREEN, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(15, COLOR_RED, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(16, COLOR_BLACK, COLOR_MAGENTA); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    active ? wbkgd(win, COLOR_PAIR(1)) : wbkgd(win, COLOR_PAIR(11));



    int max_length = coords->width_win - 35;
    trim_filename(all_files_ls, quantity_lines, max_length);
    // char qqq[256];
    // strcpy(qqq, all_files_ls[2].name);

    if(check_side) {
        mvwvline(win, 1, coords->width_win - 34, ACS_VLINE, coords->height - 4);
        mvwvline(win, 1, coords->width_win - 27, ACS_VLINE, coords->height - 4);
        mvwvline(win, 1, coords->width_win - 12, ACS_VLINE, coords->height - 4);
    } else {
        mvwvline(win, 1, (coords->width_win - 33) - 1, ACS_VLINE, coords->height - 4);
        mvwvline(win, 1, (coords->width_win - 26) - 1, ACS_VLINE, coords->height - 4);
        mvwvline(win, 1, (coords->width_win - 11) - 1, ACS_VLINE, coords->height - 4);
    }
    mvwhline(win, coords->height_win - 3, 1, ACS_HLINE, coords->width_win - 2);
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    wattron(win, A_BOLD);
    wattron(win, COLOR_PAIR(6));
    mvwprintw(win, 0, 1, path);
    wattroff(win, COLOR_PAIR(6));
    wattroff(win, A_BOLD);

    int last_line = (coords->height_win - 4);
    int i = 0 + offset;
    int row = 1;
    for (; i < quantity_lines && i < (coords->height_win - 4) + offset; ++i, ++row) {
        wattron(win, A_BOLD);
        if (strstr(all_files[i].permissions, "d") != NULL || strcmp(all_files[i].name, "..") == 0) {
            if (all_files[i].name[0] == '.' || strcmp(all_files[i].name, "..") == 0) {
                wattroff(win, A_BOLD);
                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, 1, all_files_ls[i].name);
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));
            } else {
                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, 1, all_files_ls[i].name);
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));
            }
        } else if((strstr(all_files[i].permissions, "l") != NULL)) {
            char target_name[1024];
            char target_path[1024];
            char full_target_path[1024];
            int col = 1;
            strcpy(target_name, all_files[i].name);
            strcpy(full_target_path, all_files_ls[i].name);
            
            char *ptr_path = strstr(all_files[i].name, " -> ");
            if (ptr_path != NULL) {
                strcpy(target_path, ptr_path + 4);
            }
            char *ptr_name = strstr(target_name, " -> ");
            if (ptr_name != NULL) {
                *ptr_name = '\0';
            }

            int max_leng = strlen(full_target_path) - strlen(target_name) - 4;
            if (max_leng < 0) {
                max_leng = 0;
            }
            char cut_target_path[max_leng + 1];
            strncpy(cut_target_path, target_path, max_leng);
            cut_target_path[max_leng] = '\0';

            size_t leng_absolute_path = strlen(target_path) + strlen(path) + 2;
            char absolute_path[leng_absolute_path];

            if(target_path[0] == '/'){
                strcpy(absolute_path, target_path);
            } else {
                snprintf(absolute_path, leng_absolute_path, "%s/%s", path, target_path);
            }


            wattroff(win, A_BOLD);
            if (access(absolute_path, F_OK) != -1) {
            // Файл существует

                active ? wattron(win, COLOR_PAIR(3)) : wattron(win, COLOR_PAIR(13));
                mvwprintw(win, row, col, target_name);
                col += strlen(target_name);
                active ? wattroff(win, COLOR_PAIR(3)) : wattroff(win, COLOR_PAIR(13));

                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, col, " -> ");
                col += 4;
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));

                active ? wattron(win, COLOR_PAIR(3)) : wattron(win, COLOR_PAIR(13));
                mvwprintw(win, row, col, cut_target_path);
                col += strlen(cut_target_path);
                active ? wattroff(win, COLOR_PAIR(3)) : wattroff(win, COLOR_PAIR(13));
            } else {
            // Файл не существует
                active ? wattron(win, COLOR_PAIR(5)) : wattron(win, COLOR_PAIR(15));
                mvwprintw(win, row, col, target_name);
                col += strlen(target_name);
                active ? wattroff(win, COLOR_PAIR(5)) : wattroff(win, COLOR_PAIR(15));

                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, col, " -> ");
                col += 4;
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));

                active ? wattron(win, COLOR_PAIR(3)) : wattron(win, COLOR_PAIR(13));
                mvwprintw(win, row, col, cut_target_path);
                col += strlen(cut_target_path);
                active ? wattroff(win, COLOR_PAIR(3)) : wattroff(win, COLOR_PAIR(13));
            }

            
        } else if((strstr(all_files[i].name, ".zip") != NULL) || (strstr(all_files[i].name, ".gz") != NULL) || (strstr(all_files[i].name, ".tar") != NULL)) {
            active ? wattron(win, COLOR_PAIR(5)) : wattron(win, COLOR_PAIR(15));
            mvwprintw(win, row, 1, all_files_ls[i].name);
            active ? wattroff(win, COLOR_PAIR(5)) : wattroff(win, COLOR_PAIR(15));
        } else if (all_files[i].name[0] == '.'){
            wattroff(win, A_BOLD);
            active ? wattron(win, COLOR_PAIR(2)) : wattron(win, COLOR_PAIR(12));  
            mvwprintw(win, row, 1, all_files_ls[i].name);
            active ? wattroff(win, COLOR_PAIR(2)) : wattroff(win, COLOR_PAIR(12));
        } else if (all_files[i].permissions[9] == 'x'){
            active ? wattron(win, COLOR_PAIR(4)) : wattron(win, COLOR_PAIR(14));
            mvwprintw(win, row, 1, all_files_ls[i].name);
            active ? wattroff(win, COLOR_PAIR(4)) : wattroff(win, COLOR_PAIR(14));
        } else {
            active ? wattron(win, COLOR_PAIR(2)) : wattron(win, COLOR_PAIR(12));
            mvwprintw(win, row, 1, all_files_ls[i].name);
            active ? wattroff(win, COLOR_PAIR(2)) : wattroff(win, COLOR_PAIR(12));
        }
        if(check_side) {
            mvwprintw(win, row, coords->width / 2 - 33, all_files_ls[i].size);
            mvwprintw(win, row, coords->width / 2 - 26, all_files_ls[i].time);
            mvwprintw(win, row, coords->width / 2 - 11, all_files_ls[i].permissions);
        } else {
            if (coords->width_win % 2 == 0 && coords->width % 2 == 0 || coords->width_win % 2 != 0 && coords->width % 2 == 0) {
                mvwprintw(win, row, coords->width / 2 - 33, all_files_ls[i].size);
                mvwprintw(win, row, coords->width / 2 - 26, all_files_ls[i].time);
                mvwprintw(win, row, coords->width / 2 - 11, all_files_ls[i].permissions);
            } else if (coords->width_win % 2 != 0 && coords->width % 2 != 0 || coords->width_win % 2 == 0 && coords->width % 2 != 0) {
                mvwprintw(win, row, coords->width / 2 - 32, all_files_ls[i].size);
                mvwprintw(win, row, coords->width / 2 - 25, all_files_ls[i].time);
                mvwprintw(win, row, coords->width / 2 - 10, all_files_ls[i].permissions);
            } 
        }
        wattroff(win, A_BOLD);
    }





    
    int j = 0;
    if (active) {
        for (row = 1, i = 0 + offset; i < quantity_lines && row <= last_line; ++i, ++row) {
            int item_arr = arr_coorsor[j] + 1;
            if (item_arr >= row + offset && item_arr <= last_line + offset && arr_coorsor[j] != 0) {
                if (item_arr == row + offset) {
                    wattron(win, A_BOLD);
                    wattron(win, COLOR_PAIR(11));                       // Включаем цветовую пару для всей строки
                    mvwhline(win, row, 1, ' ', coords->width_win - 2); // Заполняем строку пробелами для очистки ее содержимого
                    if (check_side) {
                        mvwprintw(win, row, 1, all_files_ls[i].name);  // Выводим текст
                        mvwprintw(win, row, (coords->width / 2 - 33), all_files_ls[i].size);
                        mvwprintw(win, row, (coords->width / 2 - 26), all_files_ls[i].time);
                        mvwprintw(win, row, (coords->width / 2 - 11), all_files_ls[i].permissions);
                    } else {
                        if (coords->width_win % 2 == 0 && coords->width % 2 == 0 || coords->width_win % 2 != 0 && coords->width % 2 == 0) {
                            mvwprintw(win, row, 1, all_files_ls[i].name);  // Выводим текст
                            mvwprintw(win, row, coords->width / 2 - 33, all_files_ls[i].size);
                            mvwprintw(win, row, coords->width / 2 - 26, all_files_ls[i].time);
                            mvwprintw(win, row, coords->width / 2 - 11, all_files_ls[i].permissions);
                        } else if (coords->width_win % 2 != 0 && coords->width % 2 != 0 || coords->width_win % 2 == 0 && coords->width % 2 != 0) {
                            mvwprintw(win, row, 1, all_files_ls[i].name);  // Выводим текст
                            mvwprintw(win, row, coords->width / 2 - 32, all_files_ls[i].size);
                            mvwprintw(win, row, coords->width / 2 - 25, all_files_ls[i].time);
                            mvwprintw(win, row, coords->width / 2 - 10, all_files_ls[i].permissions);
                        }
                    }
                    wattroff(win, COLOR_PAIR(11)); // Отключаем цветовую пару
                    wattroff(win, A_BOLD);

                    wmove(win, coords->cursor_y, coords->cursor_x); // Возвращаем курсор на его предыдущее местоположение
                    j++;
                }
            } else {
                j++;
            }
        }
        for (row = 1, i = 0 + offset; i < quantity_lines && row <= last_line; ++i, ++row) {
            if (row == coords->cursor_y) {
                wattron(win, A_BOLD);
                wattron(win, COLOR_PAIR(6));                       // Включаем цветовую пару для всей строки
                mvwhline(win, row, 1, ' ', coords->width_win - 2); // Заполняем строку пробелами для очистки ее содержимого
                if (check_side) {
                    mvwprintw(win, row, 1, all_files_ls[i].name);  // Выводим текст
                    mvwprintw(win, row, (coords->width / 2 - 33), all_files_ls[i].size);
                    mvwprintw(win, row, (coords->width / 2 - 26), all_files_ls[i].time);
                    mvwprintw(win, row, (coords->width / 2 - 11), all_files_ls[i].permissions);

                    if (strcmp(all_files[i].name, "..") == 0) {

                    } else {
                        mvwprintw(win, coords->height_win - 2, 1, all_files[i].name);
                    }
                } else {
                    if (coords->width_win % 2 == 0 && coords->width % 2 == 0 || coords->width_win % 2 != 0 && coords->width % 2 == 0) {
                        mvwprintw(win, row, 1, all_files_ls[i].name);  // Выводим текст
                        mvwprintw(win, row, coords->width / 2 - 33, all_files_ls[i].size);
                        mvwprintw(win, row, coords->width / 2 - 26, all_files_ls[i].time);
                        mvwprintw(win, row, coords->width / 2 - 11, all_files_ls[i].permissions);
                    } else if (coords->width_win % 2 != 0 && coords->width % 2 != 0 || coords->width_win % 2 == 0 && coords->width % 2 != 0) {
                        mvwprintw(win, row, 1, all_files_ls[i].name);  // Выводим текст
                        mvwprintw(win, row, coords->width / 2 - 32, all_files_ls[i].size);
                        mvwprintw(win, row, coords->width / 2 - 25, all_files_ls[i].time);
                        mvwprintw(win, row, coords->width / 2 - 10, all_files_ls[i].permissions);
                    } 

                    if (strcmp(all_files[i].name, "..") == 0) {

                    } else {
                        mvwprintw(win, coords->height_win - 2, 1, all_files[i].name);
                    }
                }
                wattroff(win, COLOR_PAIR(6)); // Отключаем цветовую пару
                wattroff(win, A_BOLD);

                wmove(win, coords->cursor_y, coords->cursor_x); // Возвращаем курсор на его предыдущее местоположение
            } 
        }
    }

    wrefresh(win);

    free(all_files_ls);
}


void trim_filename(struct file_data *all_files_ls, int number_lines, int max_length) {
    for(int i = 0; i < number_lines; ++i) {
        int name_length = strlen(all_files_ls[i].name);
        all_files_ls[i].name[max_length] = '\0';
    }
}
