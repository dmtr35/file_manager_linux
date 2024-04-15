#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <libgen.h>
 #include <ctype.h>

#include <ncurses.h>
#include <locale.h>
#include <signal.h>

#include "../func.h"

void add_char_to_enter_name(WINDOW *win, int ch, char *screen_buffer, int *buffer_pos) {
    if (*buffer_pos < 63) {                                                                 // Убедитесь, что позиция буфера не превышает максимальную длину
        if (isprint(ch)) {                                                                  // Проверяем, что символ является печатаемым
            screen_buffer[*buffer_pos] = ch;                                                // Добавляем символ к буферу
            mvwprintw(win, 7, 14 + *buffer_pos, "%c", ch);                                  // Выводим символ на экран в соответствии с позицией в строке
            wrefresh(win);                                                                  // Обновляем окно
            (*buffer_pos)++;                                                                // Увеличиваем позицию буфера для следующего символа
        }
    }
}
void delete_char_from_enter_name(WINDOW *win, char *screen_buffer, int *buffer_pos) {
    if (*buffer_pos > 0) {                                                                  // Убеждаемся, что есть символы для удаления
        (*buffer_pos)--;                                                                    // Уменьшаем позицию буфера для удаления последнего символа
        screen_buffer[*buffer_pos] = '\0';                                                  // Устанавливаем последний символ в буфере как нулевой символ, чтобы удалить его
        mvwprintw(win, 7, 14 + *buffer_pos, " ");                                           // Печатаем пробел на месте удаленного символа на экране
        wrefresh(win);                                                                      // Обновляем окно
    }
}
// =================================================================================================


void add_char_to_command_line(WINDOW *win_left, char c, char *screen_buffer, int *buffer_pos) {
// void add_char_to_command_line(WINDOW *win_left, char c, int x, int y) {
    // printf("x: %d, y: %d", coords->cursor_y, coords->cursor_x);
    waddch(win_left, c);
    // coord_x = x;
    // coord_y = y;
    // coord_y++;
    // mvwaddch(win_left, coord_x, coord_y, c);
    wrefresh(win_left);
    char char_buffer[2] = {c, '\0'};
    save_to_buffer(char_buffer, screen_buffer, buffer_pos);
}

void remove_char_from_command_line(WINDOW *win_left, size_t cursor_coords, char *screen_buffer, int *buffer_pos) {
    if (buffer_pos > 0) {
        buffer_pos--;
        screen_buffer[*buffer_pos] = '\0';
    }
    
    int y, x;
    getyx(win_left, y, x);
    if (x > cursor_coords) {
        mvwaddch(win_left, y, x - 1, ' ');                                           // Заменяем предыдущий символ пробелом
        wmove(win_left, y, x - 1);                                                   // Перемещаем курсор на предыдущий символ
        wrefresh(win_left);
    }
}

void save_to_buffer(const char *text, char *screen_buffer, int *buffer_pos) {
    int text_len = strlen(text);
    if (*buffer_pos + text_len < 10000) {
        strcpy(&screen_buffer[*buffer_pos], text);
        *buffer_pos += text_len;
    }
}

void restore_from_buffer(WINDOW *win, char *screen_buffer) {
    wclear(win);
    wprintw(win, "%s", screen_buffer);
    wrefresh(win);
}

void restore_from_buffer_offset(WINDOW *win, char *screen_buffer, int offset)
{
    wclear(win);
    wprintw(win, "%s", screen_buffer + offset);
    wrefresh(win);
}