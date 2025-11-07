#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include "chess.h"


void enable_utf8_locale(void)
{
  setlocale(LC_ALL, "");
}

int is_empty(piece_t m[][8], int i, int j)
{
	return m[i][j].type == 0;  // empty = 0
}

void draw_pieces(int oy, int ox, piece_t m[][8])
{
  // Use FILLED (black-style) glyphs for BOTH sides
  const char *p[] = { " ", "♟", "♜", "♝", "♞", "♛", "♚" };

	// in function init_color: 0 = black, 1 = orange

  // Top side:black
  for(int i = 0; i < 8; i++)
  {
	  for(int j = 0; j < 8; j++)
	  {
		  /*
		empty,  // position is empty - 0
		pawn,  // pion - 1
		rook,  // tura - 2
		bishop,  //nebun - 3
		knight,  // cal - 4
		queen,  // 5
		king  // 6
		*/

		if(!m[i][j].type)
		{
			continue;  // empty
		}
		
        put_piece(oy, ox, i, j, p[m[i][j].type], m[i][j].color);
		 
	  }
  }
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
  
  // board squares
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_WHITE, COLOR_WHITE);  // dark (your style)
  
  // piece foregounds
  int BLACK = COLOR_BLUE;
  int ORANGE = COLOR_RED;
  
  
  init_pair(0, BLACK, -1);
  init_pair(1, ORANGE,     -1);
  init_pair(-1, NONE, -1);
  
  // read board BGs and build composite pairs
  short tmp_fg, bg_light, bg_dark;
  pair_content(1, &tmp_fg, &bg_light);
  pair_content(2, &tmp_fg, &bg_dark);
  
  init_pair(21, BLACK, bg_light);
  init_pair(22, BLACK, bg_dark);
  init_pair(23, ORANGE,     bg_light);
  init_pair(24, ORANGE,     bg_dark);
}


void determine_and_create_piece(piece_t m[][8], int i, int j)
{
	switch(j)
	{
		case 0:
		case 7:
		{
			// tura
			m[i][j].type = rook;
			m[i][j].x = i;
			m[i][j].y = j;
			break;
		}
		case 1:
		case 6:
		{
			// cal
			m[i][j].type = knight;
			m[i][j].x = i;
			m[i][j].y = j;
			break;
		}
		case 2:
		case 5:
		{
			// nebun
			m[i][j].type = bishop;
			m[i][j].x = i;
			m[i][j].y = j;
			break;
		}
		case 3:
		{
			m[i][j].type = queen;
			m[i][j].x = i;
			m[i][j].y = j;
			break;
		}
		default:
		{
			m[i][j].type = king;
			m[i][j].x = i;
			m[i][j].y = j;
			break;
		}
	}
}



void create_initial_pieces(piece_t m[][8])
{
	for(int row = 0; row < 8; row++)
	{
		for(int col = 0; col < 8; col++)
		{
			if(row == 0)
			{
				// create black rooks, bishops etc.
				determine_and_create_piece(m, row, col);
				m[row][col].color = BLACK;
			}
			else if(row == 1)
			{
				// create black pawns
				m[row][col].type = pawn;
				m[row][col].color = BLACK;
				m[row][col].x = row;
				m[row][col].y = col;
			}
			else if(row == 6)
			{
				// create orange pawns
				m[row][col].type = pawn;
				m[row][col].color = ORANGE;
				m[row][col].x = row;
				m[row][col].y = col;
			}
			else if(row == 7)
			{
				// create orange rooks, bishops etc.
				determine_and_create_piece(m, row, col);
				m[row][col].color = ORANGE;
			}
			else
			{
				// empty position
				m[row][col].type = empty;
				m[row][col].x = row;
				m[row][col].y = col;
				m[row][col].color = NONE;
			}
		}
	}
	/*
	piece_t aux = m[0][1];
	m[0][1] = m[4][1];
	m[4][1] = aux;
	*/
}


void put_piece(int oy, int ox, int r, int c, const char *glyph, int color) {
    int square = get_square_color(r, c);        // for the background
    int use_pair = (color == 0)
                 ? (square == 1 ? 21 : 22)
                 : (square == 1 ? 23 : 24);

    int y = oy + r * CELL_H + CELL_H / 2;
    int x = ox + c * CELL_W + CELL_W / 2;

    attron(COLOR_PAIR(use_pair) | A_BOLD);
    mvaddstr(y, x, glyph);
    attroff(COLOR_PAIR(use_pair) | A_BOLD);
}



void draw_board(int oy, int ox)
{
  init_colors();
  
  // fill squares
  bool hc = has_colors();
  
  for(int r = 0; r < 8; r++)
    {
      for(int c = 0; c < 8; c++)
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
  
  // labels
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
  
  //draw_starting_pieces(oy, ox);
}


int read_move(WINDOW *w, char *buf, int length)
{
    const char *text = "ENTER MOVE: ";
    int text_length = (int)strlen(text);

    if (length <= 1) return -1; // need space for '\0'

    // Print prompt
    mvwprintw(w, 0, 0, "%s", text);
    wclrtoeol(w);
    wrefresh(w);

    // Ensure keypad so KEY_BACKSPACE is available
    keypad(w, TRUE);

    int count = 0;
    int c;

    // Make sure buffer is empty to start
    buf[0] = '\0';

    while (1) {
        // update cursor pos each iteration
        int cy, cx;
        getyx(w, cy, cx);

        c = wgetch(w);

        // ENTER -> finish
        if (c == '\n' || c == '\r') {
            break;
        }

        // ESC -> cancel
        if (c == 27) {
            buf[0] = '\0';
            // clear possible error line
            mvwprintw(w, 1, 0, "                      ");
            wrefresh(w);
            return -1;
        }

        // BACKSPACE keys
        if (c == KEY_BACKSPACE || c == 8 || c == 127) {
            if (count > 0) {
                // get latest cursor pos
                getyx(w, cy, cx);
                if (cx > text_length) { // ensure we don't delete prompt
                    // move back one, overwrite, move cursor
                    mvwaddch(w, cy, cx - 1, ' ');
                    wmove(w, cy, cx - 1);
                    wrefresh(w);

                    count--;
                    buf[count] = '\0';
                } else {
                    mvwprintw(w, 2, 0, "Nothing to delete   ");
                    wrefresh(w);
                    beep();
                }
            } else {
                // nothing typed
                mvwprintw(w, 2, 0, "Nothing to delete   ");
                wrefresh(w);
                beep();
            }
            continue;
        }

        // Printable characters
        if (isprint(c)) {
            if (count < length - 1) {
                // append char
                buf[count++] = (char)c;
                buf[count] = '\0';

                // show it at correct column: text_length + (count-1)
                mvwaddch(w, 0, text_length + (count - 1), (chtype)c);
                // move cursor to after written char
                wmove(w, 0, text_length + count);
                wrefresh(w);
            } else {
                mvwprintw(w, 2, 0, "Input too long (max %d)", length - 1);
                wrefresh(w);
                beep();
            }
            continue;
        }

        // Other non-printable keys: ignore or notify
        mvwprintw(w, 2, 0, "Invalid key           ");
        wrefresh(w);
        beep();
    }

    // finalize
    buf[count] = '\0';
    mvwprintw(w, 1, 0, "                       "); // clear error row
    mvwprintw(w, 2, 0, "                       ");
    wrefresh(w);
    return count;
}
