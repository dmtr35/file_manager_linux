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


char *human_readable_size(uintmax_t size, char *buf) {

    const char *suffixes[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    int i;

    for (i = 0; size >= 1024 && i < (sizeof(suffixes) / sizeof(suffixes[0]) - 1); ++i) {
        size /= 1024;
    }

    sprintf(buf, "%ju %s", size, suffixes[i]);
    return buf;
}

char* format_last_modification_time(time_t time, char *buf) {
    struct tm *timeinfo = localtime(&time);

    // Format the time as "YY.MM.DD HH:MM"
    strftime(buf, 20, "%y.%m.%d %H:%M", timeinfo);

    // printf("buf: %s\n", buf);
    return buf;
}