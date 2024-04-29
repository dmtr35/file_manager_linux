#define _GNU_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <stdint.h>

#include <fcntl.h>
#include <sys/time.h>
#include <sys/times.h>
#include <utime.h>


#include "../header.h"

#define EXEC_BIT ((short)1)
#define WRITE_BIT ((short)2)
#define READ_BIT ((short)4)



void change_permission(char *new_permission, char *path, char *name_file_row)
{
    size_t size_full_path = strlen(path) + strlen(name_file_row) + 2;
    char *full_path = malloc(size_full_path * sizeof(char));
    snprintf(full_path, size_full_path, "%s/%s", path, name_file_row);

    
    mode_t new_mode = parmission(new_permission, full_path);

    struct stat file_info;
    if (lstat(full_path, &file_info) == 0) {
        if (S_ISDIR(file_info.st_mode)) {
            chmod(full_path, new_mode);
        } else if (S_ISREG(file_info.st_mode)) {
            chmod(full_path, new_mode);
        } else if (S_ISLNK(file_info.st_mode)) {
            chmod(full_path, new_mode);
            free(full_path);
            return;
        } 
    }


    free(full_path);
}



mode_t parmission(char *new_permission, char *full_path)
{
    mode_t previous_permissions;
    struct stat file_stat;
    if (stat(full_path, &file_stat) == 0) {
        previous_permissions = file_stat.st_mode;
    } else {
        perror("Failed to get file permissions");
        return 0;
    }
    mode_t set_mode;


    unsigned short new_mode = 0;
    

    if (strstr(new_permission, "+") != NULL || strstr(new_permission, "-") != NULL || strstr(new_permission, "=") != NULL) {
        char *first_part = malloc(strlen(new_permission));
        char *second_part = malloc(strlen(new_permission));
        char *second_part_copy = malloc(strlen(new_permission));
        new_mode |= (previous_permissions);

        char sign[2];
        if (strstr(new_permission, "+") != NULL) {
            sign[0] = '+';
        } else if (strstr(new_permission, "-") != NULL) {
            sign[0] = '-';
        } else {
            sign[0] = '=';
        }
        sign[1] = '\0';

        if (new_permission[0] == '+' || new_permission[0] == '-' || new_permission[0] == '=') {
            int i = strlen(new_permission);
            for (; i > 0; --i) {
                new_permission[i] = new_permission[i-1];
            }
            new_permission[i] = 'a';
            new_permission[strlen(new_permission) + 1] = '\0';
        }

        char *buf = strstr(new_permission, sign);
        strncpy(first_part, new_permission, buf - new_permission);
        first_part[buf - new_permission] = '\0';
        strcpy(second_part, buf + 1);

        for (int i = 0; i < strlen(first_part); ++i) {
            if (strstr(second_part, "t") || strstr(second_part, "s")) {
                memset(second_part_copy, 0, sizeof(second_part_copy));

                int j = 0;
                for(int i = 0; i < strlen(second_part); ++i) {
                    if (second_part[i] == 't' || second_part[i] == 's') {
                        second_part_copy[j++] = second_part[i];
                    }
                }
                set_permission_s_and_t(first_part[i], sign, second_part_copy, &new_mode, previous_permissions);
            }
            set_permission_sign(first_part[i], sign, second_part, &new_mode, previous_permissions);
        }

        free(first_part);
        free(second_part);
        free(second_part_copy);
    } else if (strstr(new_permission, "0") != NULL || strstr(new_permission, "1") != NULL ||strstr(new_permission, "2") != NULL || strstr(new_permission, "3") != NULL || strstr(new_permission, "4") != NULL || strstr(new_permission, "5") != NULL || strstr(new_permission, "6") != NULL || strstr(new_permission, "7") != NULL ) {
        if (strlen(new_permission) == 3) {
            int i = strlen(new_permission);
            for (; i > 0; --i) {
                new_permission[i] = new_permission[i-1];
            }
            new_permission[i] = '0';
            new_permission[strlen(new_permission) + 1] = '\0';
        }
        set_permission_num(new_permission, &new_mode);
    }

    set_mode = new_mode;

    return new_mode;
}


