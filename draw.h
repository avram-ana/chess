#ifndef _DRAW_H_
#define _DRAW_H_



// dimensions
static const int CELL_W = 4;
static const int CELL_H = 2;



void draw_board(int oy, int ox);
void put_piece(int oy, int ox, int r, int c, const char *glyph, int side_pair);
int get_square_color(int r, int c);
void init_colors(void);

void enable_utf8_locale(void);

#endif
