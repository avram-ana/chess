#include <ncurses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "chess.h"

int main(void)
{
    enable_utf8_locale();

    piece_t m[8][8];
    char buf[6]; // "a1 a2" + '\0'

    initscr();
    cbreak();               // get chars immediately
    noecho();               // we will handle echoing in read_move
    keypad(stdscr, TRUE);   // enable special keys on stdscr

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    const int W = 8 * CELL_W, H = 8 * CELL_H;
    int oy = (rows - H) / 2; if (oy < 2) oy = 2;
    int ox = (cols - W) / 2; if (ox < 4) ox = 4;

    clear();
    draw_board(oy, ox);

    create_initial_pieces(m);
    draw_pieces(oy, ox, m);

    /* Clear the line where we will print the prompt (avoid passing NULL) */
    move(oy + H + 2, ox);
    clrtoeol();
    refresh();

    /* show cursor while typing */
    curs_set(1);

    /* Call read_move — pass the actual byte size of buf */
    int idk = read_move(stdscr, buf, sizeof(buf));

    /* hide cursor again if you prefer */
    curs_set(0);

    /* show what was typed (or that input was cancelled) */
    if (idk >= 0) {
        mvprintw(oy + H + 3, ox, "You entered: '%s' (len=%d)   ", buf, idk);
    } else {
        mvprintw(oy + H + 3, ox, "Input cancelled                ");
    }
    refresh();

    getch();

    endwin();
    return 0;
}
