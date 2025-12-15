// Simple 8x8 checkers game using SDL2 (+ SDL2_ttf for numeric UI text)
// Teal vs Purple pieces, with a sidebar showing remaining piece counts and AI opponent.

#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <string>

#include "GameLogic.h"
#include "CheckersAI.h"
#include "SoundManager.h"
#include "Renderer.h"

static constexpr int CELL_SIZE = 80;  // pixels per cell (must match Renderer::CELL_SIZE)

/**
 * @brief Handles mouse click events on the game board.
 * Manages piece selection and move execution for the human player.
 * @param state The current game state (will be modified if a move is made)
 * @param mouseX The X coordinate of the mouse click in pixels
 * @param mouseY The Y coordinate of the mouse click in pixels
 * @param wasCapture Output parameter set to true if the move resulted in a capture, false otherwise
 * @return true if a move was successfully executed, false otherwise (selection change or invalid click)
 */
bool handleClick(GameState &state, int mouseX, int mouseY, bool &wasCapture) {
    // Ignore clicks in the sidebar area (sidebar starts at BOARD_SIZE * CELL_SIZE)
    if (mouseX >= BOARD_SIZE * CELL_SIZE) {
        return false;
    }

    int col = mouseX / CELL_SIZE;
    int row = mouseY / CELL_SIZE;
    if (!inBounds(row, col)) return false;

    Piece clicked = state.board[row][col];

    // No selection yet
    if (state.selectedRow == -1) {
        bool isOwnPiece = (state.currentPlayer == TealMan && isTealPiece(clicked)) ||
                          (state.currentPlayer == PurpleMan && isPurplePiece(clicked));
        if (isOwnPiece) {
            state.selectedRow = row;
            state.selectedCol = col;
        }
        return false;
    }

    // Clicking own piece changes selection
    bool isOwnPiece = (state.currentPlayer == TealMan && isTealPiece(clicked)) ||
                      (state.currentPlayer == PurpleMan && isPurplePiece(clicked));
    if (isOwnPiece) {
        state.selectedRow = row;
        state.selectedCol = col;
        return false;
    }

    // Try to move
    wasCapture = false;
    if (applyMove(state, state.selectedRow, state.selectedCol, row, col, wasCapture)) {
        state.selectedRow = -1;
        state.selectedCol = -1;
        return true;
    }

    return false;
}


int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Initialize renderer (handles SDL video and TTF)
    Renderer renderer;
    if (!renderer.initialize()) {
        return 1;
    }

    // Initialize sound manager
    SoundManager soundManager;

    GameState state;
    initBoard(state);

    CheckersAI ai;
    GameResult result = GameResult::Ongoing;


    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN &&
                       e.button.button == SDL_BUTTON_LEFT) {
                // Clicks in popup window
                if (renderer.isPopupActive() &&
                    e.button.windowID == renderer.getPopupWindowID()) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    if (renderer.isNewGameButton(mx, my)) {
                        // New game
                        initBoard(state);
                        state.currentPlayer = TealMan;
                        state.selectedRow = state.selectedCol = -1;
                        result = GameResult::Ongoing;
                        renderer.closePopup();
                    } else if (renderer.isExitButton(mx, my)) {
                        running = false;
                    }
                } else if (result == GameResult::Ongoing && state.currentPlayer == TealMan &&
                           e.button.windowID == renderer.getMainWindowID()) {
                    bool humanCapture = false;
                    bool movedByHuman = handleClick(state, e.button.x, e.button.y, humanCapture);
                    if (movedByHuman) {
                        if (!humanCapture)
                            soundManager.playMove();
                        else
                            soundManager.playCapture();
                        // After human move, check if AI has any pieces or moves.
                        int tealCount = 0, purpleCount = 0;
                        countPieces(state, tealCount, purpleCount);
                        if (purpleCount == 0 || !hasAnyMoves(state, PurpleMan)) {
                            result = GameResult::TealWin;
                            soundManager.playWin();
                            renderer.openPopup(result);
                            continue;
                        }

                        // Switch to AI (Purple)
                        state.currentPlayer = PurpleMan;

                        int sr, sc, tr, tc;
                        if (ai.chooseMove(state, sr, sc, tr, tc)) {
                            bool aiCapture = false;
                            applyMove(state, sr, sc, tr, tc, aiCapture);
                            if (!aiCapture)
                                soundManager.playMove();
                            else
                                soundManager.playCapture();
                        }

                        state.selectedRow = -1;
                        state.selectedCol = -1;
                        // After AI move, check if player has any pieces or moves.
                        countPieces(state, tealCount, purpleCount);
                        if (tealCount == 0 || !hasAnyMoves(state, TealMan)) {
                            result = GameResult::PurpleWin;
                            soundManager.playLose();
                            renderer.openPopup(result);
                        } else {
                            state.currentPlayer = TealMan;
                        }
                    }
                }
            }
        }

        renderer.renderGame(state);
        if (renderer.isPopupActive()) {
            renderer.renderPopup(result);
        }
        SDL_Delay(10);
    }

    return 0;
}
