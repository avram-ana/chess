#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include <locale.h>
#include <stdbool.h>
#include "chess_logic.h"
#include "draw.h"

#define MAX 128

void from_raul_to_ana(piece_t **ana, char *move)
{
    int from_col = convert_character_to_column(move[0]);
    int to_col   = convert_character_to_column(move[2]);

    int from_row = 8 - (move[1] - '0');
    int to_row   = 8 - (move[3] - '0');

    piece_t moving = ana[from_row][from_col];

    // destination occupied? → capture
    if (ana[to_row][to_col].type != empty)
    {
        // overwrite the captured piece
        ana[to_row][to_col] = moving;
    }
    else
    {
        // normal move
        ana[to_row][to_col] = moving;
    }

    // clear origin square
    ana[from_row][from_col].type = empty;
    ana[from_row][from_col].color = NONE;
}

/*
piece_t **convert_raul_to_ana(Element_T m[])
{
  piece_t **new = (piece_t **)malloc(sizeof(piece_t *) * ROW);
  if(!new)
    {
      perror("Error allocating matrix in convert_raul_to_ana()");
      exit(EXIT_FAILURE);
	}
  
  for(int i = 0; i < ROW; i++)
    {
      new[i] = (piece_t *)malloc(sizeof(piece_t) * COLS);
      if(!new[i])
	{
	  perror("Error allocating m[i] in convert_raul_to_ana()");
	  exit(EXIT_FAILURE);
	}
    }	
  
  // const char *p[] = {" ", "♟", "♜", "♝", "♞", "♛", "♚"};
  for(int i = 0; i < ROW; i++)
    {
      for(int j = 0; j < COLS; j++)
	{
	  if((m[i * ROW + j].isWhite))
	    {
			new[i][j].color = RED;
	    }
	  else
	    {
	      new[i][j].color = BLACK;
	    }
	  
	  new[i][j].x = i;
	  new[i][j].y = j;
	  
	  switch(m[i * ROW + j].name)
	    {
		//empty,  // position is empty - 0
		//pawn,  // pion - 1
		//rook,  // tura - 2
		//bishop,  //nebun - 3
		//knight,  // cal - 4
		//queen,  // 5
		//king  // 6
	      
	    case PAWN:  // pion
	      {
		new[i][j].type = pawn;
		break;
	      }
	    case ROOK:  // tura
	      {
		new[i][j].type = rook;
		break;
	      }
	    case BISHOP:  // nebun
	      {
		new[i][j].type = bishop;
		break;
	      }
	    case KNIGHT:  // cal
	      {
		new[i][j].type = knight;
		break;
	      }
	    case QUEEN:
	      {
		new[i][j].type = queen;
		break;
	      }
	    case KING:
	      {
		new[i][j].type = king;
		break;
	      }
	    case Empty:
	      {
		new[i][j].color = NONE;
		break;
	      }
	    }
	}
    }
  
  return new;
}*/

piece_t **convert_raul_to_ana(Element_T m[])
{
  piece_t **new = (piece_t **)malloc(sizeof(piece_t *) * ROW);
  if(!new)
    {
      perror("Error allocating matrix in convert_raul_to_ana()");
      exit(EXIT_FAILURE);
    }
  
  for(int i = 0; i < ROW; i++)
    {
      new[i] = (piece_t *)malloc(sizeof(piece_t) * COLS);
      if(!new[i])
      {
         perror("Error allocating m[i] in convert_raul_to_ana()");
         exit(EXIT_FAILURE);
      }
    }   
  
  for(int i = 0; i < ROW; i++)
    {
      for(int j = 0; j < COLS; j++)
      {
        // --- THE FIX IS HERE ---
        // We use a DIRECT mapping. 
        // i = 0 (Screen Top) reads Logic Index 0 (Black Pieces).
        // i = 7 (Screen Bottom) reads Logic Index 7 (White Pieces).
        int index = i * ROW + j;

        if((m[index].isWhite))
        {
           new[i][j].color = BLACK; // Remember: BLACK constant = Blue (White pieces)
        }
        else
        {
           new[i][j].color = RED;   // Remember: RED constant = Red (Black pieces)
        }
        
        new[i][j].x = i;
        new[i][j].y = j;
        
        switch(m[index].name)
        {
          case PAWN:   new[i][j].type = pawn;   break;
          case ROOK:   new[i][j].type = rook;   break;
          case BISHOP: new[i][j].type = bishop; break;
          case KNIGHT: new[i][j].type = knight; break;
          case QUEEN:  new[i][j].type = queen;  break;
          case KING:   new[i][j].type = king;   break;
          case Empty:  new[i][j].color = NONE;  break;
        }
      }
    }
  
  return new;
}


