#pragma once

#include <array>

// Core game constants and data structures shared by the game, UI, and AI.

inline constexpr int BOARD_SIZE = 8;   // standard 8x8 board

enum Piece {
    Empty,
    TealMan,
    PurpleMan,
    TealKing,
    PurpleKing
};

struct GameState {
    std::array<std::array<Piece, BOARD_SIZE>, BOARD_SIZE> board{};
    Piece currentPlayer = TealMan;
    int selectedRow = -1;
    int selectedCol = -1;
};

/**
 * @brief Checks if the given row and column coordinates are within the board bounds.
 * @param r Row index (0-based)
 * @param c Column index (0-based)
 * @return true if both coordinates are within [0, BOARD_SIZE), false otherwise
 */
bool inBounds(int r, int c);

/**
 * @brief Determines if a square at the given coordinates is a dark square (playable square).
 * @param r Row index (0-based)
 * @param c Column index (0-based)
 * @return true if the square is dark (playable), false if it's light (non-playable)
 */
bool isDarkSquare(int r, int c);

/**
 * @brief Initializes the game board with starting positions for both players.
 * Places Teal pieces on the top 3 rows and Purple pieces on the bottom 3 rows,
 * only on dark squares.
 * @param state The game state to initialize. The board will be cleared and repopulated.
 */
void initBoard(GameState &state);

/**
 * @brief Applies a move if it is legal according to checkers rules.
 * Validates the move, performs captures if applicable, and handles kinging.
 * @param state The current game state (will be modified if move is valid)
 * @param sr Source row index (0-based)
 * @param sc Source column index (0-based)
 * @param tr Target row index (0-based)
 * @param tc Target column index (0-based)
 * @param wasCapture Output parameter set to true if a capture occurred, false otherwise
 * @return true if the move was valid and applied, false if the move was invalid
 */
bool applyMove(GameState &state, int sr, int sc, int tr, int tc, bool &wasCapture);

/**
 * @brief Counts the total number of pieces (men + kings) for each player.
 * @param state The game state to analyze
 * @param tealCount Output parameter for the number of Teal pieces
 * @param purpleCount Output parameter for the number of Purple pieces
 */
void countPieces(const GameState &state, int &tealCount, int &purpleCount);

/**
 * @brief Checks if the given player has at least one legal move available.
 * @param state The current game state
 * @param player The piece type representing the player (TealMan, PurpleMan, TealKing, or PurpleKing)
 * @return true if the player has at least one legal move, false otherwise
 */
bool hasAnyMoves(const GameState &state, Piece player);

/**
 * @brief Checks if a piece belongs to the Teal player.
 * @param p The piece to check
 * @return true if the piece is TealMan or TealKing, false otherwise
 */
inline bool isTealPiece(Piece p) {
    return p == TealMan || p == TealKing;
}

/**
 * @brief Checks if a piece belongs to the Purple player.
 * @param p The piece to check
 * @return true if the piece is PurpleMan or PurpleKing, false otherwise
 */
inline bool isPurplePiece(Piece p) {
    return p == PurpleMan || p == PurpleKing;
}


