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

void render_ls(user_data *ptr_user_data, file_data *all_files, _Bool active, _Bool check_side, WINDOW *win)
{
    int *height = &ptr_user_data->coordinates.height;
    int *width = &ptr_user_data->coordinates.width;
    int *height_win = &ptr_user_data->coordinates.height_win;
    int *width_win = &ptr_user_data->coordinates.width_win;

    getmaxyx(win, *height_win, *width_win);
    char path[256];
    strcpy(path, check_side ? ptr_user_data->left_path : ptr_user_data->right_path);

    int offset = 0;
    int quantity_lines = 0;
    _Bool *hidden_files = check_side ? &(ptr_user_data->set_bool.hidden_left_bool) : &(ptr_user_data->set_bool.hidden_right_bool);

    ls_list(ptr_user_data, all_files, path, check_side, hidden_files, &quantity_lines);

    if (check_side) {
        ptr_user_data->coordinates.quantity_lines_left = quantity_lines;
        offset = ptr_user_data->coordinates.offset_left;
    } else {
        ptr_user_data->coordinates.quantity_lines_right = quantity_lines;
        offset = ptr_user_data->coordinates.offset_right;
    }

    file_data *all_files_ls = (file_data *)malloc(500 * sizeof(file_data));
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



    int max_length = ptr_user_data->coordinates.width_win - 35;
    trim_filename(all_files_ls, quantity_lines, max_length);
    // char qqq[256];
    // strcpy(qqq, all_files_ls[2].name);

    if(check_side) {
        mvwvline(win, 1, *width_win - 34, ACS_VLINE, *height - 4);
        mvwvline(win, 1, *width_win - 27, ACS_VLINE, *height - 4);
        mvwvline(win, 1, *width_win - 12, ACS_VLINE, *height - 4);
    } else {
        mvwvline(win, 1, (*width_win - 33) - 1, ACS_VLINE, *height - 4);
        mvwvline(win, 1, (*width_win - 26) - 1, ACS_VLINE, *height - 4);
        mvwvline(win, 1, (*width_win - 11) - 1, ACS_VLINE, *height - 4);
    }
    mvwhline(win, *height_win - 3, 1, ACS_HLINE, *width_win - 2);
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    wattron(win, A_BOLD);
    wattron(win, COLOR_PAIR(6));
    // mvwprintw(win, 0, 1, path);
    mvwprintw(win, 0, 1, "%s", path);
    wattroff(win, COLOR_PAIR(6));
    wattroff(win, A_BOLD);

    int last_line = (*height_win - 4);
    int i = 0 + offset;
    int row = 1;
    for (; i < quantity_lines && i < (*height_win - 4) + offset; ++i, ++row) {
        wattron(win, A_BOLD);
        if (strstr(all_files[i].permissions, "d") != NULL || strcmp(all_files[i].name, "..") == 0) {
            if (all_files[i].name[0] == '.' || strcmp(all_files[i].name, "..") == 0) {
                wattroff(win, A_BOLD);
                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, 1, "%s", all_files_ls[i].name);
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));
            } else {
                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, 1, "%s", all_files_ls[i].name);
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));
            }
// ==========================================================================================================  
        } else if((strstr(all_files[i].permissions, "l") != NULL)) {
            char target_name[1024];
            char target_path[1024];
            char full_target_path[1024];
            int col = 1;
            strcpy(target_name, all_files[i].name);
            strcpy(full_target_path, all_files_ls[i].name);
            
            split_link(target_name, target_path, target_name);

            int max_leng = strlen(full_target_path) - strlen(target_name) - 4;
            if (max_leng < 0) {
                max_leng = 0;
            }
            char cut_target_path[max_leng + 1];
            strncpy(cut_target_path, target_path, max_leng);
            cut_target_path[max_leng] = '\0';


            wattroff(win, A_BOLD);
            if (access(target_path, F_OK) != -1 && (is_directory(target_path)) == 0) {
            // Файл существует
                active ? wattron(win, COLOR_PAIR(3)) : wattron(win, COLOR_PAIR(13));
                mvwprintw(win, row, col, "%s", target_name);
                col += strlen(target_name);
                active ? wattroff(win, COLOR_PAIR(3)) : wattroff(win, COLOR_PAIR(13));

                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, col, " -> ");
                col += 4;
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));

                active ? wattron(win, COLOR_PAIR(3)) : wattron(win, COLOR_PAIR(13));
                mvwprintw(win, row, col, "%s", cut_target_path);
                col += strlen(cut_target_path);
                active ? wattroff(win, COLOR_PAIR(3)) : wattroff(win, COLOR_PAIR(13));
            } else if(is_directory(target_path)) {
                wattron(win, A_BOLD);
                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, col, "%s", target_name);
                col += strlen(target_name);
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));
                wattroff(win, A_BOLD);

                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, col, " -> ");
                col += 4;
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));

                active ? wattron(win, COLOR_PAIR(3)) : wattron(win, COLOR_PAIR(13));
                mvwprintw(win, row, col, "%s", cut_target_path);
                col += strlen(cut_target_path);
                active ? wattroff(win, COLOR_PAIR(3)) : wattroff(win, COLOR_PAIR(13));
            } else {
            // Файл не существует
                active ? wattron(win, COLOR_PAIR(5)) : wattron(win, COLOR_PAIR(15));
                mvwprintw(win, row, col, "%s", target_name);
                col += strlen(target_name);
                active ? wattroff(win, COLOR_PAIR(5)) : wattroff(win, COLOR_PAIR(15));

                active ? wattron(win, COLOR_PAIR(1)) : wattron(win, COLOR_PAIR(11));
                mvwprintw(win, row, col, " -> ");
                col += 4;
                active ? wattroff(win, COLOR_PAIR(1)) : wattroff(win, COLOR_PAIR(11));

                active ? wattron(win, COLOR_PAIR(3)) : wattron(win, COLOR_PAIR(13));
                mvwprintw(win, row, col, "%s", cut_target_path);
                col += strlen(cut_target_path);
                active ? wattroff(win, COLOR_PAIR(3)) : wattroff(win, COLOR_PAIR(13));
            }
