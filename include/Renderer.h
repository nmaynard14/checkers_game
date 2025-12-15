#pragma once

#include <SDL2/SDL.h>
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
 * @brief Manages all video rendering for the checkers game.
 * Handles window creation, rendering the game board, pieces, UI elements, and popup windows.
 */
class Renderer {
public:
    /**
     * @brief Constructs a new Renderer and initializes SDL video and TTF subsystems.
     * Creates the main game window and renderer, and loads the font.
     * @return true if initialization succeeded, false otherwise
     */
    bool initialize();

    /**
     * @brief Destructor that cleans up all SDL resources (window, renderer, font).
     */
    ~Renderer();

    /**
     * @brief Renders the game board, pieces, and UI elements to the screen.
     * Draws the checkered board, all pieces with crowns for kings, the sidebar with
     * piece counts, and highlights the currently selected piece.
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
     * @brief Gets the main SDL renderer.
     * @return Pointer to the SDL renderer, or nullptr if not initialized
     */
    SDL_Renderer* getRenderer() const { return renderer; }

private:
    static constexpr int CELL_SIZE = 80;      ///< Pixels per board cell
    static constexpr int SIDEBAR_WIDTH = 200; ///< Width of the sidebar in pixels
    static constexpr int WINDOW_WIDTH = BOARD_SIZE * CELL_SIZE + SIDEBAR_WIDTH;
    static constexpr int WINDOW_HEIGHT = BOARD_SIZE * CELL_SIZE;

    SDL_Window *window;      ///< Main game window
    SDL_Renderer *renderer;  ///< Main game renderer
    TTF_Font *font;         ///< Font for text rendering

    struct Popup {
        bool active = false;
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Rect newGameBtn{0, 0, 0, 0};
        SDL_Rect exitBtn{0, 0, 0, 0};
    } popup;
};

