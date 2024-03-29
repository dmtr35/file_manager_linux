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






void processing_list_files(char *path, char *file_name, int *arr_coorsor, _Bool active, struct file_data *all_files, struct coordinates *coords, struct set_bool *set_bool, struct user_data *ptr_user_data, int leng_arr_coorsor_full, int *quantity_lines, int *offset)
{
    int check_empty = check_int_arr(arr_coorsor, leng_arr_coorsor_full);
    
    if (check_empty) {
        if(!(strcmp(file_name, "..") == 0)) {
            select_way(path, file_name, set_bool, ptr_user_data, coords, active, arr_coorsor, leng_arr_coorsor_full, quantity_lines, offset);
            // if (*quantity_lines == (coords->cursor_y + *offset)) {
            //     coords->cursor_y--;
            // }
        }
        if (offset < 0) {
           offset = 0;
        }
    } else {
        for(int i = 0; i < *quantity_lines; ++i) {
            for(int j = 0; j < coords->leng_arr_coorsor; ++j) {
                if(all_files[i].file_id == arr_coorsor[j]) {
                    int tt;
                    tt = all_files[i].file_id;
                    size_t leng_file_name = strlen(all_files[i].name) + 1;
                    char file_name[leng_file_name];
                    strcpy(file_name, all_files[i].name);
                    
                    select_way(path, file_name, set_bool, ptr_user_data, coords, active, arr_coorsor, leng_arr_coorsor_full, quantity_lines, offset);
                }
            }
        }
    }
    fillWithZeros(arr_coorsor, coords, leng_arr_coorsor_full);

}



void select_way(char *path, char *file_name, struct set_bool *set_bool, struct user_data *ptr_user_data, struct coordinates *coords, _Bool active, int *arr_coorsor, int leng_arr_coorsor_full, int *quantity_lines, int *offset)
{   
    int count_item_arr = count_non_zero_elements(arr_coorsor, leng_arr_coorsor_full);
    if (set_bool->restore_files) {
        restore(path, file_name, ptr_user_data, coords, active);
    } else if (set_bool->move_files){
        // remove_directory_recursive(path, file_name, set_bool, ptr_user_data);
    } else if (set_bool->copy_files){
        // remove_directory_recursive(path, file_name, set_bool, ptr_user_data);
    } else if (set_bool->delete_files){
        remove_directory_recursive(path, file_name, set_bool, ptr_user_data);
        // if (coords->cursor_y + *offset > *quantity_lines - count_item_arr && *offset != 0) {
        //     *offset -= count_item_arr;
        // } else if (coords->cursor_y + *offset > *quantity_lines - count_item_arr && *offset == 0) {
        //     coords->cursor_y = *quantity_lines - count_item_arr - *offset;
        // }
    }
}
