#ifndef _DRAW_H_
#define _DRAW_H_

// chess board dimensions
static const int CELL_W = 4;
static const int CELL_H = 2;


typedef enum color
{
    RED = 3,
    BLACK = 4,
    NONE = 5  // there is no piece there
}color_t;

typedef enum type
{
    empty,  // position is empty - 0
    pawn,  // pion - 1
    rook,  // tura - 2
    bishop,  //nebun - 3
    knight,  // cal - 4
    queen,  // 5
    king  // 6
}type_t;

typedef struct piece
{
  type_t type;
  color_t color;
  int x;
  int y;
}piece_t;


void enable_utf8_locale(void);
void init_colors(void);

void draw_board(int oy, int ox);
void draw_pieces(int oy, int ox, piece_t **m);
int get_square_color(int r, int c);
void put_piece(int oy, int ox, int r, int c, const char *glyph, int side_pair);

void type_text_on_window(int oy, int ox, const char *text, int seconds);
void show_what_is_being_typed(WINDOW *w, int oy, int ox, char c);
char *read_from_window(WINDOW *w, int oy, int ox);

#endif