void set_permission_sign(char first_part, char *sign, char *second_part, unsigned short *mode, const mode_t previous_permissions)
{
    unsigned char offset = 0;
    
    switch (first_part) {
        case 'a':
            offset = 6;
            for (int i = 0; i < 3; i++, offset -=  3) {
                second_args(second_part, sign, mode, previous_permissions, offset);
            }
            break;
        case 'u':
            offset = 6;
            second_args(second_part, sign, mode, previous_permissions, offset);
            break;
        case 'g':
            offset = 3;
            second_args(second_part, sign, mode, previous_permissions, offset);
            break;
        case 'o':
            offset = 0;
            second_args(second_part, sign, mode, previous_permissions, offset);
            break;
        default:
            break;
    }
}


void second_args(char *second_part, char *sign, unsigned short *mode, const mode_t previous_permissions, unsigned char offset)
{
    for (int i = 0; i < strlen(second_part); i++) {
        switch (second_part[i]) {
            case 'r':
                change_bit(sign, mode, previous_permissions, offset, READ_BIT);
                break;
            case 'w':
                change_bit(sign, mode, previous_permissions, offset, WRITE_BIT);
                break;
            case 'x':
                change_bit(sign, mode, previous_permissions, offset, EXEC_BIT);
                break;
            default:
                break;
        }
    }
}


void set_permission_s_and_t(char first_part, char *sign, char *second_part, unsigned short *mode, const mode_t previous_permissions)
{
    unsigned char offset = 9;
    
    switch (first_part) {
        case 'a':
            for(int i = 0; i < strlen(second_part); ++i) {
                    if (second_part[i] == 't') {
                        change_bit(sign, mode, previous_permissions, offset, EXEC_BIT);
                    } else if(second_part[i] == 's') {
                        change_bit(sign, mode, previous_permissions, offset, READ_BIT);
                        change_bit(sign, mode, previous_permissions, offset, WRITE_BIT);
                    }
            }
            break;
        case 'u':
            change_bit(sign, mode, previous_permissions, offset, READ_BIT);
            break;
        case 'g':
            change_bit(sign, mode, previous_permissions, offset, WRITE_BIT);
            break;
        case 'o':
            change_bit(sign, mode, previous_permissions, offset, EXEC_BIT);
            break;
        default:
            break;
    }
}


void change_bit(char *sign, unsigned short *mode, const mode_t previous_permissions, unsigned char offset, short exec_bit_value)
{
    if (strcmp(sign, "+") == 0) {
        *mode |= (previous_permissions | (exec_bit_value << offset)); 
    } else if (strcmp(sign, "-") == 0 && (previous_permissions & (exec_bit_value << offset))) {
        *mode ^= (exec_bit_value << offset);
    }
}


void set_permission_num(char *permission, unsigned short *mode)
{
    unsigned char offset = 9;
    for (int i = 0; i < strlen(permission); i++, offset -=  3) {
        switch (permission[i]) {
            case '0':
                break;
            case '1':
                *mode |= (EXEC_BIT) << offset;
                break;
            case '2':
                *mode |= (WRITE_BIT) << offset; 
                break;
            case '3':
                *mode |= (WRITE_BIT | EXEC_BIT) << offset;
                break;
            case '4':
                *mode |= READ_BIT << offset;
                break;
            case '5':
                *mode |= (READ_BIT | EXEC_BIT) << offset;
                break;
            case '6':
                *mode |= (READ_BIT | WRITE_BIT) << offset;
                break;
            case '7':
                *mode |= (READ_BIT | WRITE_BIT | EXEC_BIT) << offset;
                break;
            default:
                break;
        }
    }
}


