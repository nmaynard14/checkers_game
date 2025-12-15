#include "CheckersAI.h"

#include <vector>
#include <cmath>
#include <limits>

namespace {

/**
 * @brief Internal structure representing a possible move.
 */
struct Move {
    int sr, sc;      ///< Source row and column
    int tr, tc;      ///< Target row and column
    bool isCapture;  ///< Whether this move captures an opponent piece
};

/**
 * @brief Evaluates a game position from the Purple player's perspective.
 * Returns the difference between Purple and Teal piece counts.
 * @param state The game state to evaluate
 * @return Positive value if Purple is ahead, negative if Teal is ahead
 */
int evaluatePosition(const GameState &state) {
    int tealCount = 0;
    int purpleCount = 0;
    countPieces(state, tealCount, purpleCount);
    return purpleCount - tealCount;
}

} // namespace

/**
 * @brief Constructs a new CheckersAI instance.
 * Initializes the random number generator used for move selection.
 */
CheckersAI::CheckersAI()
    : rng(std::random_device{}()) {}

/**
 * @brief Chooses a move for the Purple player based on the current game state.
 * Evaluates all legal moves and selects either the best move (50% chance) or
 * a random legal move (50% chance).
 * @param state The current game state
 * @param srcRow Output parameter for the source row index of the chosen move
 * @param srcCol Output parameter for the source column index of the chosen move
 * @param dstRow Output parameter for the destination row index of the chosen move
 * @param dstCol Output parameter for the destination column index of the chosen move
 * @return true if a valid move was found, false if no moves are available
 */
bool CheckersAI::chooseMove(const GameState &state,
                            int &srcRow, int &srcCol,
                            int &dstRow, int &dstCol) {
    std::vector<Move> allMoves;

    // Generate all legal moves for Purple (men + kings).
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            Piece p = state.board[r][c];
            if (!(p == PurpleMan || p == PurpleKing)) continue;

            bool dummyCapture = false;
            // Try all possible target squares in a 2-step diagonal neighborhood.
            for (int dr = -2; dr <= 2; ++dr) {
                if (dr == 0) continue;
                for (int dc = -2; dc <= 2; ++dc) {
                    if (dc == 0) continue;
                    int tr = r + dr;
                    int tc = c + dc;
                    GameState tmp = state;
                    if (applyMove(tmp, r, c, tr, tc, dummyCapture)) {
                        allMoves.push_back({r, c, tr, tc, dummyCapture});
                    }
                }
            }
        }
    }

    if (allMoves.empty()) {
        return false;
    }

    // Evaluate moves and find the best ones.
    std::vector<Move> bestMoves;
    int bestScore = std::numeric_limits<int>::min();

    for (const auto &m : allMoves) {
        GameState tmp = state;
        bool dummyCapture = false;
        applyMove(tmp, m.sr, m.sc, m.tr, m.tc, dummyCapture);

        int score = evaluatePosition(tmp);
        if (m.isCapture) {
            score += 2; // prefer captures slightly more
        }

        if (score > bestScore) {
            bestScore = score;
            bestMoves.clear();
            bestMoves.push_back(m);
        } else if (score == bestScore) {
            bestMoves.push_back(m);
        }
    }

    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<std::size_t> pickAll(0, allMoves.size() - 1);

    Move chosen;
    if (!bestMoves.empty() && prob(rng) < 0.5) {
        // 50% of the time, pick from the best moves.
        std::uniform_int_distribution<std::size_t> pickBest(0, bestMoves.size() - 1);
        chosen = bestMoves[pickBest(rng)];
    } else {
        // Otherwise, pick any legal move.
        chosen = allMoves[pickAll(rng)];
    }

    srcRow = chosen.sr;
    srcCol = chosen.sc;
    dstRow = chosen.tr;
    dstCol = chosen.tc;
    return true;
}


