#ifndef FUNC_H
#define FUNC_H

#include <stdint.h>
#include <time.h>
#include <ncurses.h>

#define MAX_ARR_SIZE 500
#define MAX_ARR_SIZE_SAVE_PATH 5
#define MAX_PATH_LENGTH 1024
// #define ARROW " --> "

#define MAX_FILENAME_LENGTH 1024



typedef struct file_data {
    int file_id;
    int leng_all_files;
    char name[1024];
    char size[7];
    char time[15];
    char permissions[11];
} file_data;

struct set_bool {
    _Bool hidden_left_bool;
    _Bool hidden_right_bool;
    _Bool command_bool;
    _Bool menu_bool;
    _Bool save_path_bool;
    _Bool create_bool;
    _Bool enter_name_bool;

    _Bool help_bool;
    _Bool out_bool;
    _Bool restore_files;
    _Bool move_files;
    _Bool copy_files;
    _Bool delete_files;

    _Bool check_active_tab_bool;
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
    int cursor_left;
    int cursor_right;
    int quantity_lines_left;
    int quantity_lines_right;
    int leng_arr_coorsor;
    int offset_left;
    int offset_right;
    int coords_cursor_y_menu;
};

struct IntArray {
    int arr[MAX_ARR_SIZE];
    size_t size;
};

struct Save_Path_Array {
    char save_paths_arr[MAX_ARR_SIZE_SAVE_PATH][MAX_PATH_LENGTH];
};

typedef struct user_data {
    char user[1024];
    char left_path[1024];
    char right_path[1024];
    char previous_path_left[1024];
    char previous_path_right[1024];
    char home_path[1024];
    char coorsor_file[1024];
    char trash_directory[1024];
    struct set_bool set_bool;
    struct coordinates coordinates;
    struct IntArray arr_coorsor_struct;
    struct Save_Path_Array save_paths;
} user_data;

// check_func.c
int check_func(user_data *ptr_user_data);
void check_and_create_trash(user_data *ptr_user_data);

// extra_func.c
int item(user_data *ptr_user_data, _Bool active);                       // получить i
int is_directory(const char *path);
char *get_current_datatime(void);
char *replace_slashes_dash(char *path);
void extractFileNameAndPath(char *fullFileName, char *path);
char *human_readable_size(uintmax_t size, char *buf);
char *format_last_modification_time(time_t time, char *buf);
void remove_first_char(char *str);
void removeDuplicates(int *arr, size_t size);
void addToArr(int *arr, size_t size, int value);
void removeFromArr(int *arr, size_t size, int value);
_Bool containsElement(int *arr, size_t size, int value);
void fillWithZeros(int *arr, size_t size);
int check_int_arr(int *arr, size_t size);                               // проверка массива, есть ли в нем элементы
void remove_first_element(int *arr, size_t size);                       // удалить первый элемент массива
int longest(user_data *ptr_user_data, size_t leng_path);                // определяем какой размер самой длинной строки в массиве
int count_non_zero_elements(int *arr, size_t size);                     // сколько элементов в массиве
void check_offset_less_zero(int *offset);
void check_cursor_y_less_zero(int *cursor_y);
void split_link(char *full_name, char *path_link, char *name_link);     // разделение link на имя и ссылку
char *full_name_for_link(const char *name, const char *path);           // полное имя с сылкой на линк
void link_to_file(char *full_path);                                     // достать ссылку на файл из ссылки
void addAllToArr(int *arr, size_t size, int item);                          // добавить все элементы в массив


// list_builder.c
int ls_list(user_data *ptr_user_data, file_data *directories, _Bool check_side, _Bool *flag_hidden_files, int *quantity_lines);
void form_current_file(file_data *current_file, char *file_name, struct stat *file_info, char *symb, int file_id);
void strtrim(char *str);


