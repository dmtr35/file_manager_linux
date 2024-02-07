#ifndef FUNC_H
#define FUNC_H

#include <stdint.h>
#include <time.h>

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

// check_func.c
int check_func(struct user_data *ptr_user_data);

// extra.c
char *human_readable_size(uintmax_t size, char *buf);
char* format_last_modification_time(time_t time, char *buf);

// list_builder.c
int ls_list(char *path, _Bool flag_hidden_files);
int rendering(struct file_data *directories, struct file_data *files, int dir_count, int file_count);

#endif
