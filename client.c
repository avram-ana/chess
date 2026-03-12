#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <ncurses.h>
#include <locale.h>
#include "GUI.h"

#define PORT 8080
#define SIZE_OF_MESSAGE 128
#define ROW 8 
#define COLUMN 8 
#define STRING_SIZE 512

typedef enum chess_pieces {
  PAWN, KNIGHT, QUEEN, KING, ROOK, BISHOP, Empty,
} Piece_Name;

typedef struct {
    Piece_Name name;
    bool isWhite;
} Element_T;

void free_gui_matrix(piece_t **m) 
{
    if(!m) 
    {
		return;
	}
	
    for (int i = 0; i < ROW; i++) 
    {
        free(m[i]);
    }
    
    free(m);
}



piece_t **convert_raul_to_ana(Element_T m[], bool amIWhite)
{
  piece_t **new = (piece_t **)malloc(sizeof(piece_t *) * ROW);
  if(!new) { perror("Matrix Error"); exit(EXIT_FAILURE); }
  
  for(int i = 0; i < ROW; i++) {
      new[i] = (piece_t *)malloc(sizeof(piece_t) * COLUMN); 
      if(!new[i]) { perror("Row Error"); exit(EXIT_FAILURE); }
  }    
  
  for(int i = 0; i < ROW; i++)
  {
      for(int j = 0; j < COLUMN; j++)
      {
        int logic_row = i; // Default: Visual Row 'i' matches Logic Row 'i'

        // --- SWITCH CASE FOR BLACK PLAYER ---
        // Maps Visual Screen Rows (i) to Logic Matrix Rows
        if (!amIWhite) 
        {
            switch(i) 
            {
                case 0: logic_row = 7; break; // Visual Top    -> Logic Bottom (White Pieces)
                case 1: logic_row = 6; break;
                case 2: logic_row = 5; break;
                case 3: logic_row = 4; break;
                case 4: logic_row = 3; break;
                case 5: logic_row = 2; break;
                case 6: logic_row = 1; break;
                case 7: logic_row = 0; break; // Visual Bottom -> Logic Top (Black Pieces)
            }
        }
        // -------------------------------------

        // Calculate index using the switched 'logic_row'.
        // We keep 'j' as-is to align with the 'a-h' labels on screen.
        int index = logic_row * ROW + j;

        // Color Logic
        if((m[index].isWhite)) new[i][j].color = BLACK; // GUI Blue
        else new[i][j].color = RED;   // GUI Red
        
        // Map Coordinates
        new[i][j].x = j; 
        new[i][j].y = i; 
        
        // Piece Mapping
        switch(m[index].name) {
          case PAWN:   new[i][j].type = pawn;   break;
          case ROOK:   new[i][j].type = rook;   break;
          case BISHOP: new[i][j].type = bishop; break;
          case KNIGHT: new[i][j].type = knight; break;
          case QUEEN:  new[i][j].type = queen;  break;
          case KING:   new[i][j].type = king;   break;
          default:     
              new[i][j].color = NONE;
              new[i][j].type = empty;
              break;
        }
      }
  }
  return new;
}


int connectWithServer(int* client_fd, struct sockaddr_in* serverAddress) {
    int fd = 0;
    *client_fd = 0;
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;
    serverAddress->sin_family = AF_INET;
    serverAddress->sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress->sin_addr) <= 0) return -1;
    if (connect(fd,(struct sockaddr*)serverAddress,sizeof(*serverAddress))< 0) return -2;
    *client_fd = fd;
    return 0;
}

int sendMessageToServer(int client_fd, char* message) {
    if(strlen(message) >= SIZE_OF_MESSAGE) 
    {
		return -1;
    }
    
    if(send(client_fd, message, strlen(message) + 1, 0) == -1)  // + 1 - for \0
    {
		return -2;
    }
    
    return 0;
}

char *receiveMessageFromServer(int client_fd) {
    char *message = calloc(SIZE_OF_MESSAGE, sizeof(char));
    if (!message) return NULL;
    int i = 0; char c;
    while (i < SIZE_OF_MESSAGE - 1) {
        if (recv(client_fd, &c, 1, 0) <= 0) { free(message); return NULL; }
        message[i++] = c;
        if (c == '\0') break; 
    }
    return message;
}
/*
int receiveMatrixFromServer(int client_fd, Element_T* element) {
    size_t total_size = ROW * COLUMN * sizeof(Element_T);
    if(recv(client_fd, element, total_size, MSG_WAITALL) <= 0) return -1;
    return 0;
}
*/

