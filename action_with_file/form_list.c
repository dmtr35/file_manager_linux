#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <libgen.h>

#include "../func.h"






void processing_list_files(char *path, char *file_name, int *arr_coorsor, _Bool active, struct file_data *all_files, struct coordinates *coords, struct set_bool *set_bool, struct user_data *ptr_user_data, int leng_arr_coorsor_full, int *quantity_lines, int *offset, int *check_empty, _Bool *save_files)
{
    
    if (*check_empty) {
        for(int i = 0; i < *quantity_lines; ++i) {
            for(int j = 0; j < coords->leng_arr_coorsor; ++j) {
                if(all_files[i].file_id == arr_coorsor[j]) {
                    size_t leng_file_name = strlen(all_files[i].name) + 1;
                    char file_name[leng_file_name];
                    strcpy(file_name, all_files[i].name);
                    
                    select_way(path, file_name, set_bool, ptr_user_data, coords, active, arr_coorsor, leng_arr_coorsor_full, quantity_lines, offset, save_files);
                }
            }
        }
    } else {
        if(!(strcmp(file_name, "..") == 0)) {
            select_way(path, file_name, set_bool, ptr_user_data, coords, active, arr_coorsor, leng_arr_coorsor_full, quantity_lines, offset, save_files);
        }
    }
    if (offset < 0) {
        offset = 0;
    }
    fillWithZeros(arr_coorsor, coords, leng_arr_coorsor_full);
}



void select_way(char *path, char *file_name, struct set_bool *set_bool, struct user_data *ptr_user_data, struct coordinates *coords, _Bool active, int *arr_coorsor, int leng_arr_coorsor_full, int *quantity_lines, int *offset, _Bool *save_files)
{   
    int count_item_arr = count_non_zero_elements(arr_coorsor, leng_arr_coorsor_full);
    if (set_bool->restore_files) {
        restore(path, file_name, ptr_user_data, coords, active);
    } else if (set_bool->copy_files || set_bool->move_files){
        cp_mv_file(path, file_name, set_bool, ptr_user_data, active);
    } else if (set_bool->delete_files){
        remove_directory_recursive(path, file_name, set_bool, ptr_user_data, save_files);
    }
}
