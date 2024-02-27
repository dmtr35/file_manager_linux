#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <libgen.h>

#include <ncurses.h>
#include <locale.h>
#include <signal.h>

#include "func.h"


WINDOW *win_left;
WINDOW *win_right;



int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    initscr();                                                          // Инициализация ncurses
    nodelay(stdscr, TRUE);                                              // Set non-blocking mode for standard screen
    noecho();                                                           // Отключаем вывод введенных символов

    keypad(stdscr, TRUE); // Включаем обработку функциональных клавиш
    // printw("%i\n", wgetch(curscr));
    // printw ("%i\n%i\n%i\n", getch (), getch (), getch ());
    curs_set(0);

    _Bool active = 1;
    _Bool left_right = 0;
    _Bool bool_win_command = 0;
    char previous_path_left[1024];
    char previous_path_right[1024];
    

    struct coordinates coords;
    coords.height = 0;
    coords.width = 0;
    coords.height_win = 0;
    coords.width_win = 0;
    coords.cursor_x = 1;
    coords.cursor_y = 1;            

    int cursor_left = 1;
    int cursor_right = 1;
    int offset_left = 0;
    int offset_right = 0;


    int prev_height;
    int prev_width;

    struct file_data *all_files_left = (struct file_data *)malloc(500 * sizeof(struct file_data));
    struct file_data *all_files_right = (struct file_data *)malloc(500 * sizeof(struct file_data));
    _Bool flag_hidden_files = 1;
    struct user_data *ptr_user_data = malloc(sizeof(struct user_data));
    int res_check = check_func(ptr_user_data);
    strcpy(previous_path_left, ptr_user_data->left_path);    
    strcpy(previous_path_right, ptr_user_data->right_path);


    while (1) {
        int quantity_lines_left;
        int quantity_lines_right;
        getmaxyx(stdscr, coords.height, coords.width);
        win_left = newwin(coords.height, coords.width / 2, 0, 0);
        win_right = newwin(coords.height, coords.width % 2 ? (coords.width / 2) + 1 : coords.width / 2, 0, coords.width / 2);

        if(!bool_win_command) {
            if(active) {
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &quantity_lines_right, flag_hidden_files, !active, offset_right, win_right);
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &quantity_lines_left, flag_hidden_files, active, offset_left, win_left);
            } else {
                render_ls(ptr_user_data->left_path, all_files_left, &coords, &quantity_lines_left, flag_hidden_files, active, offset_left, win_left);
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &quantity_lines_right, flag_hidden_files, !active, offset_right, win_right);
            }
        } else {
            if(active) {
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &quantity_lines_right, flag_hidden_files, !active, offset_right, win_right);
                render_comm_XXX(ptr_user_data, all_files_right, &coords, &quantity_lines_right, &bool_win_command, flag_hidden_files, active, offset_right, win_left, win_right);
            } else {
                render_comm_XXX(ptr_user_data, all_files_right, &coords, &quantity_lines_right, &bool_win_command, flag_hidden_files, active, offset_right, win_left, win_right);
                render_ls(ptr_user_data->right_path, all_files_right, &coords, &quantity_lines_right, flag_hidden_files, !active, offset_right, win_right);
            }

            if(active && !bool_win_command) {
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
        } else if (ch == '\t') {
            if(active) {
                cursor_left = coords.cursor_y;
            } else {
                cursor_right = coords.cursor_y;
            }
            active = !active;
            if(active) {
                if (cursor_left <= (coords.height_win - 4)) {
                    coords.cursor_y = cursor_left;
                } else {
                    coords.cursor_y = coords.height_win - 4;
                }
            } else {
                if (cursor_right <= (coords.height_win - 4)) {
                    coords.cursor_y = cursor_right;
                } else {
                    coords.cursor_y = coords.height_win - 4;
                }
            }
        } else if (ch == '\n') {
            if (active) {
                click_on_file(ptr_user_data->left_path, all_files_left, &coords, previous_path_left, &offset_left);
            } else {
                click_on_file(ptr_user_data->right_path, all_files_right, &coords, previous_path_right, &offset_right);
            }
            
        } else if (ch == 27) {
            int next1 = wgetch(stdscr);
            int next2 = wgetch(stdscr);
            if (next1 == '[' && next2 == 'A') {
                int offset_to_increment = active ? offset_left : offset_right;
                int quantity_lines = (active ? quantity_lines_left : quantity_lines_right);
                int hidden_row = quantity_lines - coords.cursor_y;
                if (coords.cursor_y > 1) {
                    coords.cursor_y--;
                } else if (coords.cursor_y == 1 && offset_to_increment > 0) {
                    if(active) {
                        offset_left--;
                    } else {
                        offset_right--;
                    }
                }

            } else if (next1 == '[' && next2 == 'B') {
                int offset_to_increment = active ? offset_left : offset_right;
                int quantity_lines = (active ? quantity_lines_left : quantity_lines_right);
                int hidden_row = quantity_lines - coords.cursor_y;
                if (coords.cursor_y < quantity_lines && coords.cursor_y < coords.height_win - 4) {
                    coords.cursor_y++;

                } 
                else if ((coords.cursor_y >= coords.height_win - 4) && hidden_row > offset_to_increment) {
                    active ? offset_left++ : offset_right++;
                }

            } else if (next1 == '[' && next2 == 'C') {
                int offset_to_increment = active ? offset_left : offset_right;
                int quantity_lines = (active ? quantity_lines_left : quantity_lines_right);
                int hidden_row = quantity_lines - (coords.height_win - 4) - offset_to_increment;
                if(quantity_lines <= coords.height_win - 4) {
                    coords.cursor_y = quantity_lines;
                } else {
                    if(active) {
                        offset_left += hidden_row;
                    } else {
                        offset_right += hidden_row;
                    }
                    coords.cursor_y = coords.height_win - 4;
                }

            } else if (next1 == '[' && next2 == 'D') {
                int offset_to_increment = active ? offset_left : offset_right;

                if (active) {
                    offset_left = 0;
                } else {
                    offset_right = 0;
                }
                coords.cursor_y = 1;
            }


        } else if (ch == 127 || ch == KEY_BACKSPACE) {
            if (active) {
                click_on_file(ptr_user_data->left_path, all_files_left, &coords, previous_path_left, &offset_left);
            } else {
                click_on_file(ptr_user_data->right_path, all_files_right, &coords, previous_path_right, &offset_right);
            }
            
        } 
        else if (ch == 16) {                                      // Ctrl + p - переход на сохраненный ранее путь
        // else if (ch == 'p') {                                      // Ctrl + p - переход на сохраненный ранее путь
            if (active) {
                char temp[1024];
                strcpy(temp, ptr_user_data->left_path);
                strcpy(ptr_user_data->left_path, previous_path_left);
                strcpy(previous_path_left, temp);
            } else {
                char temp[1024];
                strcpy(temp, ptr_user_data->right_path);
                strcpy(ptr_user_data->right_path, previous_path_right);
                strcpy(previous_path_right, temp);
            }
        } 
        else if (ch == ('V' & 0x1F)) {                            // Обработка нажатия Ctrl + V
        // // else if (ch == 'v') {                            // Обработка нажатия Ctrl + V
            
            if (active) {
                open_in_vim(ptr_user_data->left_path, all_files_left, &coords, win_left);
            } else {
                open_in_vim(ptr_user_data->right_path, all_files_right, &coords, win_right);
            }
        } 

        else if (ch == 1 || ch == 'a') {                                           // ctrl + a
        // else if (ch == 'a') {
            bool_win_command = !bool_win_command;
        }
    }

    endwin();
    free(ptr_user_data);
    free(all_files_left);
    free(all_files_right);
    return 0;
}




        