int receiveMatrixFromServer(int client_fd, Element_T* element) {
    size_t total_bytes_expected = ROW * COLUMN * sizeof(Element_T);
    size_t bytes_received = 0;
    char* buffer = (char*)element;

    while (bytes_received < total_bytes_expected) {
        ssize_t res = recv(client_fd, buffer + bytes_received, total_bytes_expected - bytes_received, 0);
        
        if (res < 0) {
            return -1; // Error
        } else if (res == 0) {
            return -2; // Connection closed by server
        }
        
        bytes_received += res;
    }
    return 0;
}

int checkIfQuitMessage(char* message) {
    if(strcmp(message, "quit") == 0) return 0;
    return 1;
}



int main(int argc, char** argv)
{
    enable_utf8_locale();
    initscr(); 
    cbreak(); 
    noecho(); 
    keypad(stdscr, TRUE);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    const int W = 8 * CELL_W, H = 8 * CELL_H;
    int oy = (rows - H) / 2; if(oy < 2) oy = 2;
    int ox = (cols - W) / 2; if(ox < 4) ox = 4;

    type_text_on_window(oy - 1, ox, "Welcome to the Chess Game! :3", 2000);

    int client_fd = 0;
    struct sockaddr_in serverAddress;

    type_text_on_window(oy, ox, "Connecting to server...", 300);
    mvprintw(oy, ox, "%s", "                                "); 

    if(connectWithServer(&client_fd, &serverAddress) != 0)
    {
        type_text_on_window(oy, ox, "Error connecting to server.", 300);
        getch();
        endwin();
        return 0;
    }

    type_text_on_window(oy, ox, "Successfully connected!", 300);
    napms(500);
    mvprintw(oy, ox, "%s", "                                ");
    mvprintw(oy - 1, ox, "%s", "                                                              ");
 
    char* message = malloc(sizeof(char)*SIZE_OF_MESSAGE);
    char* quitMessage = malloc(sizeof(char)*SIZE_OF_MESSAGE);

    if(message == NULL || quitMessage == NULL)
    {
        endwin();
        perror("Malloc Error");
        exit(1);
    }

    strcpy(quitMessage, "quit");

    char your_username[SIZE_OF_MESSAGE];
    char* oponent_username = NULL;

    bool isWhite;

    int current_input_y = oy + 1;

    type_text_on_window(current_input_y, ox, "Type username: ", 1000);
    char *aux = read_from_window(stdscr, current_input_y, ox + strlen("Type username: "));

    while(strlen(aux) > SIZE_OF_MESSAGE - 1)

    {
        free(aux);
        type_text_on_window(oy + 5, ox, "Error: Username too long. (< 128 chars)", 500);
        napms(1500);
        mvprintw(oy + 5, ox, "                                                                ");

        move(current_input_y, ox);
        clrtoeol(); 

        current_input_y = oy + 6; 
        
        mvprintw(current_input_y, ox, "Type username: ");
        aux = read_from_window(stdscr, current_input_y, ox + strlen("Type username: "));

    }
    strcpy(your_username, aux);
    free(aux);
    move(current_input_y, ox);
    clrtoeol(); 

    type_text_on_window(oy + 1, ox, "Your username: ", 300);
    mvprintw(oy + 1, ox + strlen("Your username: "), "%s", your_username);

    sendMessageToServer(client_fd, your_username);

    type_text_on_window(oy + 2, ox, "Searching for opponent...", 500);
    
    oponent_username = receiveMessageFromServer(client_fd);
    char* color_msg = receiveMessageFromServer(client_fd);
    isWhite = (color_msg && strcmp("white", color_msg) == 0);
    if(color_msg) free(color_msg);
    
    mvprintw(oy + 2, ox, "                                ");
    if(oponent_username) {
       // Strip generic suffix if present
       size_t len = strlen(oponent_username);
       if (len > 5 && (strcmp(oponent_username + len - 5, "black") || strcmp(oponent_username + len - 5, "white")) == 0) 
       {
           oponent_username[len - 5] = '\0';
	   }
	   
       type_text_on_window(oy + 2, ox, "Opponent found: ", 500);
       type_text_on_window(oy + 2, ox + strlen("Opponent found: "), oponent_username, 500);
       refresh();
    }
    napms(1000);
    type_text_on_window(oy + 4, ox, "You are ", 300);
    if(isWhite)
    {
        type_text_on_window(oy + 4, ox + strlen("You are "), "white.", 300);
    }
    else
    {
        type_text_on_window(oy + 4, ox + strlen("You are "), "black.", 300);
    }
    napms(1000);
    
    for(int k=0; k<6; k++) 
    {
        mvprintw(oy + k, ox, "                                         ");
    }
    
    
    
    Element_T* matrix_data = (Element_T*)malloc(sizeof(Element_T)*ROW*COLUMN);
    piece_t **gui_matrix = NULL;

    char* init_msg = receiveMessageFromServer(client_fd);
    if(init_msg) free(init_msg);

    draw_board(oy, ox);
    
    int name_top_y = oy - 2;
    int name_bot_y = oy + 17; 
    int turn_y     = oy + 18; 
    int input_y    = oy + 19; 
    int alert_y    = oy + 20; 

    while(1)
    {
        if(oponent_username) 
        {
            mvprintw(name_top_y, ox, "Opponent: %s", oponent_username);
        }
        mvprintw(name_bot_y, ox, "(You: %s)", your_username);
        refresh();

        char* server_cmd = receiveMessageFromServer(client_fd);
        if(server_cmd == NULL) break;
        
        if(strcmp(server_cmd, "printGame") == 0)
        {
			napms(500);
			mvprintw(alert_y, ox, "                         ");
			mvprintw(oy + 16, ox, "  a   b   c   d   e   f   g   h");
            if(receiveMatrixFromServer(client_fd, matrix_data) == 0)
            {
                if(gui_matrix) free_gui_matrix(gui_matrix);
                gui_matrix = convert_raul_to_ana(matrix_data, isWhite);

                draw_board(oy, ox);
                draw_pieces(oy, ox, gui_matrix);
                refresh();
            }
        }
        else if(strcmp(server_cmd, "error") == 0)
        {
             char* err_text = receiveMessageFromServer(client_fd);
             if(err_text) {
                 mvprintw(alert_y, ox, "%s", err_text);
                 refresh();
                 napms(2000);
                 mvprintw(alert_y, ox, "                                          ");
                 free(err_text);
             }
        }

        else if(strcmp(server_cmd, "wait") == 0)
        {
            mvprintw(turn_y, ox, "Waiting...                              ");
            mvprintw(input_y, ox, "                  ");
            refresh();
        }
        else if(strcmp(server_cmd, "quit") == 0) {
            type_text_on_window(oy, ox, "You WON (Opponent Quit)!", 2000);
            break;
        }
        else if(strcmp(server_cmd, "rematch") == 0) {
            mvprintw(alert_y, ox, "Rematch? (yes/no): ");
            char *resp = read_from_window(stdscr, alert_y, ox + 19);
            sendMessageToServer(client_fd, (strcmp(resp,"yes")==0) ? "yes" : "no");
            free(resp);
            mvprintw(alert_y, ox, "                                         ");
        }
        else if(strcmp(server_cmd, "draw") == 0) {
            mvprintw(alert_y, ox, "Draw offer? (yes/no): ");
            char *resp = read_from_window(stdscr, alert_y, ox + 22);
            sendMessageToServer(client_fd, (strcmp(resp,"yes")==0) ? "yes" : "no");
            free(resp);
            mvprintw(alert_y, ox, "                                         ");
        }
        else if(strcmp(server_cmd, "PrintWin") == 0) {
             type_text_on_window(alert_y, ox, "You WON!", 2000);
        }
        else if(strcmp(server_cmd, "PrintLose") == 0) {
             type_text_on_window(alert_y, ox, "You LOST!", 2000);
        }
        else if(strcmp(server_cmd, "whiteCheck") == 0) {
             mvprintw(alert_y, ox, "White is in CHECK!");
        }
        else if(strcmp(server_cmd, "blackCheck") == 0) {
             mvprintw(alert_y, ox, "Black is in CHECK!");
        }
        
        
        
        if(strcmp(server_cmd, "yourTurn") == 0)
        {
            mvprintw(turn_y, ox, "Your Turn!                      ");
            mvprintw(input_y, ox, "type move: ");
            refresh(); 
            
            char *user_move = read_from_window(stdscr, input_y, ox + strlen("type move: "));
            
            if (!isWhite && strlen(user_move) >= 4) 
            {
                // If Black, the board is visually flipped (Rank 1 is Logic Rank 8).
                // We must invert the Rank digits in the move string.
                // '1' becomes '8', '2' becomes '7', etc.
                // Formula: new_char = '9' - old_char
                if(isdigit(user_move[1])) 
                {
					user_move[1] = '9' - (user_move[1] - '0');
				}
                if(isdigit(user_move[3])) 
                {
					user_move[3] = '9' - (user_move[3] - '0');
				}
            }
            
            strcpy(message, user_move);
            free(user_move);
            
            if(checkIfQuitMessage(message) == 0) 
            {
                sendMessageToServer(client_fd, quitMessage);
                break;
            }
            sendMessageToServer(client_fd, message);
        }
        else // opponent's turn
        {
			mvprintw(turn_y, ox, "                                                                  ");
			mvprintw(turn_y, ox, "Opponent's turn! Waiting...              ");
            mvprintw(input_y, ox, "                                     ");
            refresh(); 
		}
        
        free(server_cmd);
    }
    

    free(matrix_data);
    free_gui_matrix(gui_matrix);
    free(message);
    free(quitMessage);
    if(oponent_username) free(oponent_username);
    
    endwin();
    return 0;
}
