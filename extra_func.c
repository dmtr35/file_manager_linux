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

char* get_current_datatime(void)
{
    time_t rawtime;
    struct tm *timeinfo;
    char *buffer = (char *)malloc(20 * sizeof(char));

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 20, "%d-%m-%Y_%H-%M-%S", timeinfo);

    return buffer;
}

char *replace_slashes_dash(char *path)
{
    size_t length_dir_file = strlen(path) + 1;
    char slash_or_dash[2];
    char first_symbol[2];
    char second_symbol[2];
    strncpy(slash_or_dash, path, 1);


    if(strstr(slash_or_dash, "/")){
        first_symbol[0] = '/';
        second_symbol[0] = '-';
    } else {
        first_symbol[0] = '-';
        second_symbol[0] = '/';
    }

    char *result = (char *)malloc(length_dir_file);

    for (int i = 0, j = 0; i < length_dir_file; ++i) {
        if (path[i] == first_symbol[0]) {
            result[j++] = second_symbol[0];
        } else {
            result[j++] = path[i];
        }
    }
    result[length_dir_file] = '\0';
    return result;
}


void extractFileNameAndPath(char *fullFileName, char *path)
{
    const char *pattern = "____";
    char *position = strstr(fullFileName, pattern);

    size_t length = strlen(position);
    char *start = position + 24;
    char *end = position + length - 11;

    size_t len_file_path = end - start;
    strncpy(path, start, len_file_path);
    path[len_file_path] = '\0';
}


void remove_first_char(char *str)
{
    if (str != NULL && *str != '\0') {
        memmove(str, str + 1, strlen(str));
    }
}


char *human_readable_size(uintmax_t size, char *buf)
{
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


char* format_last_modification_time(time_t time, char *buf)
{
    struct tm *timeinfo = localtime(&time);
    strftime(buf, 20, "%y.%m.%d %H:%M", timeinfo);

    return buf;
}


void removeDuplicates(int *arr, size_t size)
{
    if (size <= 1) {
        return;  // Если в массиве нет элементов или только один элемент, дубликатов нет
    }

    // Сортируем массив
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    // Удаляем дубликаты
    int j = 0;  // Индекс для нового массива без дубликатов
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] != arr[i + 1]) {
            arr[j++] = arr[i];  // Сохраняем только уникальные элементы
        }
    }
    arr[j++] = arr[size - 1];  // Сохраняем последний элемент

    // Сдвигаем все элементы на одну позицию влево, чтобы заменить нулевой элемент
    for (int i = 0; i < j - 1; i++) {
        arr[i] = arr[i + 1];
    }

    // Заполняем все оставшиеся элементы массива нулями
    for (int i = j - 1; i < size; i++) {
        arr[i] = 0;
    }
}

void addToArr(int *arr, size_t size, int value)
{
    for (int i = 0; i < size; i++) {
        if (arr[i] == 0) {
            arr[i] = value;
            break;
        }
    }
}

// Функция для удаления элемента из массива
void removeFromArr(int *arr, size_t size, int value)
{
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            arr[i] = 0;
            break;
        }
    }
}

bool containsElement(int *arr, size_t size, int value)
{
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return true;  // Элемент найден, возвращаем true
        }
    }
    return false;  // Элемент не найден, возвращаем false
}

void fillWithZeros(int *arr, struct coordinates *coords, size_t size)
{
    memset(arr, 0, size * sizeof(int));
    coords->leng_arr_coorsor = 0;
}

// проверка массива, есть ли в нем элементы
int check_int_arr(int *arr, size_t size)
{
int is_empty = 1;                                    // Предполагаем, что массив пустой (заполнен нулями)
    for (int i = 0; i < size; i++) {
        if (arr[i] != 0) {
            is_empty = 0;                           // Массив не пустой, так как найден ненулевой элемент
            return is_empty;
        }
    }
return is_empty;
}


int count_non_zero_elements(int *arr, size_t size)
{
    int non_zero_count = 0;

    for (int i = 0; i < size; i++) {
        if (arr[i] != 0) {
            non_zero_count++;
        }
    }

    return non_zero_count;
}
