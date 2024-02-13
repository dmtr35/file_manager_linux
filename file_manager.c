#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>

#include <ncurses.h>
#include <locale.h>
#include <signal.h>

#include "func.h"


WINDOW *win_left;
WINDOW *win_right;


int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    initscr(); // Инициализация ncurses

    keypad(stdscr, TRUE); // Включаем обработку функциональных клавиш
    noecho(); // Отключаем вывод введенных символов
    curs_set(0);

    _Bool left_of_right = 1;
    _Bool active = 1;

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


    // struct file_data all_files_left[500];
    struct file_data *all_files = (struct file_data *)malloc(500 * sizeof(struct file_data));
    int number_lines = 0;
    _Bool flag_hidden_files = 1;
    struct user_data *ptr_user_data = malloc(sizeof(struct user_data));
    int res_check = check_func(ptr_user_data);



    int number_lines_left = ls_list(ptr_user_data->current_directory, all_files, number_lines, flag_hidden_files);
    rendering(ptr_user_data->current_directory, all_files, &coords, number_lines_left, 1, 1, win_left);


    int number_lines_right = ls_list(ptr_user_data->home, all_files, number_lines, flag_hidden_files);
    rendering(ptr_user_data->home, all_files, &coords, number_lines_right, 0, 0, win_right);

    // for (int i = 0; i < number_lines_right; i++) {
    //     printf("%s\n", all_files[i].name);
    //     // printf("Size: %s\n", all_files[i].size);
    //     // printf("Permissions: %s\n", all_files[i].permissions);
    //     // printf("Modification Time: %s\n", all_files[i].time);
    //     printf("\n");
    // }
    


    while (1) {

        getmaxyx(stdscr, coords.height, coords.width);
        win_left = newwin(coords.height, coords.width / 2, 0, 0);
        win_right = newwin(coords.height, coords.width % 2 ? (coords.width / 2) + 1 : coords.width / 2, 0, coords.width / 2);
        int number_lines_left = ls_list(ptr_user_data->current_directory, all_files, number_lines, flag_hidden_files);
        rendering(ptr_user_data->current_directory, all_files, &coords, number_lines_left, 1, 1, win_left);
        int number_lines_right = ls_list(ptr_user_data->home, all_files, number_lines, flag_hidden_files);
        rendering(ptr_user_data->home, all_files, &coords, number_lines_right, 0, 0, win_right);

        int ch = wgetch(win_left); // Получаем ввод от пользователя

        if (ch == 'q') {
            break;
        }
    }

    endwin(); // Завершаем работу с ncurses
    free(ptr_user_data);
    free(all_files);
    return 0;
}




        


