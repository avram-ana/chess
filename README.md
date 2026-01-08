
 _______  __   __  _______  _______  _______    _______  _______  __   __  _______ 
|       ||  | |  ||       ||       ||       |  |       ||   _   ||  |_|  ||       |
|       ||  |_|  ||    ___||  _____||  _____|  |    ___||  |_|  ||       ||    ___|
|       ||       ||   |___ | |_____ | |_____   |   | __ |       ||       ||   |___ 
|      _||       ||    ___||_____  ||_____  |  |   ||  ||       ||       ||    ___|
|     |_ |   _   ||   |___  _____| | _____| |  |   |_| ||   _   || ||_|| ||   |___ 
|_______||__| |__||_______||_______||_______|  |_______||__| |__||_|   |_||_______|
                                                                

This C code implements a classic chess game. 
The ncurses library has been used for the GUI.
/*
         _                          _     
        | |                        | |    
      __| |_ __ __ ___      __     | |__  
     / _` | '__/ _` \ \ /\ / /     | '_ \ 
    | (_| | | | (_| |\ V  V /   _  | | | |
     \__,_|_|  \__,_| \_/\_/   (_) |_| |_|
*/

The header contains functions that manage the GUI:
  - enable_utf8_locale : enables UTF-8 support so chess Unicode symbols render correctly in the terminal.
    
  - init_colors : initializes ncurses color pairs used for board squares and pieces.
  - draw_board : draws the chessboard grid starting at offset (oy, ox).
  - draw_pieces : renders all chess pieces on the board based on the current matrix.
  - get_square_color : determines the current square color.
  - put_piece : draws a single piece glyph at board position (r, c) with the appropriate color pair.
    
  - type_text_on_window : displays animated text (typing effect) at a given position for a given duration.
  - show_what_is_being_typed : prints characters as the user types them in a window.
  - read_from_window : reads a full string input from the user at a specified window position (backspace key is also being handled).

/*
          _                              _             _            _
         | |                            | |           (_)          | |    
      ___| |__   ___  ___ ___           | | ___   __ _ _  ___      | |__  
     / __| '_ \ / _ \/ __/ __|          | |/ _ \ / _` | |/ __|     | '_ \ 
    | (__| | | |  __/\__ \__ \  ______  | | (_) | (_| | | (__   _  | | | |
     \___|_| |_|\___||___/___/ |______/ |_|\___/ \__, |_|\___| (_) |_| |_|
                                                  __/ |                   
                                                 |___/                    
*/

  - Initialize_Classic_Game_Matrix : creates and returns a standard chess starting board.
  - Initialize_Classic_Game : allocates and initializes a full game state with players and board.
  - Reinstate_Game : resets the game to an initial chess board.
    
  - Apply_Move_Matrix : applies a valid move to the board matrix.
  - Game_Move : validates and executes a player move, updating game state accordingly.
    
  - print_Matrix : prints the current matrix in the terminal (used for debugging purposes)
  - Print_Game_Result : displays the final result when the game ends.

        How to play
The game expects moves in the following format:
The first two characters represent the starting square, and the last two characters represent the destination square.
        Columns: a-h
        Rows: 1–8
Moves must be provided in coordinate notation, without separators.
        For example : a1a2 / b2b3

<img width="777" height="979" alt="image" src="https://github.com/user-attachments/assets/b829a0c8-4803-407a-9fa2-7e76068ed358" />
