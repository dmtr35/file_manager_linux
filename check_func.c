#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>


#include "func.h"


int check_func(user_data *ptr_user_data)
{
    char *ptr_user = ptr_user_data->user;
    char *ptr_home_path = ptr_user_data->home_path;

    uid_t euid = geteuid();               // Получаем UID

    if(euid == 0) {
        const char *sudo_user = getenv("SUDO_USER");

        if(sudo_user != NULL) {
            strcpy(ptr_user, sudo_user);
        }else{
            printf("%s not determined\n", sudo_user);
            return 1;
        }

    } else {
        const char *user = getenv("USER");
        if(user != NULL) {
            strcpy(ptr_user, user);
        } else {
            printf("%s not determined\n", user);
            return 1;
        }
    }
    

    // получаем домашнюю директорию
    struct passwd *pw;

    if((pw = getpwnam(ptr_user)) != NULL) {
        strcpy(ptr_user_data->right_path, pw->pw_dir);
        strcpy(ptr_home_path, pw->pw_dir);
        // strcpy(ptr_user_data->right_path, "/home/dm/XXXXX");     //////////////////////////////////// удалить, тестовая папка

        snprintf(ptr_user_data->trash_directory, strlen(ptr_home_path) + 11, "%s/.my_trash", ptr_home_path);
        check_and_create_trash(ptr_user_data);
    }


    // получаем текущюю директорию
    if (getcwd(ptr_user_data->left_path, 1024) == NULL) {
        perror("Error getting current directory");
        return 1;
    }

    // strcpy(ptr_user_data->left_path, "/home/dm/YYYYY"); //////////////////////////////////// удалить, тестовая папка

    return 0;

}


void check_and_create_trash(user_data *ptr_user_data)
{
    struct stat st = {0};
    if(stat(ptr_user_data->trash_directory, &st) == -1) {   // получение инфо из файла/директории ptr_user_data->trash_directory; &st - куда сохранится результат
        mkdir(ptr_user_data->trash_directory, 0700);
    }
}
