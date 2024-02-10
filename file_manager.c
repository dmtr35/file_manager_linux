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


WINDOW *win_left;
WINDOW *win_right;


int main() {
    initscr(); // Инициализация ncurses
    // start_color();

    // init_pair(1, COLOR_WHITE, COLOR_BLUE); // Цветовая пара 1: Белый текст на темносинем фоне
    // init_color (COLOR_BLUE, 0, 0, 450);
    // init_color(COLOR_WHITE, 750, 750, 750);

    // init_pair(2, COLOR_YELLOW, COLOR_MAGENTA);
    // init_color(COLOR_BLACK, 255, 255, 450);

    // init_pair(1, COLOR_WHITE, COLOR_BLUE); // Белый текст на темно-синем фоне
    // init_pair(2, COLOR_BLACK, COLOR_BLUE);

    keypad(stdscr, TRUE); // Включаем обработку функциональных клавиш
    noecho(); // Отключаем вывод введенных символов

    _Bool left_of_right = 0;

    struct coordinates coords;
    coords.height = 0;
    coords.width = 0;
    coords.height_win = 0;
    coords.width_win = 0;
    coords.cursor_x = 1;
    coords.cursor_y = 2;


    getmaxyx(stdscr, coords.height, coords.width);
    win_left = newwin(coords.height, coords.width / 2, 0, 0);
    win_right = newwin(coords.height, coords.width % 2 ? (coords.width / 2) + 1 : coords.width / 2, 0, coords.width / 2);

    // int row_to_highlight = coords.cursor_left_y;


    // struct file_data all_files_left[500];
    struct file_data *all_files = (struct file_data *)malloc(500 * sizeof(struct file_data));
    int number_lines = 0;
    _Bool flag_hidden_files = 0;
    struct user_data *ptr_user_data = malloc(sizeof(struct user_data));
    int res_check = check_func(ptr_user_data);

    // strcpy(ptr_user_data->current_directory, "/");
    // int res = ls_list(ptr_user_data->home, flag_hidden_files);
    int number_lines_left = ls_list(ptr_user_data->current_directory, all_files, number_lines, flag_hidden_files);
    
    rendering(all_files, &coords, number_lines_left, left_of_right, win_left);
    // printf("result_count_left: %d\n", number_lines_left);
    // for (int i = 0; i < number_lines_left; i++) {
    //     printf("Directory Name: %s\n", all_files_left[i].name);
    //     printf("Size: %s\n", all_files_left[i].size);
    //     printf("Permissions: %s\n", all_files_left[i].permissions);
    //     printf("Modification Time: %s\n", all_files_left[i].time);
    //     printf("\n");
    // }

    int number_lines_right = ls_list(ptr_user_data->home, all_files, number_lines, flag_hidden_files);
    rendering(all_files, &coords, number_lines_right, left_of_right, win_right);

    // printf("user: %s\n", ptr_user_data->user);
    // printf("home: %s\n", ptr_user_data->home);
    // printf("current_directory: %s\n", ptr_user_data->current_directory);


    while (1) {
        // // getmaxyx(stdscr, height, width);
        // // int row_l1 = 1;
        // // int row_l2 = 1;
        // // int row_l3 = 1;
        // // int row_l4 = 1;
        // // int row_r1 = 1;
        // // int row_r2 = 1;
        // // int row_r3 = 1;
        // // int row_r4 = 1;

        // int win_left_starty, win_left_startx, win_right_starty, win_right_startx;

        
        // win_left = newwin(coords.height, coords.width / 2, 0, 0);
        // win_right = newwin(coords.height, coords.width % 2 ? (coords.width / 2) + 1 : coords.width / 2, 0, coords.width / 2);

        // // wattron(win_left, COLOR_PAIR(2));
        // // box(win_left, 0, 0);
        // // wattroff(win_left, COLOR_PAIR(2));

        // // wattron(win_right, COLOR_PAIR(2));
        // // box(win_right, 0, 0);
        // // wattroff(win_right, COLOR_PAIR(2));

        // wbkgd(win_left, COLOR_PAIR(1));
        // wbkgd(win_right, COLOR_PAIR(1));
        // // wbkgd(win_left, COLOR_PAIR(2));

        // getmaxyx(win_left, coords.height_win_left, coords.width_win_left);
        // getmaxyx(win_right, coords.height_win_right, coords.width_win_right);

        // mvwvline(win_left, 1, coords.width_win_left - 33, ACS_VLINE, coords.height - 4);
        // mvwvline(win_left, 1, coords.width_win_left - 27, ACS_VLINE, coords.height - 4);
        // mvwvline(win_left, 1, coords.width_win_left - 12, ACS_VLINE, coords.height - 4);
        // mvwhline(win_left, coords.height_win_left - 3, 1, ACS_HLINE, coords.width_win_left - 2);

        // mvwvline(win_right, 1, coords.width_win_right - 33, ACS_VLINE, coords.height - 4);
        // mvwvline(win_right, 1, coords.width_win_right - 27, ACS_VLINE, coords.height - 4);
        // mvwvline(win_right, 1, coords.width_win_right - 12, ACS_VLINE, coords.height - 4);
        // mvwhline(win_right, coords.height_win_right - 3, 1, ACS_HLINE, coords.width_win_right -2);

        // wborder(win_left, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
        // wborder(win_right, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

        // mvwprintw(win_left, row_l1++, 1, "..");
        // row_l1++;
        // wattron(win_left, COLOR_PAIR(2));
        // // mvwchgat(win_left, row_to_highlight, 0, -1, A_NORMAL, 2, NULL);
        // // mvwprintw(win_left, row_to_highlight, 1, "file_manager");
        // wattroff(win_left, COLOR_PAIR(2));

        // // mvwprintw(win_left, row_l1++, 1, "file_manager.c");
        // // mvwprintw(win_left, row_l1++, 1, "q_info.txt");
        // // mvwprintw(win_left, row_l1++, 1, "test.c");

        // // mvwprintw(win_left, row_l2++, width / 2 - 32, " size");
        // // mvwprintw(win_left, row_l2++, width / 2 - 32, "55,3K");
        // // mvwprintw(win_left, row_l2++, width / 2 - 32, "55,3K");
        // // mvwprintw(win_left, row_l2++, width / 2 - 32, "55130");
        // // mvwprintw(win_left, row_l2++, width / 2 - 32, "55,3K");
        
        
        // // mvwprintw(win_left, row_l3++, width / 2 - 26, "     data");
        // // mvwprintw(win_left, row_l3++, width / 2 - 26, "31.01.24 17:52");
        // // mvwprintw(win_left, row_l3++, width / 2 - 26, "31.01.24 17:53");
        // // mvwprintw(win_left, row_l3++, width / 2 - 26, "29.01.24 18:25");
        // // mvwprintw(win_left, row_l3++, width / 2 - 26, "30.01.24 18:07");
        
        // // mvwprintw(win_left, row_l4++, width / 2 - 11, "permission");
        // // mvwprintw(win_left, row_l4++, width / 2 - 11, "-rwxrwxr-x");
        // // mvwprintw(win_left, row_l4++, width / 2 - 11, "-rw-rw-r--");
        // // mvwprintw(win_left, row_l4++, width / 2 - 11, "-rw-rw-r--");
        // // mvwprintw(win_left, row_l4++, width / 2 - 11, "-rw-rw-r--");




        // // right
        // // mvwprintw(win_right, row_r1++, 1, "..");
        // // mvwprintw(win_right, row_r1++, 1, "file_manager");
        // // mvwprintw(win_right, row_r1++, 1, "file_manager.c");
        // // mvwprintw(win_right, row_r1++, 1, "q_info.txt");
        // // mvwprintw(win_right, row_r1++, 1, "test.c");

        // // mvwprintw(win_right, row_r2++, width % 2 ? (width / 2 - 32) + 1 : width / 2 - 32, " size");
        // // mvwprintw(win_right, row_r2++, width % 2 ? (width / 2 - 32) + 1 : width / 2 - 32, "55,3K");
        // // mvwprintw(win_right, row_r2++, width % 2 ? (width / 2 - 32) + 1 : width / 2 - 32, "55,3K");
        // // mvwprintw(win_right, row_r2++, width % 2 ? (width / 2 - 32) + 1 : width / 2 - 32, "55,3K");
        // // mvwprintw(win_right, row_r2++, width % 2 ? (width / 2 - 32) + 1 : width / 2 - 32, "55,3K");
        
        // // mvwprintw(win_right, row_r3++, width % 2 ? (width / 2 - 26) + 1 : width / 2 - 26, "     data");
        // // mvwprintw(win_right, row_r3++, width % 2 ? (width / 2 - 26) + 1 : width / 2 - 26, "31.01.24 17:52");
        // // mvwprintw(win_right, row_r3++, width % 2 ? (width / 2 - 26) + 1 : width / 2 - 26, "31.01.24 17:53");
        // // mvwprintw(win_right, row_r3++, width % 2 ? (width / 2 - 26) + 1 : width / 2 - 26, "29.01.24 18:25");
        // // mvwprintw(win_right, row_r3++, width % 2 ? (width / 2 - 26) + 1 : width / 2 - 26, "30.01.24 18:07");
        
        // // mvwprintw(win_right, row_r4++, width % 2 ? (width / 2 - 11) + 1 : width / 2 - 11, "permission");
        // // mvwprintw(win_right, row_r4++, width % 2 ? (width / 2 - 11) + 1 : width / 2 - 11, "-rwxrwxr-x");
        // // mvwprintw(win_right, row_r4++, width % 2 ? (width / 2 - 11) + 1 : width / 2 - 11, "-rw-rw-r--");
        // // mvwprintw(win_right, row_r4++, width % 2 ? (width / 2 - 11) + 1 : width / 2 - 11, "-rw-rw-r--");
        // // mvwprintw(win_right, row_r4++, width % 2 ? (width / 2 - 11) + 1 : width / 2 - 11, "-rw-rw-r--");


        // wrefresh(win_right);
        // wmove(win_right, coords.cursor_right_y, coords.cursor_right_x);
        // wmove(win_left, coords.cursor_left_y, coords.cursor_left_x);
        // wrefresh(win_right);
        // wrefresh(win_left);


        int ch = wgetch(win_left); // Получаем ввод от пользователя

        if (ch == 'q') {
            break; // Если введен 'q', выходим из цикла
        }
    }

    endwin(); // Завершаем работу с ncurses
    free(ptr_user_data);
    free(all_files);
    return 0;
}




        


