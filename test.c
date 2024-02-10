#include <ncurses.h>
#include <signal.h>

WINDOW *win;

void handle_resize(int sig) {
    // Ничего не делаем, обработчик сигнала просто оповещает о изменении размера окна
}

int main() {
    initscr(); // Инициализация ncurses
    // signal(SIGWINCH, handle_resize); // Устанавливаем обработчик сигнала изменения размера окна

    keypad(stdscr, TRUE); // Включаем обработку функциональных клавиш
    noecho(); // Отключаем вывод введенных символов


    int height, width;
    getmaxyx(stdscr, height, width);

    win = newwin(height, width, 0, 0); // Создаем новое окно
    box(win, 0, 0); // Рисуем рамку вокруг окна


    int row = 2; // Строка, в которой начнем выводить текст

    mvwprintw(win, row++, 1, "123456789012345678901234567890123456789012345678901234567890123456789012345678");
    mvwprintw(win, row++, 1, "Press 'q' to exit.");
    mvwprintw(win, row++, 1, "You can type commands here.");

    wrefresh(win); // Обновляем окно
    
    int ch;
    resize_term(0, 0);
    int prev_height, prev_width;
    getmaxyx(win, prev_height, prev_width);

    while ((ch = getch()) != 'q') {
        if (ch == KEY_RESIZE) {
            // Размер терминала изменился, обновляем размер окна
            int cur_height, cur_width;
            getmaxyx(stdscr, cur_height, cur_width);

            if (cur_height != prev_height || cur_width != prev_width) {
                prev_height = cur_height;
                prev_width = cur_width;

                wclear(win);
                wresize(win, cur_height, cur_width);
                mvwin(win, 0, 0);
                box(win, 0, 0);
                wrefresh(win); // Обновляем окно
            }
        }

        mvwprintw(win, row++, 1, "You pressed a key!"); // Выводим сообщение после каждого ввода
        box(win, 0, 0); // Рисуем рамку заново
        wrefresh(win); // Обновляем окно
    }

    endwin(); // Завершаем работу с ncurses

    return 0;
}




#include <ncurses.h>

int main() {
    // Инициализация ncurses
    initscr();
    keypad(stdscr, TRUE); // Включаем обработку функциональных клавиш
    noecho();             // Отключаем вывод введенных символов

    int row, col;
    getmaxyx(stdscr, row, col);

    // Выводим начальный размер окна
    mvprintw(0, 0, "Window size:1: Rows = %d, Cols = %d", row, col);
    refresh();

    int ch;
    while ((ch = getch()) != 'q') {
        // Измеряем размер экрана в рядах и колонках
        getmaxyx(stdscr, row, col);

        // Очищаем экран
        clear();

        // Выводим новый размер окна
        mvprintw(0, 0, "Window size:2: Rows = %d, Cols = %d", row, col);
        refresh();
    }

    endwin(); // Завершаем работу с ncurses

    return 0;
}