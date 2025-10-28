#include <ncurses.h>
#include <locale.h>
#include "draw.h"


// Draw all pieces in starting positions
void draw_starting_pieces(int oy, int ox)
{
  // Use FILLED (black-style) glyphs for BOTH sides
  const char *solid_row[] = {"♜","♞","♝","♛","♚","♝","♞","♜"};
  const char *solid_pawn  = "♟";
  
  // Top side = pure black (filled)
  for (int c = 0; c < 8; ++c) put_piece(oy, ox, 0, c, solid_row[c], 10); // rank 8
  for (int c = 0; c < 8; ++c) put_piece(oy, ox, 1, c, solid_pawn,   10); // rank 7
  
  // Bottom side = filled orange
  for (int c = 0; c < 8; ++c) put_piece(oy, ox, 6, c, solid_pawn,   11); // rank 2
  for (int c = 0; c < 8; ++c) put_piece(oy, ox, 7, c, solid_row[c], 11); // rank 1
}


int main(void)
{
  enable_utf8_locale();
  
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  
  int rows, cols; getmaxyx(stdscr, rows, cols);
  const int W = 8 * CELL_W, H = 8 * CELL_H;
  int oy = (rows - H) / 2; if (oy < 2) oy = 2;
  int ox = (cols - W) / 2; if (ox < 4) ox = 4;
  
  clear();
  draw_board(oy, ox);
  draw_starting_pieces(oy, ox);

  // draw_board(oy, ox);
  
  mvprintw(oy + H + 2, ox, NULL);
  refresh();
  getch();
  
  endwin();
  return 0;
}