// ==========================================================================================================  
        } else if((strstr(all_files[i].name, ".zip") != NULL) || (strstr(all_files[i].name, ".gz") != NULL) || (strstr(all_files[i].name, ".tar") != NULL)) {
            active ? wattron(win, COLOR_PAIR(5)) : wattron(win, COLOR_PAIR(15));
            mvwprintw(win, row, 1, "%s", all_files_ls[i].name);
            active ? wattroff(win, COLOR_PAIR(5)) : wattroff(win, COLOR_PAIR(15));
        } else if (all_files[i].name[0] == '.'){
            wattroff(win, A_BOLD);
            active ? wattron(win, COLOR_PAIR(2)) : wattron(win, COLOR_PAIR(12));  
            mvwprintw(win, row, 1, "%s", all_files_ls[i].name);
            active ? wattroff(win, COLOR_PAIR(2)) : wattroff(win, COLOR_PAIR(12));
        } else if (all_files[i].permissions[9] == 'x'){
            active ? wattron(win, COLOR_PAIR(4)) : wattron(win, COLOR_PAIR(14));
            mvwprintw(win, row, 1, "%s", all_files_ls[i].name);
            active ? wattroff(win, COLOR_PAIR(4)) : wattroff(win, COLOR_PAIR(14));
        } else {
            active ? wattron(win, COLOR_PAIR(2)) : wattron(win, COLOR_PAIR(12));
            mvwprintw(win, row, 1, "%s", all_files_ls[i].name);
            active ? wattroff(win, COLOR_PAIR(2)) : wattroff(win, COLOR_PAIR(12));
        }
        if(check_side) {
            mvwprintw(win, row, *width / 2 - 33, "%s", all_files_ls[i].size);
            mvwprintw(win, row, *width / 2 - 26, "%s", all_files_ls[i].time);
            mvwprintw(win, row, *width / 2 - 11, "%s", all_files_ls[i].permissions);
        } else {
            if (*width_win % 2 == 0 && *width % 2 == 0 || *width_win % 2 != 0 && *width % 2 == 0) {
                mvwprintw(win, row, *width / 2 - 33, "%s", all_files_ls[i].size);
                mvwprintw(win, row, *width / 2 - 26, "%s", all_files_ls[i].time);
                mvwprintw(win, row, *width / 2 - 11, "%s", all_files_ls[i].permissions);
            } else if (*width_win % 2 != 0 && *width % 2 != 0 || *width_win % 2 == 0 && *width % 2 != 0) {
                mvwprintw(win, row, *width / 2 - 32, "%s", all_files_ls[i].size);
                mvwprintw(win, row, *width / 2 - 25, "%s", all_files_ls[i].time);
                mvwprintw(win, row, *width / 2 - 10, "%s", all_files_ls[i].permissions);
            } 
        }
        wattroff(win, A_BOLD);
    }





    
    int j = 0;
    if (active) {
        for(row = 1, i = 0 + offset; i < quantity_lines && row <= last_line; ++i, ++row) {
            for(int j = 0; j < ptr_user_data->coordinates.leng_arr_coorsor; ++j) {
                int item_arr = ptr_user_data->arr_coorsor_struct.arr[j];
                int file_id = all_files[i].file_id;
                if(file_id == item_arr && item_arr > 0) {
                    wattron(win, A_BOLD);
                    wattron(win, COLOR_PAIR(11));                       // Включаем цветовую пару для всей строки
                    mvwhline(win, row, 1, ' ', *width_win - 2); // Заполняем строку пробелами для очистки ее содержимого
                    if (check_side) {
                        mvwprintw(win, row, 1, "%s", all_files_ls[i].name);  // Выводим текст
                        mvwprintw(win, row, (*width / 2 - 33), "%s", all_files_ls[i].size);
                        mvwprintw(win, row, (*width / 2 - 26), "%s", all_files_ls[i].time);
                        mvwprintw(win, row, (*width / 2 - 11), "%s", all_files_ls[i].permissions);
                    } else {
                        if (*width_win % 2 == 0 && *width % 2 == 0 || *width_win % 2 != 0 && *width % 2 == 0) {
                            mvwprintw(win, row, 1, "%s", all_files_ls[i].name);  // Выводим текст
                            mvwprintw(win, row, *width / 2 - 33, "%s", all_files_ls[i].size);
                            mvwprintw(win, row, *width / 2 - 26, "%s", all_files_ls[i].time);
                            mvwprintw(win, row, *width / 2 - 11, "%s", all_files_ls[i].permissions);
                        } else if (*width_win % 2 != 0 && *width % 2 != 0 || *width_win % 2 == 0 && *width % 2 != 0) {
                            mvwprintw(win, row, 1, "%s", all_files_ls[i].name);  // Выводим текст
                            mvwprintw(win, row, *width / 2 - 32, "%s", all_files_ls[i].size);
                            mvwprintw(win, row, *width / 2 - 25, "%s", all_files_ls[i].time);
                            mvwprintw(win, row, *width / 2 - 10, "%s", all_files_ls[i].permissions);
                        }
                    }
                }
            }
        }
        for (row = 1, i = 0 + offset; i < quantity_lines && row <= last_line; ++i, ++row) {
            if (row == ptr_user_data->coordinates.cursor_y) {
                wattron(win, A_BOLD);
                wattron(win, COLOR_PAIR(6));                       // Включаем цветовую пару для всей строки
                mvwhline(win, row, 1, ' ', *width_win - 2); // Заполняем строку пробелами для очистки ее содержимого
                if (check_side) {
                    mvwprintw(win, row, 1, "%s", all_files_ls[i].name);  // Выводим текст
                    mvwprintw(win, row, (*width / 2 - 33), "%s", all_files_ls[i].size);
                    mvwprintw(win, row, (*width / 2 - 26), "%s", all_files_ls[i].time);
                    mvwprintw(win, row, (*width / 2 - 11), "%s", all_files_ls[i].permissions);

                    if (strcmp(all_files[i].name, "..") == 0) {

                    } else {
                        mvwprintw(win, *height_win - 2, 1, "%s", all_files[i].name);
                    }
                } else {
                    if (*width_win % 2 == 0 && *width % 2 == 0 || *width_win % 2 != 0 && *width % 2 == 0) {
                        mvwprintw(win, row, 1, "%s", all_files_ls[i].name);  // Выводим текст
                        mvwprintw(win, row, *width / 2 - 33, "%s", all_files_ls[i].size);
                        mvwprintw(win, row, *width / 2 - 26, "%s", all_files_ls[i].time);
                        mvwprintw(win, row, *width / 2 - 11, "%s", all_files_ls[i].permissions);
                    } else if (*width_win % 2 != 0 && *width % 2 != 0 || *width_win % 2 == 0 && *width % 2 != 0) {
                        mvwprintw(win, row, 1, "%s", all_files_ls[i].name);  // Выводим текст
                        mvwprintw(win, row, *width / 2 - 32, "%s", all_files_ls[i].size);
                        mvwprintw(win, row, *width / 2 - 25, "%s", all_files_ls[i].time);
                        mvwprintw(win, row, *width / 2 - 10, "%s", all_files_ls[i].permissions);
                    } 

                    if (strcmp(all_files[i].name, "..") == 0) {

                    } else {
                        mvwprintw(win, *height_win - 2, 1, "%s", all_files[i].name);
                    }
                }
                wattroff(win, COLOR_PAIR(6)); // Отключаем цветовую пару
                wattroff(win, A_BOLD);

                wmove(win, ptr_user_data->coordinates.cursor_y, ptr_user_data->coordinates.cursor_x); // Возвращаем курсор на его предыдущее местоположение
            } 
        }
    }

    wrefresh(win);

    free(all_files_ls);
}


void trim_filename(file_data *all_files_ls, int number_lines, int max_length) {
    for(int i = 0; i < number_lines; ++i) {
        int name_length = strlen(all_files_ls[i].name);
        all_files_ls[i].name[max_length] = '\0';
    }
}
