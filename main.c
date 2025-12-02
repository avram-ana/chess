#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
#include <stdbool.h>
#include "chess_logic.h"
#include "draw.h"

#define MAX 128

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
				/*
				 empty,  // position is empty - 0
				pawn,  // pion - 1
				rook,  // tura - 2
				bishop,  //nebun - 3
				knight,  // cal - 4
				queen,  // 5
				king  // 6
				 */
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
	
	
    
    type_text_on_window(oy, ox, "Hello to the Chess Game :3 !", 2000);
    type_text_on_window(oy + 1, ox, "Choose the Version you want to play:", 2000);
    type_text_on_window(oy + 2, ox, "Type '1' for Classic  |  Type '2' for Fisher Random (Chess 960)", 2000);
	type_text_on_window(oy + 4, ox, "Enter version: ", 1000);
    
    // strcpy(buffer, read_from_window(stdscr, oy + 4, ox + strlen("Enter version: ")));
    char x = getch();
    char s[2];
    s[0] = x;
    s[1] = '\0';
    type_text_on_window(oy + 4, ox + strlen("Enter version: "), s, 1);
    
    switch(atoi(s))
    {
		case 1:
		{
			//mvprintw(oy + 8, ox, "%s", "CASE 1");
			//mvprintw(oy + 6, ox, "%s", "Type the PLAYER 1 username: ");
			//Play_Classical_Chess(W, H, oy, ox);
			//mvprintw(oy + 8, ox, "%s", "CASE 1 AFTER");
			Game_T* Game=Initialize_Classic_Game();
			type_text_on_window(oy + 6, ox, "Type the PLAYER 1 username: ", 1000);
			strcpy(Game->client1.username, read_from_window(stdscr, oy + 6, ox + strlen("Type the PLAYER 1 username: ")));

			type_text_on_window(oy + 7, ox, "Type the PLAYER 2 username: ", 1000); //scanf("%s",Game->client2.username);
			strcpy(Game->client2.username, read_from_window(stdscr, oy + 7, ox + strlen("Type the PLAYER 2 username: ")));
			
			clear();
			// ("%s is white and %s is black\n",Game->client1.username,Game->client2.username
			type_text_on_window(oy - 7, ox, Game->client1.username, 500);
			type_text_on_window(oy - 7, ox + strlen(Game->client1.username), " is white and ", 500);
			type_text_on_window(oy - 7, ox + strlen(Game->client1.username) + strlen(" is white and "), Game->client2.username, 500);
			type_text_on_window(oy - 7, ox + strlen(Game->client1.username) + strlen(" is white and ") + strlen(Game->client2.username), " is black.", 500);
			
			Game->client1.isWhite=true;
			Game->client2.isWhite=false;
			
			piece_t **matrix = convert_raul_to_ana(Game->Matrix);
			draw_board(oy, ox);
			draw_pieces(oy, ox, matrix);
			
			/*
			char move[STRING_SIZE];
			print_Game(Game);
			while(true)
			  {
			    do
			      {
				printf("%s type a move:",Game->client1.username); scanf("%s",move);
				if(strcmp(move,"draw")==0)
				  {
				    printf("%s do you also want to draw? Type Y/N:",Game->client2.username);scanf("%s",move);
				    if(strcmp(move,"Y")==0 || strcmp(move,"y")==0)
				      {
					Game->game_is_running=false;
					Game->game_ended_in_draw=true;
					break;
				      }
				    else
				      {
					printf("\nGame will resume\n");
					printf("%s type a move:",Game->client1.username);scanf("%s",move);
				      }
				  }
			      }while(Game_Move(Game,Game->client1,move)==false);
			    
			    if(Game->game_is_running==false)
			      break;
			    
			    print_Game(Game);
			    if(Game->is_black_king_checked==true) 
			      printf("\n   BLACK IS IN CHECK   \n");
			    
			    do
			      {
				printf("%s type a move:",Game->client2.username);scanf("%s",move);
				if(strcmp(move,"draw")==0)
				  {
				    printf("%s do you also want to draw? Type Y/N:",Game->client1.username);scanf("%s",move);
				    if(strcmp(move,"Y")==0 || strcmp(move,"y")==0)
				      {
					Game->game_is_running=false;
					Game->game_ended_in_draw=true;
					break;
				      }
				    else
				      {
					printf("\nGame will resume\n");
					printf("%s type a move:",Game->client1.username);scanf("%s",move);
				      }
				  }
			      }while(Game_Move(Game,Game->client2,move)==false);
			    
			    if(Game->game_is_running==false)
			      break;
			    
			    print_Game(Game);
			    if(Game->is_white_king_checked==true) 
			      printf("\n   WHITE IS IN CHECK  \n");
			    
			    Game->number_of_moves_played++;
			    if(Game->number_of_moves_played==40)
			      {
				Game->game_is_running=false;
				Game->game_ended_in_draw=true;
				Game->game_ended_in_white_won=false;
				break;
          }
			  } 
			Print_Game_Result(Game);
			*/
			free(Game->Matrix);
			free(Game);
			break;
		}
		default:
		  {
		    //mvprintw(oy + 8, ox, "%s", "CASE DEFAULT");
		    //Play_Classical_Chess(W, H, oy, ox);
		    break;
		  }
    }
    
    /*
	if(s[0] == 1)
	{
	refresh();
	Play_Classical_Chess(W, H, oy, ox);
	}
	else if(s[0] == 2)
	{
	Play_Classical_Chess(W, H, oy, ox);
	}
	else
	{
	mvprintw(oy + 6, ox, "%s", "Invalid version entered. Version that has been entered: ");
		mvprintw(oy + 6, ox + strlen("Invalid version entered. Version that has been entered: "), "%d", s[0]);
		refresh();
		}*/
    
    getch();
    endwin();
    
    //printf("%c\n", x);
    
    return 0;
}
