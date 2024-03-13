#ifndef FUNC_H
#define FUNC_H

#include <stdint.h>
#include <time.h>
#include <ncurses.h>


struct user_data {
    char user[1024];
    char left_path[1024];
    char right_path[1024];
    char home_path[1024];
    char coorsor_file[1024];
};

struct set_bool {
    _Bool hidden_files_bool;
    _Bool command_bool;
    _Bool menu_bool;
    _Bool help_bool;
    _Bool out_bool;
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
    int height_menu;
    int width_menu;
    int cursor_x;
    int cursor_y;

    int quantity_lines_left;
    int quantity_lines_right;

    int offset_left;
    int offset_right;
};




// check_func.c
int check_func(struct user_data *ptr_user_data);

// extra_func.c
int is_directory(const char *path);
char *human_readable_size(uintmax_t size, char *buf);
char *format_last_modification_time(time_t time, char *buf);
void remove_first_char(char *str);
void removeDuplicates(int *arr, size_t size);
void addToArr(int *arr, size_t size, int value);
void removeFromArr(int *arr, size_t size, int value);
bool containsElement(int *arr, size_t size, int value);
void fillWithZeros(int *arr, size_t size);


// list_builder.c
int ls_list(char *path, struct file_data *directories, _Bool flag_hidden_files, int *quantity_lines);
void file_data(struct file_data *current_file, char *file_name, struct stat *file_info, char *symb);
void strtrim(char *str);


// rendering_ls
void render_ls(char *path, struct file_data *all_files, struct coordinates *coords, struct set_bool *set_bool, _Bool active, _Bool check_side, int *arr_coorsor, size_t leng_arr_coorsor, WINDOW *win);
void trim_filename(struct file_data *all_files, int number_lines, int max_length);
void render_comm_line(struct user_data *ptr_user_data, struct file_data *all_files, struct coordinates *coords, struct set_bool *set_bool, _Bool active, _Bool check_side, int *arr_coorsor, size_t leng_arr_coorsor, WINDOW *win_left, WINDOW *win_right);
void render_help(char *path, struct file_data *all_files, struct coordinates *coords, struct set_bool *set_bool, _Bool active, int *arr_coorsor, size_t leng_arr_coorsor, WINDOW *win);
void render_menu(struct user_data *ptr_user_data, struct file_data *all_files_left, struct file_data *all_files_right, struct set_bool *set_bool, int *coords_cursor_y_menu, struct coordinates *coords, _Bool active, _Bool check_side, _Bool turn_render_ls, int *arr_coorsor, size_t leng_arr_coorsor, WINDOW *win_menu, WINDOW *win_right, WINDOW *win_left);




// button_processing
void click_on_file(char *path, struct file_data *all_files, struct coordinates *coords, char *previous_path, _Bool check_side);
void backspace(char *path, struct file_data *all_files, struct coordinates *coords, char *previous_path, _Bool check_side);
void open_in_vim(char *path, struct file_data *all_files, struct coordinates *coords, _Bool check_side, WINDOW *win);


// command_line.c
// buffer_save.c
void add_char_to_command_line(WINDOW *win_left, char c, char *screen_buffer, int *buffer_pos);
// void add_char_to_command_line(WINDOW *win, char c, int x, int y);
void remove_char_from_command_line(WINDOW *win_left, size_t cursor_coords, char *screen_buffer, int *buffer_pos);
void save_to_buffer(const char *text, char *screen_buffer, int *buffer_pos);
void restore_from_buffer(WINDOW *win, char *screen_buffer);
void restore_from_buffer_offset(WINDOW *win, char *screen_buffer, int offset);


// rendering_help


// rendering_menu
// void render_menu(struct user_data *ptr_user_data, struct file_data *all_files, struct coordinates *coords, _Bool active, _Bool *menu_bool, WINDOW *win);

// remove_file
void remove_one_file(const char *path);
void remove_directory_recursive(const char *path, char *file_name);

#endif