// rendering_ls
void render_ls(user_data *ptr_user_data, file_data *all_files, _Bool active, _Bool check_side, WINDOW *win);
void trim_filename(file_data *all_files, int number_lines, int max_length);
void render_comm_line(user_data *ptr_user_data, file_data *all_files, _Bool active, _Bool check_side, WINDOW *win_left, WINDOW *win_right);
void render_help(user_data *ptr_user_data, file_data *all_files, _Bool active, WINDOW *win);
void render_menu(user_data *ptr_user_data, file_data *all_files_left, file_data *all_files_right, _Bool active, _Bool check_side, _Bool turn_render_ls, WINDOW *win_menu, WINDOW *win_right, WINDOW *win_left);
void render_save_path(user_data *ptr_user_data, file_data *all_files_left, file_data *all_files_right, _Bool active, _Bool check_side, _Bool turn_render_ls, WINDOW *win_menu, WINDOW *win_right, WINDOW *win_left);
void render_create(user_data *ptr_user_data, file_data *all_files_left, file_data *all_files_right, _Bool active, _Bool check_side, _Bool turn_render_ls, WINDOW *win_menu, WINDOW *win_right, WINDOW *win_left);

void select_coorsor(user_data *ptr_user_data, file_data *all_files, int *quantity_lines, int *offset, int count_item_arr, int *check_empty);
void render_ls_and_menu(user_data *ptr_user_data, file_data *all_files_left, file_data *all_files_right, _Bool turn_render_ls, _Bool active, _Bool check_side, _Bool *is_enter_pressed, int *coords_cursor_y_menu, WINDOW *win_menu, WINDOW *win_right, WINDOW *win_left);
void render_ls_and_save_path(user_data *ptr_user_data, file_data *all_files_left, file_data *all_files_right, _Bool turn_render_ls, _Bool active, _Bool check_side, _Bool *is_enter_pressed, int *coords_cursor_y_menu, WINDOW *win_menu, WINDOW *win_right, WINDOW *win_left);
void render_ls_and_create(user_data *ptr_user_data, file_data *all_files_left, file_data *all_files_right, _Bool turn_render_ls, _Bool active, _Bool check_side, _Bool *is_enter_pressed, int *coords_cursor_y_menu, WINDOW *win_menu, WINDOW *win_right, WINDOW *win_left);

// create.c
void mkdir_p(char *file_name, char *path);
void touch_file(char *file_name, char *path);
void create_link(char *file_name, char *path, char *name_file_row);
void split_and_creation_link(char *source_directory, char *destination_directory);
void rename_file(char *new_file_name, char *path, char *name_file_row);
void change_permission(char *new_file_name, char *path, char *name_file_row);
mode_t parmission(char *new_permission, char *full_path);

// button_processing
void backspace(user_data *ptr_user_data, file_data *all_files, _Bool active, _Bool check_side);
void click_on_file(user_data *ptr_user_data, file_data *all_files, _Bool active, _Bool check_side);
void open_in_vim(user_data *ptr_user_data, file_data *all_files, _Bool check_side, WINDOW *win);


// command_line.c
void add_char_to_command_line(WINDOW *win_left, char c, char *screen_buffer, int *buffer_pos);
void remove_char_from_command_line(WINDOW *win_left, size_t cursor_coords, char *screen_buffer, int *buffer_pos);
void save_to_buffer(const char *text, char *screen_buffer, int *buffer_pos);
void restore_from_buffer(WINDOW *win, char *screen_buffer);
void restore_from_buffer_offset(WINDOW *win, char *screen_buffer, int offset);
// buffer_save.c
void add_char_to_enter_name(WINDOW *win, int ch, char *screen_buffer, int *buffer_pos);
void delete_char_from_enter_name(WINDOW *win, char *screen_buffer, int *buffer_pos);


// rendering_help



// remove_file
void remove_directory_recursive(user_data *ptr_user_data, char *path, char *file_name, _Bool *save_files);
void remove_one_file(char *path);
void save_file(user_data *ptr_user_data, char *path, char *file_name, char *ptr_full_path);
void restore(user_data *ptr_user_data, char *path, char *file_name, _Bool active);
void cp_mv_file(user_data *ptr_user_data, file_data *all_files, char *path, char *file_name, _Bool active);
void copy_file(user_data *ptr_user_data, file_data *all_files, char *source_directory, char *destination_directory, _Bool active);
void copy_directory(user_data *ptr_user_data, file_data *all_files, char *source_directory, char *destination_directory, _Bool active);

// form_list.c
void processing_list_files(user_data *ptr_user_data, file_data *all_files, char *path, char *file_name, _Bool active, int *quantity_lines, int *offset, _Bool *save_files);
void select_way(user_data *ptr_user_data, file_data *all_files, char *path, char *file_name, _Bool active, int *quantity_lines, int *offset, _Bool *save_files);





#endif
