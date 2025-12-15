#pragma once

#include <random>

#include "GameLogic.h"

/**
 * @brief Represents the difficulty level of the AI opponent.
 */
enum class AIDifficulty {
    Easy = 0,    ///< AI makes optimal moves 30% of the time
    Medium = 1,  ///< AI makes optimal moves 60% of the time
    Hard = 2     ///< AI makes optimal moves 100% of the time
};

/**
 * @brief AI opponent for the Purple player in the checkers game.
 * Uses a simple evaluation function and makes optimal moves based on difficulty level,
 * otherwise choosing randomly from legal moves.
 */
class CheckersAI {
public:
    /**
     * @brief Constructs a new CheckersAI instance.
     * @param difficulty The difficulty level (Easy, Medium, or Hard)
     * Initializes the random number generator used for move selection.
     */
    CheckersAI(AIDifficulty difficulty = AIDifficulty::Medium);

    /**
     * @brief Chooses a move for the Purple player based on the current game state.
     * Evaluates all legal moves and selects either the best move (based on difficulty) or
     * a random legal move.
     * @param state The current game state
     * @param srcRow Output parameter for the source row index of the chosen move
     * @param srcCol Output parameter for the source column index of the chosen move
     * @param dstRow Output parameter for the destination row index of the chosen move
     * @param dstCol Output parameter for the destination column index of the chosen move
     * @return true if a valid move was found, false if no moves are available
     */
    bool chooseMove(const GameState &state,
                    int &srcRow, int &srcCol,
                    int &dstRow, int &dstCol);

private:
    std::mt19937 rng;
    AIDifficulty difficulty;  ///< The difficulty level of the AI
    float optimalMoveChance; ///< Probability of making the optimal move (0.0 to 1.0)
};


