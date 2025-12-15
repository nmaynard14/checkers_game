#include "GameLogic.h"

#include <cmath>

/**
 * @brief Checks if the given row and column coordinates are within the board bounds.
 * @param r Row index (0-based)
 * @param c Column index (0-based)
 * @return true if both coordinates are within [0, BOARD_SIZE), false otherwise
 */
bool inBounds(int r, int c) {
    return r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE;
}

/**
 * @brief Determines if a square at the given coordinates is a dark square (playable square).
 * @param r Row index (0-based)
 * @param c Column index (0-based)
 * @return true if the square is dark (playable), false if it's light (non-playable)
 */
bool isDarkSquare(int r, int c) {
    return ((r + c) % 2 == 1);
}

/**
 * @brief Initializes the game board with starting positions for both players.
 * Places Teal pieces on the top 3 rows and Purple pieces on the bottom 3 rows,
 * only on dark squares.
 * @param state The game state to initialize. The board will be cleared and repopulated.
 */
void initBoard(GameState &state) {
    for (auto &row : state.board) {
        row.fill(Empty);
    }

    const int rowsPerSide = 3;

    // Teal at top, moving "down"
    for (int r = 0; r < rowsPerSide; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (isDarkSquare(r, c)) {
                state.board[r][c] = TealMan;
            }
        }
    }

    // Purple at bottom, moving "up"
    for (int r = BOARD_SIZE - rowsPerSide; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (isDarkSquare(r, c)) {
                state.board[r][c] = PurpleMan;
            }
        }
    }
}

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
bool applyMove(GameState &state, int sr, int sc, int tr, int tc, bool &wasCapture) {
    wasCapture = false;

    if (!inBounds(sr, sc) || !inBounds(tr, tc)) return false;
    if (!isDarkSquare(tr, tc)) return false;        // must land on dark square
    if (state.board[tr][tc] != Empty) return false; // target must be empty

    Piece piece = state.board[sr][sc];
    if (piece == Empty) return false;

    bool tealPiece = isTealPiece(piece);
    bool purplePiece = isPurplePiece(piece);
    if (!tealPiece && !purplePiece) return false;

    int dr = tr - sr;
    int dc = tc - sc;

    bool isKing = (piece == TealKing || piece == PurpleKing);

    // Allowed movement directions in terms of row delta
    int forwardDir = tealPiece ? 1 : -1;
    auto isAllowedDir = [&](int deltaRow) {
        if (deltaRow == forwardDir) return true;
        if (isKing && deltaRow == -forwardDir) return true;
        return false;
    };

    // Simple move: one step diagonally in allowed directions
    if (std::abs(dc) == 1 && std::abs(dr) == 1 && isAllowedDir(dr)) {
        state.board[tr][tc] = piece;
        state.board[sr][sc] = Empty;
    } else if (std::abs(dc) == 2 && std::abs(dr) == 2 && isAllowedDir(dr / 2)) {
        // Capture move: two steps diagonally, jumping over opponent
        int mr = sr + dr / 2;
        int mc = sc + dc / 2;
        if (!inBounds(mr, mc)) return false;
        Piece mid = state.board[mr][mc];
        if (mid == Empty) return false;

        if ((tealPiece && !isPurplePiece(mid)) || (purplePiece && !isTealPiece(mid))) {
            return false; // must capture opponent
        }

        state.board[mr][mc] = Empty;
        state.board[tr][tc] = piece;
        state.board[sr][sc] = Empty;
        wasCapture = true;
    } else {
        return false;
    }

    // Handle kinging
    if (piece == TealMan && tr == BOARD_SIZE - 1) {
        state.board[tr][tc] = TealKing;
    } else if (piece == PurpleMan && tr == 0) {
        state.board[tr][tc] = PurpleKing;
    }

    return true;
}

/**
 * @brief Counts the total number of pieces (men + kings) for each player.
 * @param state The game state to analyze
 * @param tealCount Output parameter for the number of Teal pieces
 * @param purpleCount Output parameter for the number of Purple pieces
 */
void countPieces(const GameState &state, int &tealCount, int &purpleCount) {
    tealCount = 0;
    purpleCount = 0;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            Piece pc = state.board[r][c];
            if (isTealPiece(pc)) ++tealCount;
            else if (isPurplePiece(pc)) ++purpleCount;
        }
    }
}

/**
 * @brief Checks if the given player has at least one legal move available.
 * @param state The current game state
 * @param player The piece type representing the player (TealMan, PurpleMan, TealKing, or PurpleKing)
 * @return true if the player has at least one legal move, false otherwise
 */
bool hasAnyMoves(const GameState &state, Piece player) {
    bool lookTeal = isTealPiece(player);
    bool lookPurple = isPurplePiece(player);

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            Piece piece = state.board[r][c];
            if (piece == Empty) continue;
            if (lookTeal && !isTealPiece(piece)) continue;
            if (lookPurple && !isPurplePiece(piece)) continue;

            bool tealPiece = isTealPiece(piece);
            int forwardDir = tealPiece ? 1 : -1;
            bool isKing = (piece == TealKing || piece == PurpleKing);

            for (int dc = -2; dc <= 2; ++dc) {
                if (dc == 0) continue;
                for (int step = 1; step <= 2; ++step) {
                    int dr = step;
                    for (int dirSign : {1, -1}) {
                        int actualDr = dr * dirSign;
                        // Non-king pieces cannot move backwards
                        if (!isKing) {
                            if (actualDr != forwardDir && actualDr != 2 * forwardDir)
                                continue;
                        }
                        int tr = r + actualDr;
                        int tc = c + (dc > 0 ? step : -step);

                        bool dummyCapture = false;
                        GameState tmp = state;
                        if (applyMove(tmp, r, c, tr, tc, dummyCapture)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}