int main(void)
{
  enable_utf8_locale();
  
  initscr();
  cbreak();               // get chars immediately
  noecho();               // we will handle echoing in read_move
  keypad(stdscr, TRUE);   // enable special keys on stdscr
  
  int rows, cols;
  getmaxyx(stdscr, rows, cols);
  const int W = 8 * CELL_W, H = 8 * CELL_H;
  int oy = (rows - H) / 2; 
  if(oy < 2)
    {
      oy = 2;
    }
  
  int ox = (cols - W) / 2;
  if(ox < 4) 
    {
      ox = 4;
    }
  
  
  
  type_text_on_window(oy, ox, "Welcome to the Chess Game! :3", 2000);
  /*
    type_text_on_window(oy + 1, ox, "Choose the Version you want to play:", 2000);
    type_text_on_window(oy + 2, ox, "Type '1' for Classic  |  Type '2' for Fisher Random (Chess 960)", 2000);
    type_text_on_window(oy + 4, ox, "Enter version: ", 1000);
    
    // strcpy(buffer, read_from_window(stdscr, oy + 4, ox + strlen("Enter version: ")));
    char x = getch();
    char s[2];
    s[0] = x;
    s[1] = '\0';
    type_text_on_window(oy + 4, ox + strlen("Enter version: "), s, 1);
  */
  
  /*
    switch(atoi(s))
    {
    case 1:
    {
  */
  Game_T* Game=Initialize_Classic_Game();
  type_text_on_window(oy + 6, ox, "Type the PLAYER 1 username: ", 1000);
  char *aux = read_from_window(stdscr, oy + 6, ox + strlen("Type the PLAYER 1 username: "));
  strcpy(Game->client1.username, aux);
  free(aux);
  
  type_text_on_window(oy + 7, ox, "Type the PLAYER 2 username: ", 1000);
  aux = read_from_window(stdscr, oy + 7, ox + strlen("Type the PLAYER 2 username: "));
  strcpy(Game->client2.username, aux);
  free(aux);
  
  clear();
  
  type_text_on_window(oy - 7, ox, Game->client1.username, 500);
  type_text_on_window(oy - 7, ox + strlen(Game->client1.username), " is blue and ", 500);
  type_text_on_window(oy - 7, ox + strlen(Game->client1.username) + strlen(" is blue and "), Game->client2.username, 500);
  type_text_on_window(oy - 7, ox + strlen(Game->client1.username) + strlen(" is blue and ") + strlen(Game->client2.username), " is red.", 500);
  
  Game->client1.isWhite=true;
  Game->client2.isWhite=false;
  
  piece_t **matrix = convert_raul_to_ana(Game->Matrix);
  
  char move[STRING_SIZE];
  
  while(true)
    {
      draw_board(oy, ox);
      draw_pieces(oy, ox, matrix);
      do
	{
	  mvprintw(oy + 17, ox, "%s", "                                                ");
	  mvprintw(oy + 17, ox, "%s", Game->client1.username);
	  mvprintw(oy + 17, ox + strlen(Game->client1.username), "%s", "'s turn!");
	  mvprintw(oy + 18, ox, "%s", "type a move: ");
	  mvprintw(oy + 18, ox + strlen("type a move: "), "%s", "                                             ");
	  aux = read_from_window(stdscr, oy + 18, ox + strlen("type a move: "));
	  strcpy(move, aux);
	  free(aux);
	  
	  if(strcmp(move,"draw")==0)
	    {
	      mvprintw(oy + 19, ox, "%s", Game->client2.username);
	      mvprintw(oy + 19, ox + strlen(Game->client2.username), "%s", ", do you also want to draw?");
	      mvprintw(oy + 20, ox, "%s", "Type Y/N: ");
	      
	      aux = read_from_window(stdscr, oy + 20, ox + strlen("Type Y/N: "));
	      strcpy(move, aux);
	      free(aux);
	      if(strcmp(move,"Y")==0 || strcmp(move,"y")==0)
		{
		  Game->game_is_running=false;
		  Game->game_ended_in_draw=true;
		  break;	
		}
	      else if((!(strcmp(move, "N")) || (!(strcmp(move, "n")))))
		{
		  mvprintw(oy + 19, ox, "%s", "                                             ");  // to clean the line
		  mvprintw(oy + 20, ox, "%s", "                                             ");
		  type_text_on_window(oy + 19, ox, "The game will resume.", 1000);
		  mvprintw(oy + 19, ox, "%s", "                                             ");
		  mvprintw(oy + 18, ox + strlen("type a move: "), "%s", "                          ");
		}
	    }
	}while(Game_Move(Game,Game->client1,move)==false);
      
      if(Game->game_is_running==false)
	{
	  break;
	}
	
	if(isalpha(move[0]) && isdigit(move[1]) && isalpha(move[2]) && isdigit(move[3]))  // it's a piece move
	    {
	      from_raul_to_ana(matrix, move);  // swap elements in ana's matrix too
	    }

      if(Game->is_black_king_checked)
	{
	  type_text_on_window(oy + 21, ox, "Black is in check.", 1000);
	  napms(300);
	  mvprintw(oy + 21, ox, "%s", "                       ");
	}
      
      do
	{
	  draw_board(oy, ox);
	  draw_pieces(oy, ox, matrix);
	  
	  mvprintw(oy + 17, ox, "%s", "                                                ");
	  mvprintw(oy + 17, ox, "%s", Game->client2.username);
	  mvprintw(oy + 17, ox + strlen(Game->client2.username), "%s", "'s turn!");
	  mvprintw(oy + 18, ox, "%s", "type a move: ");
	  mvprintw(oy + 18, ox + strlen("type a move: "), "%s", "                                             ");
	  
	  aux = read_from_window(stdscr, oy + 18, ox + strlen("type a move: "));
	  strcpy(move, aux);
	  free(aux);
	  
	  if(strcmp(move, "draw") == 0)
	    {
	      mvprintw(oy + 19, ox, "%s", Game->client1.username);
	      mvprintw(oy + 19, ox + strlen(Game->client1.username), "%s", ", do you also want to draw?");
	      mvprintw(oy + 20, ox, "%s", "Type Y/N: ");
	      
	      aux = read_from_window(stdscr, oy + 20, ox + strlen("Type Y/N: "));
	      strcpy(move, aux);
	      free(aux);
	      
	      if(strcmp(move, "Y") == 0 || strcmp(move, "y") == 0)
		{
		  Game->game_is_running = false;
		  Game->game_ended_in_draw = true;
		  break;
		}
	      else
		{
		  mvprintw(oy + 19, ox, "%s", "                                             ");
		  mvprintw(oy + 20, ox, "%s", "                                             ");
		  type_text_on_window(oy + 19, ox, "The game will resume.", 1000);
		  mvprintw(oy + 19, ox, "%s", "                                             ");
		}
	    }
	}while(Game_Move(Game, Game->client2, move) == false);
      
      if(Game->game_is_running == false)
	{
	  break;
	}
	
	if(isalpha(move[0]) && isdigit(move[1]) && isalpha(move[2]) && isdigit(move[3]))
	    {
	      from_raul_to_ana(matrix, move);
	    }

      Game->number_of_moves_played++;

      if(Game->is_white_king_checked)
	{
	  type_text_on_window(oy + 21, ox, "White is in check.", 1000);
	  napms(300);
	  mvprintw(oy + 21, ox, "%s", "                  ");
	}

      if(Game->number_of_moves_played == 40)
	{
	  Game->game_is_running = false;
	  Game->game_ended_in_draw = true;
	  Game->game_ended_in_white_won = false;
	  break;
	}
    }

  Print_Game_Result(Game);
  
  for(int i = 0; i < ROW; i++)
    {
      free(matrix[i]);
    }
  free(matrix);
  matrix = NULL;
  
  free(Game->Matrix);
  free(Game);
                   //break;
			
		/*
		}
		default:
		  {
		    //mvprintw(oy + 8, ox, "%s", "CASE DEFAULT");
		    //Play_Classical_Chess(W, H, oy, ox);
		    break;
		  }
		  }*/
		  
  return 0;
}
