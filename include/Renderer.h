#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
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
 * @brief Manages all video rendering for the checkers game using OpenGL for 3D graphics.
 * Handles window creation, 3D rendering of the game board and pieces, UI elements, and popup windows.
 */
class Renderer {
public:
    /**
     * @brief Constructs a new Renderer and initializes SDL video, OpenGL, and TTF subsystems.
     * Creates the main game window with OpenGL context and loads the font.
     * @return true if initialization succeeded, false otherwise
     */
    bool initialize();

    /**
     * @brief Destructor that cleans up all SDL and OpenGL resources (window, context, font).
     */
    ~Renderer();

    /**
     * @brief Renders the game board, pieces, and UI elements in 3D using OpenGL.
     * Draws a 3D checkered board, 3D cylindrical pieces with crown models for kings,
     * the sidebar with piece counts, and highlights the currently selected piece.
     * @param state The current game state to render
     */
    void renderGame(const GameState &state);

    /**
     * @brief Opens a popup window for victory/defeat messages.
     * @param result The game result (TealWin or PurpleWin)
     * @return true if popup was successfully created, false otherwise
     */
    bool openPopup(GameResult result);

    /**
     * @brief Closes the popup window if it's currently open.
     */
    void closePopup();

    /**
     * @brief Renders the popup window with victory/defeat message and buttons.
     * @param result The game result to display
     */
    void renderPopup(GameResult result);

    /**
     * @brief Checks if the popup window is currently active.
     * @return true if popup is open, false otherwise
     */
    bool isPopupActive() const { return popup.active; }

    /**
     * @brief Gets the window ID of the popup window.
     * @return The SDL window ID, or 0 if popup is not active
     */
    Uint32 getPopupWindowID() const;

    /**
     * @brief Checks if a click is within the "New Game" button bounds.
     * @param x Mouse X coordinate in popup window
     * @param y Mouse Y coordinate in popup window
     * @return true if click is within button bounds, false otherwise
     */
    bool isNewGameButton(int x, int y) const;

    /**
     * @brief Checks if a click is within the "Exit" button bounds.
     * @param x Mouse X coordinate in popup window
     * @param y Mouse Y coordinate in popup window
     * @return true if click is within button bounds, false otherwise
     */
    bool isExitButton(int x, int y) const;

    /**
     * @brief Gets the main window ID.
     * @return The SDL window ID of the main game window
     */
    Uint32 getMainWindowID() const { return SDL_GetWindowID(window); }

    /**
     * @brief Converts screen coordinates to 3D board coordinates.
     * Used for mouse picking in the 3D scene.
     * @param mouseX Screen X coordinate
     * @param mouseY Screen Y coordinate
     * @param outRow Output parameter for board row (-1 if invalid)
     * @param outCol Output parameter for board column (-1 if invalid)
     */
    void screenToBoard(int mouseX, int mouseY, int &outRow, int &outCol) const;

private:
    static constexpr float CELL_SIZE = 1.0f;      ///< Size of each board cell in 3D units
    static constexpr float PIECE_HEIGHT = 0.3f;   ///< Height of pieces
    static constexpr float PIECE_RADIUS = 0.35f;   ///< Radius of pieces
    static constexpr float BOARD_HEIGHT = 0.1f;   ///< Height/thickness of board squares
    static constexpr int SIDEBAR_WIDTH = 200;     ///< Width of the sidebar in pixels
    static constexpr int WINDOW_WIDTH = 800;      ///< Window width in pixels
    static constexpr int WINDOW_HEIGHT = 800;     ///< Window height in pixels

    SDL_Window *window;      ///< Main game window
    SDL_GLContext glContext; ///< OpenGL context
    TTF_Font *font;         ///< Font for text rendering (used for UI overlay)

    // Camera parameters
    float cameraX = 0.0f;    ///< Camera X position
    float cameraY = 4.0f;    ///< Camera Y position (height)
    float cameraZ = 6.0f;    ///< Camera Z position (distance)
    float cameraAngleX = -30.0f; ///< Camera rotation around X axis (degrees)
    float cameraAngleY = 45.0f;  ///< Camera rotation around Y axis (degrees)

    struct Popup {
        bool active = false;
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Rect newGameBtn{0, 0, 0, 0};
        SDL_Rect exitBtn{0, 0, 0, 0};
    } popup;

    /**
     * @brief Sets up OpenGL rendering state (lighting, depth testing, etc.)
     */
    void setupOpenGL();

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
     * Uses immediate mode rendering over the 3D scene.
     * @param tealCount Number of teal pieces
     * @param purpleCount Number of purple pieces
     */
    void renderUIOverlay(int tealCount, int purpleCount);
};

