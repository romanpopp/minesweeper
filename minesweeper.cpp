#include "minesweeper.h"

/// @brief Generates a pseudo random integer from min to max using rand()
/// @param min Minimum value, inclusive
/// @param max Maximum value, inclusive
/// @return A random integer number
int randInt(int min, int max) {
    return min + (int) ((double) rand() / RAND_MAX * (max - min + 1));
}

/// @brief A game of minesweeper

// ------------------------------------------------------------------------------------------------------------------ //
// ----------------------------------------------  Game Setup Methods  ---------------------------------------------- //
// ------------------------------------------------------------------------------------------------------------------ //

Game::Game(int difficulty) {
    active = true;
    switch (difficulty) {
        case 1: // Easy, 8x8 board with 10 mines
        rows = 8; cols = 8; mines = 10; diffSelected = "EASY"; break;
        case 2: // Intermediate, 16x16 board with 40 mines
        rows = 16; cols = 16; mines = 40; diffSelected = "INTERMEDIATE"; break;
        case 3: // Expert, 30x16 board with 99 mines
        rows = 16; cols = 30; mines = 99; diffSelected = "EXPERT"; break;
        default:
        throw invalid_argument("Invalid difficulty.");
    }
    moveCount = 0;
    flagCount = mines;
    CreateMineBoard();
    AddBoardNumbers();
    CreatePlayerBoard();
}

void Game::CreateMineBoard() {
    // Initializing empty mineBoard array
    hiddenBoard = new char * [rows];
    for (int i = 0; i < rows; i++) {
        hiddenBoard[i] = new char[cols];
    }
    // Setting every character in this 2D array to '.'
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            hiddenBoard[r][c] = '0';
        }
    }
    // Adding random mines
    for (int i = mines; i > 0; i--) {
        int r = randInt(0, rows - 1);
        int c = randInt(0, cols - 1);
        if (hiddenBoard[r][c] == '0') {
            hiddenBoard[r][c] = 'X';
        }
        else { 
            i++;
        }
    }
}

void Game::CreatePlayerBoard() {
    // Initializing empty board array
    board = new char * [rows];
    for (int i = 0; i < rows; i++) {
        board[i] = new char[cols];
    }
    // Setting all positions to a blank '.'
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            board[r][c] = '.';
        }
    }
}

void Game::AddBoardNumbers() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // Check if this position contains a mine
            if (hiddenBoard[r][c] == 'X') {
                IncrementCells(r, c);
            }
        }
    }
}

void Game::IncrementCells(int r, int c) {
    for (int n = r - 1; n <= r + 1; n++) {
        for (int m = c - 1; m <= c + 1; m++) {
            if (IsValidCell(n, m) && hiddenBoard[n][m] != 'X') {
                hiddenBoard[n][m]++;
            }
        }
    }
}

Game::~Game() {
    for (int i = 0; i < rows; i++) {
        delete[] hiddenBoard[i];
        delete[] board[i];
    }
    delete[] hiddenBoard;
    delete[] board;
}

// ------------------------------------------------------------------------------------------------------------------ //
// ---------------------------------------------  Methods for Gameplay  --------------------------------------------- //
// ------------------------------------------------------------------------------------------------------------------ //

bool Game::RevealCell(int r, int c) {
    // Only allows removal of unflagged and unrevealed cells
    if (board[r][c] != '.') {
        return false;
    }
    // Reveal this cell!
    board[r][c] = hiddenBoard[r][c];

    // Checking if the game has been lost
    if (board[r][c] == 'X') {
        active = false;
        return true; // Still a valid cell reveal, even if you lost 
    }
    else {
        // Cells with no mines nearby will auto reveal
        if (board[r][c] == '0') { 
            board[r][c] = ' ';

            // Reveal all surrounding cells
            for (int row = r - 1; row <= r + 1; row++) {
                for (int col = c - 1; col <= c + 1; col++) {
                    if (IsValidCell(row, col) && board[row][col] == '.') {
                        RevealCell(row, col);
                    }
                }
            }
        }
        return true; // Cell was revealed successfully
    }
}

bool Game::FlagCell(int r, int c) {
    if (board[r][c] == '*') {
        board[r][c] = '.';
        flagCount++;
        return true;
    }
    else if (board[r][c] != '.') {
        return false; // Can't flag revealed cells
    }
    flagCount--;
    board[r][c] = '*';
    return true;
}

bool Game::IsValidCell(int r, int c) {
    // Making sure the cell is within the bounds of the game
    if (r >= rows || r < 0 || c >= cols || c < 0) {
        return false; // :(
    }
    return true; // :)
}

