#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>


#include "func.h"


int check_func(struct user_data *ptr_user_data)
{
    uid_t euid = geteuid();               // Получаем UID


    if(euid == 0) {
        const char *sudo_user = getenv("SUDO_USER");

        if(sudo_user != NULL) {
            strcpy(ptr_user_data->user, sudo_user);
        }else{
            printf("%s not determined\n", sudo_user);
            return 1;
        }

    } else {
        const char *user = getenv("USER");
        if(user != NULL) {
            strcpy(ptr_user_data->user, user);
        } else {
            printf("%s not determined\n", user);
            return 1;
        }
    }
    
    // получаем домашнюю директорию
    struct passwd *pw;

    if((pw = getpwnam(ptr_user_data->user)) != NULL) {
        strcpy(ptr_user_data->right_path, pw->pw_dir);
        strcpy(ptr_user_data->home_path, pw->pw_dir);
    } else {
        fprintf(stderr, "Unable to determine right_path directory\n");
        return 1;
    }


    // получаем текущюю директорию
    if (getcwd(ptr_user_data->left_path, 1024) == NULL) {
        perror("Error getting current directory");
        return 1;
    }

    return 0;

}


