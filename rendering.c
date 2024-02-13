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

void rendering(char *path, struct file_data *all_files, struct coordinates *coords, int number_lines, _Bool left_of_right, _Bool active, WINDOW *win)
{
    start_color();
    init_color(COLOR_BLUE, 0, 0, 650);
    init_color(COLOR_WHITE, 1000, 1000, 1000);

    init_pair(1, COLOR_WHITE, COLOR_BLUE); // Цветовая пара 1: Белый текст на темносинем фоне         // folder
    init_pair(2, COLOR_CYAN, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне.       // files // hide folder
    init_pair(3, COLOR_YELLOW, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне       // link 
    init_pair(4, COLOR_MAGENTA, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне       // ex      
    init_pair(5, COLOR_RED, COLOR_BLUE); // Цветовая пара 2: Красный текст на темносинем фоне.        // zip
    init_pair(6, COLOR_BLACK, COLOR_WHITE);


    wbkgd(win, COLOR_PAIR(1));
    
    getmaxyx(win, coords->height_win, coords->width_win);

    if(left_of_right) {
        mvwvline(win, 1, coords->width_win - 34, ACS_VLINE, coords->height - 4);
        mvwvline(win, 1, coords->width_win - 27, ACS_VLINE, coords->height - 4);
        mvwvline(win, 1, coords->width_win - 12, ACS_VLINE, coords->height - 4);
    } else {
        mvwvline(win, 1, coords->width % 2 ? (coords->width_win - 34) - 1 : (coords->width_win - 34), ACS_VLINE, coords->height - 4);
        mvwvline(win, 1, coords->width % 2 ? (coords->width_win - 27) - 1 : (coords->width_win - 27), ACS_VLINE, coords->height - 4);
        mvwvline(win, 1, coords->width % 2 ? (coords->width_win - 12) - 1 : (coords->width_win - 12), ACS_VLINE, coords->height - 4);
    }
    mvwhline(win, coords->height_win - 3, 1, ACS_HLINE, coords->width_win - 2);
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    wattron(win, A_BOLD);
    mvwprintw(win, 0, 1, path);
    wattroff(win, A_BOLD);

    for (int i = 0, row = 1; i < number_lines && i < coords->height_win - 4; ++i, ++row) {
        wattron(win, A_BOLD);
        if (strstr(all_files[i].permissions, "d") != NULL || strcmp(all_files[i].name, "..") == 0) {
            if (all_files[i].name[0] == '.' || strcmp(all_files[i].name, "..") == 0) {
                wattroff(win, A_BOLD);
                wattron(win, COLOR_PAIR(2));
                mvwprintw(win, row, 1, all_files[i].name);
                wattroff(win, COLOR_PAIR(2));
            } else {
                wattron(win, COLOR_PAIR(1));
                mvwprintw(win, row, 1, all_files[i].name);
                wattroff(win, COLOR_PAIR(1));
            }
        } else if((strstr(all_files[i].permissions, "l") != NULL)) {
            wattroff(win, A_BOLD);
            wattron(win, COLOR_PAIR(3));
            mvwprintw(win, row, 1, all_files[i].name);
            wattroff(win, COLOR_PAIR(3));
        } else if((strstr(all_files[i].name, ".zip") != NULL) || (strstr(all_files[i].name, ".gz") != NULL) || (strstr(all_files[i].name, ".tar") != NULL)) {
            wattron(win, COLOR_PAIR(5));
            mvwprintw(win, row, 1, all_files[i].name);
            wattroff(win, COLOR_PAIR(5));
        } else if (all_files[i].name[0] == '.'){
            wattron(win, COLOR_PAIR(2));   
            mvwprintw(win, row, 1, all_files[i].name);
            wattroff(win, COLOR_PAIR(2));          
        } else if (all_files[i].permissions[9] == 'x'){
            wattron(win, COLOR_PAIR(4));   
            mvwprintw(win, row, 1, all_files[i].name);
            wattroff(win, COLOR_PAIR(4));          
        } else {
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, row, 1, all_files[i].name);
            wattroff(win, COLOR_PAIR(2));
        }
        mvwprintw(win, row, coords->width / 2 - 33, all_files[i].size);
        mvwprintw(win, row, coords->width / 2 - 26, all_files[i].time);
        mvwprintw(win, row, coords->width / 2 - 11, all_files[i].permissions);
        wattroff(win, A_BOLD);
    }

    if (active) {
        for (int i = 0, row = 1; i < number_lines && row < coords->height_win - 4; ++i, ++row) {
            if (row == coords->cursor_y) {
                wattron(win, A_BOLD);
                wattron(win, COLOR_PAIR(6));                       // Включаем цветовую пару для всей строки
                mvwhline(win, row, 1, ' ', coords->width_win - 2); // Заполняем строку пробелами для очистки ее содержимого
                mvwprintw(win, row, 1, all_files[i].name);         // Выводим текст
                mvwprintw(win, row, coords->width / 2 - 33, all_files[i].size);
                mvwprintw(win, row, coords->width / 2 - 26, all_files[i].time);
                mvwprintw(win, row, coords->width / 2 - 11, all_files[i].permissions);
                wattroff(win, COLOR_PAIR(6)); // Отключаем цветовую пару
                wattroff(win, A_BOLD);

                wmove(win, coords->cursor_y, coords->cursor_x); // Возвращаем курсор на его предыдущее местоположение
            }
        }
    }

    wbkgd(win, COLOR_PAIR(1));


    // wmove(win, coords->cursor_y, coords->cursor_x);
    wrefresh(win);
    // wattron(win, COLOR_PAIR(6));   
    // wmove(win, coords->cursor_y, coords->cursor_x);
    // wprintw(win, "%s", all_files);
    // wattroff(win, COLOR_PAIR(6));
    // wrefresh(win);
}