bool Game::HasCompletedGame() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // If this tile is not a mine but still hasn't been revealed
            if (hiddenBoard[r][c] != 'X' && (board[r][c] == '.' || board[r][c] == '*')) {
                return false;
            }
        }
    }
    return true;
}

ostream &operator<<(ostream &stream, const Game &game) {
    // Information above board
    stream << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" 
        << game.diffSelected << " | "
        << game.flagCount << " Flags " << endl << endl
        << "  | ";
    // Column numbers
    for (int i = 0; i < game.cols; i++) {
        stream << i + 1;
        if (i < 8) { stream << " "; }
        stream << " ";
    }
    stream << endl << "--+";
    for (int i = 0; i < game.cols; i++) {
        stream << "---";
    }
    stream << endl;
    // Adding the board
    for (int r = 0; r < game.rows; r++) {
        // Row numbers
        stream << r + 1;
        if (r < 9) {
            stream << " ";
        } 
        stream << "| ";
        for (int c = 0; c < game.cols; c++) {
            stream << game.board[r][c] << "  ";
        }
        stream << endl;
    }
    stream << endl;
    return stream;
}

// ------------------------------------------------------------------------------------------------------------------ //
// ---------------------------------------------------  Main Loop  -------------------------------------------------- //
// ------------------------------------------------------------------------------------------------------------------ //

/// @brief Main game loop
int main() {
    // Getting seed for rand() using system time
    srand((unsigned) time(NULL));
    
    while (true) {
        // Getting game difficulty
        int choice = 0;
        cout << "MINESWEEPER difficulty options:\n"
            << "(1) EASY \t\t |   8x8, 10 mines\n"
            << "(2) INTERMEDIATE \t | 16x16, 40 mines\n"
            << "(3) EXPERT \t\t | 30x16, 99 mines\n\n" 
            << "Please choose a difficulty: ";
        while (choice == 0) {
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 3) {
                choice = 0;
                cin.clear(); // Reset failbit
                cin.ignore(999999, '\n'); // Skip bad input
                cout << "Please choose a difficulty: ";
            }
        }
        Game game(choice); // Initialize this game

        // Removes newline character from previous input
        cin.ignore(999999, '\n');

        // Game loop
        bool win = false;
        while (game.active) {
            // Prints the board position + asks for input
            cout << game << "Select a cell to reveal (ROW, COLUMN): ";
            int x = 0; int y = 0; // Input variables
            string choice;

            // Determines whether this play is checking or flagging a cell
            bool flag = false; 

            while (x <= 0 || y <= 0) {
                getline(cin, choice);
                bool beforeComma = true; // Determines whether to add to X or Y
                
                // Checking input for 'F' in first position to indicate a flag
                if (choice[0] == 'F') {
                    flag = true;
                    choice = choice.substr(1);
                }
                // Checking the input string
                for (int i = 0; i < choice.length(); i++) {
                    // Character = space, ignore and continue
                    if (choice[i] == ' ') {
                        continue;
                    }
                    // Character = comma, swap to counting Y
                    else if (choice[i] == ',') {
                        beforeComma = false;
                    }
                    // Adds digits to X until comma
                    else if (beforeComma) {
                        x *= 10;
                        x += choice[i] - 48;
                    }
                    else {
                        y *= 10;
                        y += choice[i] - 48;
                    }
                }

                // Checking to make sure this is a valid pair
                if (!game.IsValidCell(x - 1, y - 1)) {
                    x = 0; y = 0;
                    cout << "Invalid input."  
                        << "\n\n***Type input as an ordered pair, with an F to indicate a flag.***"
                        << "\n|\tEX: 4,5 reveals the cell at row 4, column 5"
                        << "\n|\tEX: F3,8 flags the cell at row 3, column 8"
                        << "\n\nEnter a position: ";
                }
            }

            // Reveals or flags the given cell
            if (flag) {
                game.FlagCell(x - 1, y - 1);
            }
            else {
                game.RevealCell(x - 1, y - 1);
            }

            // Check for game completion
            if (game.HasCompletedGame()) {
                win = true;
                game.active = false;
            }
        }

        // Game end sequence
        if (win) {
            cout << game << "You win!";

        }
        else {
            cout << game << "Game over!";
        }
        cin.ignore(999999, '\n');

        string playAgain;
        cout << "\nPlay again? ('n' for no): ";
        cin >> playAgain;
        if (playAgain[0] == 'n' || playAgain[0] == 'N') {
            return 0;
        }
        cout << endl;
    }
}
