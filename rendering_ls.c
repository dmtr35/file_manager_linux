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


void render_ls(char *path, struct file_data *all_files, struct coordinates *coords, int *quantity_lines, _Bool flag_hidden_files, _Bool left_right, _Bool active, int offset, WINDOW *win)
{
    ls_list(path, all_files, flag_hidden_files, quantity_lines);

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

    getmaxyx(win, coords->height_win, coords->width_win);
    int total_width = coords->width;
    int window_width = coords->width_win; 
    int check_side = (window_width == (total_width / 2)) ? 1 : 0;


    int max_length = coords->width_win - 35;
    trim_filename(all_files, *quantity_lines, max_length);

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

    int i = 0 + offset;
    int row = 1;
    for (; i < *quantity_lines && i < (coords->height_win - 4) + offset; ++i, ++row) {
        wattron(win, A_BOLD);
        if (strstr(all_files[i].permissions, "d") != NULL || strcmp(all_files[i].name, "..") == 0) {
            if (all_files[i].name[0] == '.' || strcmp(all_files[i].name, "..") == 0) {
                wattroff(win, A_BOLD);
                active ? wattron(win, COLOR_PAIR(2)) : wattron(win, COLOR_PAIR(12));
                mvwprintw(win, row, 1, all_files[i].name);
                active ? wattroff(win, COLOR_PAIR(2)) : wattroff(win, COLOR_PAIR(12));
            } else {
                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, 1, all_files[i].name);
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));
            }
        } else if((strstr(all_files[i].permissions, "l") != NULL)) {
            wattroff(win, A_BOLD);
            active ? wattron(win, COLOR_PAIR(3)) : wattron(win, COLOR_PAIR(13));
            mvwprintw(win, row, 1, all_files[i].name);
            active ? wattroff(win, COLOR_PAIR(3)) : wattroff(win, COLOR_PAIR(13));
        } else if((strstr(all_files[i].name, ".zip") != NULL) || (strstr(all_files[i].name, ".gz") != NULL) || (strstr(all_files[i].name, ".tar") != NULL)) {
            active ? wattron(win, COLOR_PAIR(5)) : wattron(win, COLOR_PAIR(15));
            mvwprintw(win, row, 1, all_files[i].name);
            active ? wattroff(win, COLOR_PAIR(5)) : wattroff(win, COLOR_PAIR(15));
        } else if (all_files[i].name[0] == '.'){
            active ? wattron(win, COLOR_PAIR(2)) : wattron(win, COLOR_PAIR(12));  
            mvwprintw(win, row, 1, all_files[i].name);
            active ? wattroff(win, COLOR_PAIR(2)) : wattroff(win, COLOR_PAIR(12));
        } else if (all_files[i].permissions[9] == 'x'){
            active ? wattron(win, COLOR_PAIR(4)) : wattron(win, COLOR_PAIR(14));
            mvwprintw(win, row, 1, all_files[i].name);
            active ? wattroff(win, COLOR_PAIR(4)) : wattroff(win, COLOR_PAIR(14));
        } else {
            active ? wattron(win, COLOR_PAIR(2)) : wattron(win, COLOR_PAIR(12));
            mvwprintw(win, row, 1, all_files[i].name);
            active ? wattroff(win, COLOR_PAIR(2)) : wattroff(win, COLOR_PAIR(12));
        }
        if(check_side) {
            mvwprintw(win, row, coords->width / 2 - 33, all_files[i].size);
            mvwprintw(win, row, coords->width / 2 - 26, all_files[i].time);
            mvwprintw(win, row, coords->width / 2 - 11, all_files[i].permissions);
        } else {
            mvwprintw(win, row, coords->width / 2 - 32, all_files[i].size);
            mvwprintw(win, row, coords->width / 2 - 25, all_files[i].time);
            mvwprintw(win, row, coords->width / 2 - 10, all_files[i].permissions);
        }
        wattroff(win, A_BOLD);
    }

    i = 0 + offset;
    row = 1;
    int last_line = (coords->height_win - 4) + offset;
    if (active) {
        for (; i < *quantity_lines && row <= last_line; ++i, ++row) {
            if (row == coords->cursor_y) {
                wattron(win, A_BOLD);
                wattron(win, COLOR_PAIR(6));                       // Включаем цветовую пару для всей строки
                mvwhline(win, row, 1, ' ', coords->width_win - 2); // Заполняем строку пробелами для очистки ее содержимого
                if (check_side) {
                    mvwprintw(win, row, 1, all_files[i].name); // Выводим текст
                    mvwprintw(win, row, (coords->width / 2 - 33), all_files[i].size);
                    mvwprintw(win, row, (coords->width / 2 - 26), all_files[i].time);
                    mvwprintw(win, row, (coords->width / 2 - 11), all_files[i].permissions);
                } else {
                    mvwprintw(win, row, 1, all_files[i].name); // Выводим текст
                    mvwprintw(win, row, coords->width % 2 ? (coords->width / 2 - 32) : (coords->width / 2 - 33), all_files[i].size);
                    mvwprintw(win, row, coords->width % 2 ? (coords->width / 2 - 25) : (coords->width / 2 - 26), all_files[i].time);
                    mvwprintw(win, row, coords->width % 2 ? (coords->width / 2 - 10) : (coords->width / 2 - 11), all_files[i].permissions);
                }
                wattroff(win, COLOR_PAIR(6)); // Отключаем цветовую пару
                wattroff(win, A_BOLD);

                wmove(win, coords->cursor_y, coords->cursor_x); // Возвращаем курсор на его предыдущее местоположение
            } 
        }
    }

    wrefresh(win);

    // return res_number_lines;
}


void trim_filename(struct file_data *all_files, int number_lines, int max_length) {
    for(int i = 0; i < number_lines; ++i) {
        int name_length = strlen(all_files[i].name);
        all_files[i].name[max_length] = '\0';
    }
}
