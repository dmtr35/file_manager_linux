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


void rendering(struct file_data *all_files, struct coordinates *coords, int number_lines, _Bool left_of_right, WINDOW *win)
{
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); // Цветовая пара 1: Белый текст на темносинем фоне
    init_color (COLOR_BLUE, 0, 0, 450);
    init_color(COLOR_WHITE, 750, 750, 750);

    init_pair(2, COLOR_YELLOW, COLOR_MAGENTA);
    init_color(COLOR_BLACK, 255, 255, 450);

    init_pair(1, COLOR_WHITE, COLOR_BLUE); // Белый текст на темно-синем фоне
    init_pair(2, COLOR_BLACK, COLOR_BLUE);

    int row_to_highlight = coords->cursor_y;
    int win_left_starty, win_left_startx, win_right_starty, win_right_startx;






        int row_l1 = 1;
        int row_l2 = 1;
        int row_l3 = 1;
        int row_l4 = 1;
        int row_r1 = 1;
        int row_r2 = 1;
        int row_r3 = 1;
        int row_r4 = 1;


        
        // win = newwin(coords->height, coords->width / 2, 0, 0);
        // win = newwin(coords->height, coords->width % 2 ? (coords->width / 2) + 1 : coords->width / 2, 0, coords->width / 2);

        // wattron(win, COLOR_PAIR(2));
        // box(win, 0, 0);
        // wattroff(win, COLOR_PAIR(2));

        // wattron(win, COLOR_PAIR(2));
        // box(win, 0, 0);
        // wattroff(win, COLOR_PAcoords->

        wbkgd(win, COLOR_PAIR(1));
        // wbkgd(win, COLOR_PAIR(1));
        // wbkgd(win, COLOR_PAIR(2));

        getmaxyx(win, coords->height_win, coords->width_win);
        // getmaxyx(win, coords->height_win_right, coords->width_win_right);

        mvwvline(win, 1, coords->width_win - 33, ACS_VLINE, coords->height - 4);
        mvwvline(win, 1, coords->width_win - 27, ACS_VLINE, coords->height - 4);
        mvwvline(win, 1, coords->width_win - 12, ACS_VLINE, coords->height - 4);
        mvwhline(win, coords->height_win - 3, 1, ACS_HLINE, coords->width_win - 2);

        // mvwvline(win, 1, coords->width_win_right - 33, ACS_VLINE, coords->height - 4);
        // mvwvline(win, 1, coords->width_win_right - 27, ACS_VLINE, coords->height - 4);
        // mvwvline(win, 1, coords->width_win_right - 12, ACS_VLINE, coords->height - 4);
        // mvwhline(win, coords->height_win_right - 3, 1, ACS_HLINE, coords->width_win_right -2);

        wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
        // wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

        mvwprintw(win, row_l1++, 1, "->->");
        row_l1++;
        wattron(win, COLOR_PAIR(2));
        mvwchgat(win, row_to_highlight, 0, -1, A_NORMAL, 2, NULL);
        mvwprintw(win, row_to_highlight, 1, "file_manager");
        wattroff(win, COLOR_PAIR(2));

        mvwprintw(win, row_l1++, 1, "file_manager->c");
        mvwprintw(win, row_l1++, 1, "q_info->txt");
        mvwprintw(win, row_l1++, 1, "test->c");

        mvwprintw(win, row_l2++, coords->width / 2 - 32, " size");
        mvwprintw(win, row_l2++, coords->width / 2 - 32, "55,3K");
        mvwprintw(win, row_l2++, coords->width / 2 - 32, "55,3K");
        mvwprintw(win, row_l2++, coords->width / 2 - 32, "55130");
        mvwprintw(win, row_l2++, coords->width / 2 - 32, "55,3K");
        
        
        mvwprintw(win, row_l3++, coords->width / 2 - 26, "     data");
        mvwprintw(win, row_l3++, coords->width / 2 - 26, "31->01->24 17:52");
        mvwprintw(win, row_l3++, coords->width / 2 - 26, "31->01->24 17:53");
        mvwprintw(win, row_l3++, coords->width / 2 - 26, "29->01->24 18:25");
        mvwprintw(win, row_l3++, coords->width / 2 - 26, "30->01->24 18:07");
        
        mvwprintw(win, row_l4++, coords->width / 2 - 11, "permission");
        mvwprintw(win, row_l4++, coords->width / 2 - 11, "-rwxrwxr-x");
        mvwprintw(win, row_l4++, coords->width / 2 - 11, "-rw-rw-r--");
        mvwprintw(win, row_l4++, coords->width / 2 - 11, "-rw-rw-r--");
        mvwprintw(win, row_l4++, coords->width / 2 - 11, "-rw-rw-r--");




        // mvwprintw(win, row_r1++, 1, "->->");
        // mvwprintw(win, row_r1++, 1, "file_manager");
        // mvwprintw(win, row_r1++, 1, "file_manager->c");
        // mvwprintw(win, row_r1++, 1, "q_info->txt");
        // mvwprintw(win, row_r1++, 1, "test->c");

        // mvwprintw(win, row_r2++, coords->width % 2 ? (coords->width / 2 - 32) + 1 : coords->width / 2 - 32, " size");
        // mvwprintw(win, row_r2++, coords->width % 2 ? (coords->width / 2 - 32) + 1 : coords->width / 2 - 32, "55,3K");
        // mvwprintw(win, row_r2++, coords->width % 2 ? (coords->width / 2 - 32) + 1 : coords->width / 2 - 32, "55,3K");
        // mvwprintw(win, row_r2++, coords->width % 2 ? (coords->width / 2 - 32) + 1 : coords->width / 2 - 32, "55,3K");
        // mvwprintw(win, row_r2++, coords->width % 2 ? (coords->width / 2 - 32) + 1 : coords->width / 2 - 32, "55,3K");
        
        // mvwprintw(win, row_r3++, coords->width % 2 ? (coords->width / 2 - 26) + 1 : coords->width / 2 - 26, "     data");
        // mvwprintw(win, row_r3++, coords->width % 2 ? (coords->width / 2 - 26) + 1 : coords->width / 2 - 26, "31->01->24 17:52");
        // mvwprintw(win, row_r3++, coords->width % 2 ? (coords->width / 2 - 26) + 1 : coords->width / 2 - 26, "31->01->24 17:53");
        // mvwprintw(win, row_r3++, coords->width % 2 ? (coords->width / 2 - 26) + 1 : coords->width / 2 - 26, "29->01->24 18:25");
        // mvwprintw(win, row_r3++, coords->width % 2 ? (coords->width / 2 - 26) + 1 : coords->width / 2 - 26, "30->01->24 18:07");
        
        // mvwprintw(win, row_r4++, coords->width % 2 ? (coords->width / 2 - 11) + 1 : coords->width / 2 - 11, "permission");
        // mvwprintw(win, row_r4++, coords->width % 2 ? (coords->width / 2 - 11) + 1 : coords->width / 2 - 11, "-rwxrwxr-x");
        // mvwprintw(win, row_r4++, coords->width % 2 ? (coords->width / 2 - 11) + 1 : coords->width / 2 - 11, "-rw-rw-r--");
        // mvwprintw(win, row_r4++, coords->width % 2 ? (coords->width / 2 - 11) + 1 : coords->width / 2 - 11, "-rw-rw-r--");
        // mvwprintw(win, row_r4++, coords->width % 2 ? (coords->width / 2 - 11) + 1 : coords->width / 2 - 11, "-rw-rw-r--");


        wmove(win, coords->cursor_y, coords->cursor_x);
        wrefresh(win);
}


