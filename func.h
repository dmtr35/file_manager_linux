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




// check_func.c
int check_func(struct user_data *ptr_user_data);
void render_command(struct user_data *ptr_user_data, struct coordinates *coords, _Bool left_of_right, _Bool active, _Bool *bool_win_command);

// extra.c
char *human_readable_size(uintmax_t size, char *buf);
char* format_last_modification_time(time_t time, char *buf);


// list_builder.c
int ls_list(char *path, struct file_data *directories, _Bool flag_hidden_files);
void file_data(struct file_data *current_file, char *file_name, struct stat *file_info, char *symb);
void strtrim(char *str);


// rendering_ls
int render_ls(char *path, struct file_data *all_files, struct coordinates *coords, _Bool flag_hidden_files, _Bool left_of_right, _Bool active, int offset, WINDOW *win);
void trim_filename(struct file_data *all_files, int number_lines, int max_length);


// extra_func
void remove_first_char(char *str);


// button_processing
void click_on_file(char *path, struct file_data *all_files, struct coordinates *coords, char *previous_path);
void open_in_vim(char *path, struct file_data *all_files, struct coordinates *coords, WINDOW *win);

// rendering_command.c
void rendering_comm(struct user_data *ptr_user_data, struct coordinates *coords, _Bool left_of_right, _Bool active, _Bool *bool_win_command, WINDOW *win);
void add_char_to_command_line(WINDOW *win, char c);
void remove_char_from_command_line(WINDOW *win, size_t cursor_coords);





#endif
