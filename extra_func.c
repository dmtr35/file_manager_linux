#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <libgen.h>

#include <ncurses.h>
#include <locale.h>
#include <signal.h>

#include "func.h"






void remove_first_char(char *str) {
    if (str != NULL && *str != '\0') {
        memmove(str, str + 1, strlen(str));
    }
}


