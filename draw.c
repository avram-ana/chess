#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include "draw.h"

void enable_utf8_locale(void)
{
  setlocale(LC_ALL, "");
}



int get_square_color(int row, int column)
{
  // Colors/pairs:
  // 1,2  -> board squares (light/dark)
  // 10   -> PURE_BLACK fg (top side)
  // 11   -> ORANGE fg (bottom side)
  // 21/22-> PURE_BLACK on light/dark square bg
  // 23/24-> ORANGE     on light/dark square bg
  
  // light square: the sum is even
  // dark square: the sum is odd
  if(!((row + column) % 2))  // if (row + column) % 2 == 0
    {
      return 2;
    }

  return 1;
}


void init_colors(void)
{
  if(!has_colors())
    {
      return;
    }
  
  start_color();
  use_default_colors();
  
  // Board squares
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_WHITE);  // dark (your style)
  
  // Piece foregounds
  int BLACK = COLOR_BLACK;
  int ORANGE = COLOR_RED;
  
  
  init_pair(10, BLACK, -1);
  init_pair(11, ORANGE,     -1);
  
  // Read board BGs and build composite pairs
  short tmp_fg, bg_light, bg_dark;
  pair_content(1, &tmp_fg, &bg_light);
  pair_content(2, &tmp_fg, &bg_dark);
  
  init_pair(21, BLACK, bg_light);
  init_pair(22, BLACK, bg_dark);
  init_pair(23, ORANGE,     bg_light);
  init_pair(24, ORANGE,     bg_dark);
}


void put_piece(int oy, int ox, int r, int c, const char *glyph, int side_pair /*10 or 11*/) {
    int square = get_square_color(r, c);        // 1 or 2
    int use_pair = (side_pair == 10)
                 ? (square == 1 ? 21 : 22)      // black side
                 : (square == 1 ? 23 : 24);     // orange side

    int y = oy + r * CELL_H + CELL_H / 2;
    int x = ox + c * CELL_W + CELL_W / 2;

    attron(COLOR_PAIR(use_pair) | A_BOLD);
    mvaddstr(y, x, glyph);
    attroff(COLOR_PAIR(use_pair) | A_BOLD);
}



void draw_board(int oy, int ox)
{
  init_colors(); // sets up pairs 1,2 and composite piece pairs
  
  // Fill squares
  bool hc = has_colors();
  
  for(int r = 0; r < 8; ++r)
    {
      for(int c = 0; c < 8; ++c)
	{
	  int pair = get_square_color(r, c);
	  if(hc)
	    {
	      attron(COLOR_PAIR(pair));
	    }
	  for(int dy = 0; dy < CELL_H; ++dy)
	    {
	      for(int dx = 0; dx < CELL_W; ++dx)
		{
		  mvaddch(oy + r * CELL_H + dy, ox + c * CELL_W + dx, ' ');
		}
	    }
	  if(hc)
	    {
	      attroff(COLOR_PAIR(pair));
	    }
        }
    }
  
  // Labels
  for(int c = 0; c < 8; ++c)
    {
      char file = 'a' + c;
      int cx = ox + c * CELL_W + CELL_W / 2;
      mvaddch(oy - 1, cx, file);
    }
  
  for(int r = 0; r < 8; ++r)
    {
      char rank = '8' - r;
      int ry = oy + r * CELL_H + CELL_H / 2;
      mvaddch(ry, ox - 2, rank);
    }
  
  // Pieces on top
  //draw_starting_pieces(oy, ox);
}
