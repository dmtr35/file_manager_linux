#ifndef FUNC_H
#define FUNC_H

#include <stdint.h>
#include <time.h>
#include <ncurses.h>


struct user_data {
    char user[1024];
    char home[1024];
    char current_directory[1024];
};

struct file_data {
    char name[1024];
    char size[7];
    char time[15];
    char permissions[11];
};

struct coordinates {
    int width;
    int height;
    int height_win;
    int width_win;
    int cursor_x;
    int cursor_y;
};

// struct coordinates_left {
//     int height_win_left;
//     int width_win_left;
//     int cursor_left_x;
//     int cursor_left_y;
// };

// struct coordinates_right {
//     int height_win_right;
//     int width_win_right;
//     int cursor_right_x;
//     int cursor_right_y;
// };


// check_func.c
int check_func(struct user_data *ptr_user_data);


// extra.c
char *human_readable_size(uintmax_t size, char *buf);
char* format_last_modification_time(time_t time, char *buf);


// list_builder.c
int ls_list(char *path, struct file_data *directories, int dir_count, _Bool flag_hidden_files);
void file_data(struct file_data *current_file, char *file_name, struct stat *file_info, char *symb);
void strtrim(char *str);


// rendering
void rendering(char *path, struct file_data *all_files, struct coordinates *coords, int number_lines, _Bool left_of_right, _Bool active, WINDOW *win);


#endif
