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



void render_menu(char *path, struct file_data *all_files, struct coordinates *coords, int *quantity_lines, _Bool flag_hidden_files, _Bool active, int offset, WINDOW *win)
{
    char title[5] = "HELP";

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

    getmaxyx(win, coords->height_menu, coords->width_menu);

    int max_length = coords->width_win - 35;

    // if(active) {
    //     mvwvline(win, 1, coords->width_win - 34, ACS_VLINE, coords->height - 4);
    //     mvwvline(win, 1, coords->width_win - 27, ACS_VLINE, coords->height - 4);
    //     mvwvline(win, 1, coords->width_win - 12, ACS_VLINE, coords->height - 4);
    // } else {
    //     mvwvline(win, 1, coords->width % 2 ? (coords->width_win - 33) - 1 : (coords->width_win - 34), ACS_VLINE, coords->height - 4);
    //     mvwvline(win, 1, coords->width % 2 ? (coords->width_win - 26) - 1 : (coords->width_win - 27), ACS_VLINE, coords->height - 4);
    //     mvwvline(win, 1, coords->width % 2 ? (coords->width_win - 11) - 1 : (coords->width_win - 12), ACS_VLINE, coords->height - 4);
    // }
    mvwhline(win, coords->height_win - 3, 1, ACS_HLINE, coords->width_win - 2);
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    wattron(win, A_BOLD);
    wattron(win, COLOR_PAIR(6));
    mvwprintw(win, 0, 1, title);
    wattroff(win, COLOR_PAIR(6));
    wattroff(win, A_BOLD);



    wrefresh(win);

}
