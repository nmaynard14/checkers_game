// Simple 8x8 checkers game using Raylib for 3D graphics
// Teal vs Purple pieces, with a sidebar showing remaining piece counts and AI opponent.

#include <iostream>

#include "GameLogic.h"
#include "CheckersAI.h"
#include "SoundManager.h"
#include "Renderer.h"

/**
 * @brief Handles mouse click events on the game board.
 * Manages piece selection and move execution for the human player.
 * @param state The current game state (will be modified if a move is made)
 * @param renderer The renderer for coordinate conversion
 * @param wasCapture Output parameter set to true if the move resulted in a capture, false otherwise
 * @return true if a move was successfully executed, false otherwise (selection change or invalid click)
 */
bool handleClick(GameState &state, Renderer &renderer, bool &wasCapture) {
    Vector2 mousePos = renderer.getMousePosition();
    int mouseX = (int)mousePos.x;
    int mouseY = (int)mousePos.y;
    
    // Determine active player for camera positioning
    bool isTealActive = (state.currentPlayer == TealMan || state.currentPlayer == TealKing);
    
    // Convert screen coordinates to board coordinates using 3D picking
    int row, col;
    renderer.screenToBoard(mouseX, mouseY, row, col, isTealActive);
    if (row == -1 || col == -1) return false;

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

    // Initialize renderer (handles Raylib window and 3D rendering)
    Renderer renderer;
    if (!renderer.initialize()) {
        return 1;
    }

    // Initialize sound manager
    SoundManager soundManager;

    // Show difficulty selection menu
    AIDifficulty selectedDifficulty = AIDifficulty::Medium;
    int menuSelection = -1;
    bool showingMenu = true;
    
    while (showingMenu && !renderer.shouldClose()) {
        menuSelection = renderer.renderDifficultyMenu(static_cast<int>(selectedDifficulty));
        
        if (menuSelection >= 0 && menuSelection <= 2) {
            selectedDifficulty = static_cast<AIDifficulty>(menuSelection);
            showingMenu = false;
        }
    }
    
    if (renderer.shouldClose()) {
        return 0;
    }

    GameState state;
    initBoard(state);

    CheckersAI ai(selectedDifficulty);
    GameResult result = GameResult::Ongoing;
    bool showPopup = false;

    bool running = true;
    while (running && !renderer.shouldClose()) {
        // Check if current player has no valid moves (they lose immediately)
        if (result == GameResult::Ongoing) {
            int tealCount = 0, purpleCount = 0;
            countPieces(state, tealCount, purpleCount);
            
            // Check if current player has no moves
            if (state.currentPlayer == TealMan || state.currentPlayer == TealKing) {
                if (tealCount == 0 || !hasAnyMoves(state, TealMan)) {
                    result = GameResult::PurpleWin;
                    soundManager.playLose();
                    showPopup = true;
                }
            } else if (state.currentPlayer == PurpleMan || state.currentPlayer == PurpleKing) {
                if (purpleCount == 0 || !hasAnyMoves(state, PurpleMan)) {
                    result = GameResult::TealWin;
                    soundManager.playWin();
                    showPopup = true;
                }
            }
        }
        
        // Handle input
        if (renderer.isMouseButtonPressed()) {
            if (showPopup) {
                // Handle popup clicks (simplified - just close on click for now)
                // In a full implementation, you'd check button bounds
                showPopup = false;
            } else if (result == GameResult::Ongoing && state.currentPlayer == TealMan) {
                bool humanCapture = false;
                bool movedByHuman = handleClick(state, renderer, humanCapture);
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
                        showPopup = true;
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
                        showPopup = true;
                    } else {
                        state.currentPlayer = TealMan;
                    }
                }
            }
        }

        // Render game
        renderer.renderGame(state);
        
        // Render popup overlay if needed
        if (showPopup) {
            renderer.beginDrawing();
            // Draw semi-transparent overlay
            DrawRectangle(0, 0, 800, 800, (Color){0, 0, 0, 180});
            
            // Draw popup box
            int popupWidth = 400;
            int popupHeight = 220;
            int popupX = (800 - popupWidth) / 2;
            int popupY = (800 - popupHeight) / 2;
            
            DrawRectangle(popupX, popupY, popupWidth, popupHeight, (Color){30, 30, 30, 255});
            DrawRectangleLines(popupX, popupY, popupWidth, popupHeight, WHITE);
            
            // Draw message
            const char* msg = (result == GameResult::TealWin) ? "You Win!" : "You Lose!";
            int textWidth = MeasureText(msg, 40);
            DrawText(msg, popupX + (popupWidth - textWidth) / 2, popupY + 40, 40, YELLOW);
            
            // Draw buttons
            int btnY = popupY + popupHeight - 70;
            int btnW = 140;
            int btnH = 40;
            
            // New Game button
            DrawRectangle(popupX + 50, btnY, btnW, btnH, (Color){70, 70, 70, 255});
            DrawRectangleLines(popupX + 50, btnY, btnW, btnH, WHITE);
            int ngTextWidth = MeasureText("New Game", 20);
            DrawText("New Game", popupX + 50 + (btnW - ngTextWidth) / 2, btnY + 10, 20, WHITE);
            
            // Exit button
            DrawRectangle(popupX + 210, btnY, btnW, btnH, (Color){70, 70, 70, 255});
            DrawRectangleLines(popupX + 210, btnY, btnW, btnH, WHITE);
            int exTextWidth = MeasureText("Exit", 20);
            DrawText("Exit", popupX + 210 + (btnW - exTextWidth) / 2, btnY + 10, 20, WHITE);
            
            // Handle button clicks
            Vector2 mousePos = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (mousePos.x >= popupX + 50 && mousePos.x <= popupX + 50 + btnW &&
                    mousePos.y >= btnY && mousePos.y <= btnY + btnH) {
                    // New Game
                    initBoard(state);
                    state.currentPlayer = TealMan;
                    state.selectedRow = state.selectedCol = -1;
                    result = GameResult::Ongoing;
                    showPopup = false;
                } else if (mousePos.x >= popupX + 210 && mousePos.x <= popupX + 210 + btnW &&
                           mousePos.y >= btnY && mousePos.y <= btnY + btnH) {
                    // Exit
                    running = false;
                }
            }
            
            renderer.endDrawing();
        }
    }

    return 0;
}
