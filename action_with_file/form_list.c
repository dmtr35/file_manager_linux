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






void processing_list_files(struct user_data *ptr_user_data, struct file_data *all_files, char *path, char *file_name, _Bool active, int *quantity_lines, int *offset, int *check_empty, _Bool *save_files)
{
    
    if (*check_empty) {
        for(int i = 0; i < *quantity_lines; ++i) {
            for(int j = 0; j < ptr_user_data->coordinates.leng_arr_coorsor; ++j) {
                if(all_files[i].file_id == ptr_user_data->arr_coorsor_struct.arr[j]) {
                    size_t leng_file_name = strlen(all_files[i].name) + 1;
                    char file_name[leng_file_name];
                    strcpy(file_name, all_files[i].name);
                    
                    select_way(ptr_user_data, path, file_name, active, quantity_lines, offset, save_files);
                }
            }
        }
    } else {
        if(!(strcmp(file_name, "..") == 0)) {
            select_way(ptr_user_data, path, file_name, active, quantity_lines, offset, save_files);
        }
    }
    if (offset < 0) {
        offset = 0;
    }
    fillWithZeros(ptr_user_data->arr_coorsor_struct.arr, ptr_user_data->arr_coorsor_struct.size);
    ptr_user_data->coordinates.leng_arr_coorsor = 0;
}



void select_way(struct user_data *ptr_user_data, char *path, char *file_name, _Bool active, int *quantity_lines, int *offset, _Bool *save_files)
{   
    int count_item_arr = count_non_zero_elements(ptr_user_data->arr_coorsor_struct.arr, ptr_user_data->arr_coorsor_struct.size);
    if (ptr_user_data->set_bool.restore_files) {
        restore(ptr_user_data, path, file_name, active);
    } else if (ptr_user_data->set_bool.copy_files || ptr_user_data->set_bool.move_files){
        cp_mv_file(ptr_user_data, path, file_name, active);
        if (ptr_user_data->set_bool.move_files) {
            remove_directory_recursive(ptr_user_data, path, file_name, save_files);
        }
    } else if (ptr_user_data->set_bool.delete_files){
        remove_directory_recursive(ptr_user_data, path, file_name, save_files);
    }
}
