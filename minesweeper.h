#ifndef __MINESWEEPER_H__
#define __MINESWEEPER_H__

#include<iostream>
#include<limits.h>
#include<stdexcept>
#include<time.h>
#include<sstream>
using namespace std;

struct Game {
    private:
    // Private variables
        char **hiddenBoard; // Board for mine positions / numbers
        char **board; // Board that player sees
        int rows; // Rows on the board
        int cols; // Columns on the board
        int mines; // Number of mines for this game
        int flagCount; // Number of flags placed
        string diffSelected; // String representation of the difficulty selected

    // Private functions
        /// @brief Function to override << operator
        friend ostream &operator<<(ostream &stream, const Game &game);
        /// @brief Initializes the hidden board with random mines for this game
        void CreateMineBoard();
        /// @brief Initializes the board visible to the player
        void CreatePlayerBoard();
        /// @brief Adds numbers around every mine on the hidden board
        void AddBoardNumbers();
        /// @brief Increments all cells surrounding the given cell that do not contain mines
        /// @param cellR The row of the cell
        /// @param cellC The column of the cell
        void IncrementCells(int cellR, int cellC);

    public:
    // Public variables
        bool active; // Determines if the game loop should continue
        int moveCount; // Move count, only increments if the player makes a valid move

    // Public methods
        /// @brief Creates a new minesweeper board with a given difficulty setting
        /// @param difficulty 1 for easy, 2 for intermediate, 3 for expert
        Game(int difficulty);
        /// @brief Attempts to reveal the specified cell
        /// @param r Row of the cell
        /// @param c Column of the cell
        /// @return True if the cell was able to be revealed
        bool RevealCell(int r, int c);
        /// @brief Attempts to flag the specified cell
        /// @param r Row of the cell
        /// @param c Column of the cell
        /// @return True if the cell was able to be flagged
        bool FlagCell(int r, int c);
        /// @brief Determines if the given cell is a valid cell
        /// @param r The row of the cell
        /// @param c The column of the cell
        /// @return True if the cell is within the confines of this game
        bool IsValidCell(int r, int c);
        /// @brief Checks to see if the game is finished, i.e. all tiles are revealed except for the mines
        /// @return True if the game is completed
        bool HasCompletedGame();
        /// @brief Destructor for Game object
        ~Game();
};
#endif
