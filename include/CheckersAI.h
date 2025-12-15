#pragma once

#include <random>

#include "GameLogic.h"

/**
 * @brief AI opponent for the Purple player in the checkers game.
 * Uses a simple evaluation function and makes optimal moves approximately 50% of the time,
 * otherwise choosing randomly from legal moves.
 */
class CheckersAI {
public:
    /**
     * @brief Constructs a new CheckersAI instance.
     * Initializes the random number generator used for move selection.
     */
    CheckersAI();

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
    bool chooseMove(const GameState &state,
                    int &srcRow, int &srcCol,
                    int &dstRow, int &dstCol);

private:
    std::mt19937 rng;
};


