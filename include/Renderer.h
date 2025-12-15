#pragma once

#include <raylib.h>
#include "GameLogic.h"

/**
 * @brief Represents the result of a game.
 */
enum class GameResult {
    Ongoing,   ///< Game is still in progress
    TealWin,   ///< Teal player has won
    PurpleWin  ///< Purple player has won
};

/**
 * @brief Manages all video rendering for the checkers game using Raylib for 3D graphics.
 * Handles window creation, 3D rendering of the game board and pieces, UI elements, and popup windows.
 */
class Renderer {
public:
    /**
     * @brief Constructs a new Renderer and initializes Raylib.
     * Creates the main game window and sets up 3D camera.
     * @return true if initialization succeeded, false otherwise
     */
    bool initialize();

    /**
     * @brief Destructor that cleans up all Raylib resources.
     */
    ~Renderer();

    /**
     * @brief Renders the game board, pieces, and UI elements in 3D using Raylib.
     * Draws a 3D checkered board, 3D cylindrical pieces with crown models for kings,
     * the sidebar with piece counts, and highlights the currently selected piece.
     * @param state The current game state to render
     */
    void renderGame(const GameState &state);

    /**
     * @brief Checks if the window should close.
     * @return true if window close was requested, false otherwise
     */
    bool shouldClose() const;

    /**
     * @brief Begins drawing frame (call before rendering).
     */
    void beginDrawing();

    /**
     * @brief Ends drawing frame and swaps buffers (call after rendering).
     */
    void endDrawing();

    /**
     * @brief Converts screen coordinates to 3D board coordinates.
     * Used for mouse picking in the 3D scene.
     * @param mouseX Screen X coordinate
     * @param mouseY Screen Y coordinate
     * @param outRow Output parameter for board row (-1 if invalid)
     * @param outCol Output parameter for board column (-1 if invalid)
     * @param isTealActive Whether Teal is the active player (for camera positioning)
     */
    void screenToBoard(int mouseX, int mouseY, int &outRow, int &outCol, bool isTealActive) const;

    /**
     * @brief Gets the mouse position.
     * @return Vector2 with mouse x and y coordinates
     */
    Vector2 getMousePosition() const;

    /**
     * @brief Checks if the left mouse button was pressed this frame.
     * @return true if left mouse button was pressed, false otherwise
     */
    bool isMouseButtonPressed() const;

private:
    static constexpr float CELL_SIZE = 1.0f;      ///< Size of each board cell in 3D units
    static constexpr float PIECE_HEIGHT = 0.3f;   ///< Height of pieces
    static constexpr float PIECE_RADIUS = 0.35f;  ///< Radius of pieces
    static constexpr int SIDEBAR_WIDTH = 200;     ///< Width of the sidebar in pixels
    static constexpr int WINDOW_WIDTH = 800;      ///< Window width in pixels
    static constexpr int WINDOW_HEIGHT = 800;     ///< Window height in pixels

    Camera3D camera;  ///< 3D camera for viewing the board
    Font font;        ///< Font for text rendering

    // Camera parameters
    float cameraDistance = 12.0f;  ///< Distance from board center
    float cameraAngle = 60.0f;     ///< Angle from horizontal (degrees)

    /**
     * @brief Updates camera position based on active player.
     * @param isTealActive Whether Teal is the active player
     */
    void updateCamera(bool isTealActive);

    /**
     * @brief Renders a single board square in 3D.
     * @param row Board row (0-7)
     * @param col Board column (0-7)
     * @param isDark Whether this is a dark square
     * @param isSelected Whether this square is currently selected
     */
    void renderBoardSquare(int row, int col, bool isDark, bool isSelected);

    /**
     * @brief Renders a 3D cylindrical piece.
     * @param x X position in 3D space
     * @param z Z position in 3D space
     * @param isTeal Whether this is a teal piece (false = purple)
     * @param isKing Whether this is a king piece
     */
    void renderPiece(float x, float z, bool isTeal, bool isKing);

    /**
     * @brief Renders a 3D crown on top of a king piece.
     * @param x X position in 3D space
     * @param z Z position in 3D space
     */
    void renderCrown(float x, float z);

    /**
     * @brief Renders the 2D UI overlay (sidebar with piece counts).
     * @param tealCount Number of teal pieces
     * @param purpleCount Number of purple pieces
     */
    void renderUIOverlay(int tealCount, int purpleCount);
};
