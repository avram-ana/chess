```
 _______  __   __  _______  _______  _______    _______  _______  __   __  _______ 
|       ||  | |  ||       ||       ||       |  |       ||   _   ||  |_|  ||       |
|       ||  |_|  ||    ___||  _____||  _____|  |    ___||  |_|  ||       ||    ___|
|       ||       ||   |___ | |_____ | |_____   |   | __ |       ||       ||   |___ 
|      _||       ||    ___||_____  ||_____  |  |   ||  ||       ||       ||    ___|
|     |_ |   _   ||   |___  _____| | _____| |  |   |_| ||   _   || ||_|| ||   |___ 
|_______||__| |__||_______||_______||_______|  |_______||__| |__||_|   |_||_______|
```

This C code implements a classic chess game. 
The ncurses library has been used for the GUI.
```
         _                          _     
        | |                        | |    
      __| |_ __ __ ___      __     | |__  
     / _` | '__/ _` \ \ /\ / /     | '_ \ 
    | (_| | | | (_| |\ V  V /   _  | | | |
     \__,_|_|  \__,_| \_/\_/   (_) |_| |_|
```

### The header contains functions that manage the graphical user interface.
### This module is responsible for rendering the chessboard and pieces using ncurses, 
### handling user input and displaying textual feedback.
###
    
  - __enable_utf8_locale__ : enables UTF-8 support so chess Unicode symbols render correctly in the terminal.
    
  - __init_colors__ : initializes ncurses color pairs used for board squares and pieces.
  - __draw_board__ : draws the chessboard grid starting at offset (oy, ox).
  - __draw_pieces__ : renders all chess pieces on the board based on the current matrix.
  - __get_square_color__ : determines the current square color.
  - __put_piece__ : draws a single piece glyph at board position (r, c) with the appropriate color pair.
    
  - __type_text_on_window__ : displays animated text (typing effect) at a given position for a given duration.
  - __show_what_is_being_typed__ : prints characters as the user types them in a window.
  - __read_from_window__ : reads a full string input from the user at a specified window position (backspace key is also being handled).

```
          _                              _             _            _
         | |                            | |           (_)          | |    
      ___| |__   ___  ___ ___           | | ___   __ _ _  ___      | |__  
     / __| '_ \ / _ \/ __/ __|          | |/ _ \ / _` | |/ __|     | '_ \ 
    | (__| | | |  __/\__ \__ \  ______  | | (_) | (_| | | (__   _  | | | |
     \___|_| |_|\___||___/___/ |______/ |_|\___/ \__, |_|\___| (_) |_| |_|
                                                  __/ |                   
                                                 |___/                    
```
### This header defines the core data structures and functions responsible for managing 
### the internal state of the chess game. It operates independently of the graphical interface 
### and implements the core chess logic, including move validation and game state management.
###
  - __Initialize_Classic_Game_Matrix__ : creates and returns a standard chess starting board.
  - __Initialize_Classic_Game__ : allocates and initializes a full game state with players and board.
  - __Reinstate_Game__ : resets the game to an initial chess board.
    
  - __Apply_Move_Matrix__ : applies a valid move to the board matrix.
  - __Game_Move__ : validates and executes a player move, updating game state accordingly.
    
  - __print_Matrix__ : prints the current matrix in the terminal (used for debugging purposes)
  - __Print_Game_Result__ : displays the final result when the game ends.

        How to play
- The game expects moves in the following format:
- The first two characters represent the starting square, and the last two characters represent the destination square.
   - Columns: a-h
   - Rows: 1–8
- Moves must be provided in coordinate notation, without separators.
   - _For example: a1a2 / b2h2_

<img width="497" height="616" alt="image" src="https://github.com/user-attachments/assets/59e714d0-0830-4c49-863c-8f7e9727af67" />
