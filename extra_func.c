#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <stdint.h>



#include "func.h"


int is_directory(const char *path)
{
    DIR *dir = opendir(path);
    if(dir != NULL) {
        closedir(dir);
        return 1;           // Это директория
    } else {
        return 0;
    }
}


void remove_first_char(char *str) {
    if (str != NULL && *str != '\0') {
        memmove(str, str + 1, strlen(str));
    }
}


char *human_readable_size(uintmax_t size, char *buf) {
    const char *suffixes[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    int i;

    for (i = 0; size >= 1024 && i < (sizeof(suffixes) / sizeof(suffixes[0]) - 1); ++i) {
        size /= 1024;
    }

    if (strcmp(suffixes[i], "B") == 0) {
        sprintf(buf, "%3ju %s ", size, suffixes[i]);
    } else {
        sprintf(buf, "%3ju %s", size, suffixes[i]);
    }
    return buf;
}


char* format_last_modification_time(time_t time, char *buf) {
    struct tm *timeinfo = localtime(&time);
    strftime(buf, 20, "%y.%m.%d %H:%M", timeinfo);

    return buf;
}