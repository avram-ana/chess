#ifndef _DRAW_H_
#define _DRAW_H_


// chess board dimensions
static const int CELL_W = 4;
static const int CELL_H = 2;


typedef enum color
{
	ORANGE = 1,
	BLACK = 0,
	NONE = -1  // there is no piece there
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


void draw_board(int oy, int ox);
void draw_pieces(int oy, int ox, piece_t m[][8]);

void put_piece(int oy, int ox, int r, int c, const char *glyph, int side_pair);
int get_square_color(int r, int c);
void init_colors(void);

void create_initial_pieces(piece_t m[][8]);

int read_move(WINDOW *w, char *buf, int length);

void enable_utf8_locale(void);

#endif
