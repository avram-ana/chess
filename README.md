```
    _________ .__                             ________                       
    \_   ___ \|  |__   ____   ______ ______  /  _____/_____    _____   ____  
    /    \  \/|  |  \_/ __ \ /  ___//  ___/ /   \  ___\__  \  /     \_/ __ \ 
    \     \___|   Y  \  ___/ \___ \ \___ \  \    \_\  \/ __ \|  Y Y  \  ___/ 
     \______  /___|  /\___  >____  >____  >  \______  (____  /__|_|  /\___  >
            \/     \/     \/     \/     \/          \/     \/      \/     \/ 
```

 This project implements a terminal-based multiplayer chess game written in C, featuring move validation, 
game state tracking, and real-time gameplay over TCP sockets. The ncurses library is used to render the 
chessboard, the pieces and to handle interactive input.

## ♟️ Features
- Terminal-based chess interface using **ncurses**
- Multiplayer gameplay using **TCP sockets**
- **Client–server architecture**
- **POSIX threads** for concurrent game sessions
- Unicode chess pieces with **UTF-8 rendering**
- Move validation and game state management
- Support for **draw requests**, **rematch**, and **quit**

```
     _____ _                      _                 _      
    /  __ \ |                    | |               (_)     
    | /  \/ |__   ___  ___ ___   | |     ___   __ _ _  ___ 
    | |   | '_ \ / _ \/ __/ __|  | |    / _ \ / _` | |/ __|
    | \__/\ | | |  __/\__ \__ \  | |___| (_) | (_| | | (__ 
     \____/_| |_|\___||___/___/  \_____/\___/ \__, |_|\___|
                                               __/ |       
                                              |___/        
```
  The code defines the core data structures and functions responsible for managing 
 the internal state of the chess game. It operates independently of the graphical interface 
 and implements the core chess logic, including move validation and game state management.⚙️
```
     _   _      _                      _    _             
    | \ | |    | |                    | |  (_)            
    |  \| | ___| |___      _____  _ __| | ___ _ __   __ _ 
    | . ` |/ _ \ __\ \ /\ / / _ \| '__| |/ / | '_ \ / _` |
    | |\  |  __/ |_ \ V  V / (_) | |  |   <| | | | | (_| |
    \_| \_/\___|\__| \_/\_/ \___/|_|  |_|\_\_|_| |_|\__, |
                                                     __/ |
                                                    |___/ 
```
  The server implements the multiplayer logic using TCP sockets and POSIX threads. It listens for 
incoming connections, pairs two clients into a match, and creates a thread for each game 
session. The server manages turn order, validates moves using the chess logic module, synchronizes 
the board state between clients, and handles events such as check notifications, draw requests, player 
disconnections and rematches. 🌐

```
     _____ _   _ _____ 
    |  __ \ | | |_   _|
    | |  \/ | | | | |  
    | | __| | | | | |  
    | |_\ \ |_| |_| |_ 
     \____/\___/ \___/ 
```

The graphical interface of the game is implemented using the ncurses library, allowing the chessboard and 
pieces to be rendered directly in the terminal. 🖥️

The GUI module is responsible for:
- drawing the chessboard grid
- rendering chess pieces using Unicode chess symbols
- handling keyboard input from the player
- displaying feedback messages and game status
- UTF-8 rendering for proper display of chess glyphs


## How to play
🚀 The game expects moves in the following format:
  - The first two characters represent the starting square, and the last two characters represent the destination square.
      - Columns: a-h
      - Rows: 1–8
        
  - Moves must be provided in coordinate notation, without separators.
      - _For example: a1a2 / b2h2_
        
- The provided **Makefile** automates compilation and linking of all modules, including the ncurses dependency required
  for the terminal interface. Build the project with:

      make
- After compiling, start the server and connect two clients to begin a match.

  
<img width="497" height="616" alt="image" src="https://github.com/user-attachments/assets/59e714d0-0830-4c49-863c-8f7e9727af67" />
