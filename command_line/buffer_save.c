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

#include "../func.h"

char screen_buffer[10000] = {0};
int buffer_pos = 0;
int coord_x;
int coord_y;

void add_char_to_command_line(WINDOW *win_left, char c, int x, int y) {
    // printf("x: %d, y: %d", coords->cursor_y, coords->cursor_x);
    waddch(win_left, c);
    coord_x = x;
    coord_y = y;
    // coord_y++;
    // mvwaddch(win_left, coord_x, coord_y, c);
    wrefresh(win_left);
    char char_buffer[2] = {c, '\0'};
    save_to_buffer(char_buffer);
}

void remove_char_from_command_line(WINDOW *win_left, size_t cursor_coords) {
    if (buffer_pos > 0) {
        buffer_pos--;
        screen_buffer[buffer_pos] = '\0';
    }
    
    int y, x;
    getyx(win_left, y, x);
    if (x > cursor_coords) {
        mvwaddch(win_left, y, x - 1, ' ');                                           // Заменяем предыдущий символ пробелом
        wmove(win_left, y, x - 1);                                                   // Перемещаем курсор на предыдущий символ
        wrefresh(win_left);
    }

}



void save_to_buffer(const char *text) {
    int text_len = strlen(text);
    if (buffer_pos + text_len < 10000) {
        strcpy(&screen_buffer[buffer_pos], text);
        buffer_pos += text_len;
    }
}

void restore_from_buffer(WINDOW *win) {
    wclear(win);
    wprintw(win, "%s", screen_buffer);
    wrefresh(win);
}