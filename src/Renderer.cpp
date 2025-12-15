#include "Renderer.h"

#include <cmath>
#include <iostream>
#include <string>

/**
 * @brief Constructs a new Renderer and initializes SDL video and TTF subsystems.
 * Creates the main game window and renderer, and loads the font.
 * @return true if initialization succeeded, false otherwise
 */
bool Renderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow(
        "8x8 Checkers - Teal vs Purple (SDL2)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Use a common system font path; adjust if needed for your system.
    font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << "\n";
    }

    return true;
}

/**
 * @brief Destructor that cleans up all SDL resources (window, renderer, font).
 */
Renderer::~Renderer() {
    closePopup();

    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

/**
 * @brief Renders the game board, pieces, and UI elements to the screen.
 * Draws the checkered board, all pieces with crowns for kings, the sidebar with
 * piece counts, and highlights the currently selected piece.
 * @param state The current game state to render
 */
void Renderer::renderGame(const GameState &state) {
    // Clear background
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    // Draw board
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            SDL_Rect cell{c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            bool dark = isDarkSquare(r, c);
            if (dark)
                SDL_SetRenderDrawColor(renderer, 118, 150, 86, 255);   // dark green
            else
                SDL_SetRenderDrawColor(renderer, 238, 238, 210, 255); // light
            SDL_RenderFillRect(renderer, &cell);

            // Highlight selection
            if (r == state.selectedRow && c == state.selectedCol) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_Rect border{cell.x + 2, cell.y + 2, cell.w - 4, cell.h - 4};
                SDL_RenderDrawRect(renderer, &border);
            }
        }
    }

    // Draw pieces as circles
    int tealCount = 0;
    int purpleCount = 0;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            Piece pc = state.board[r][c];
            if (pc == Empty) continue;

            if (isTealPiece(pc))  ++tealCount;
            if (isPurplePiece(pc)) ++purpleCount;

            int cx = c * CELL_SIZE + CELL_SIZE / 2;
            int cy = r * CELL_SIZE + CELL_SIZE / 2;
            int radius = CELL_SIZE / 2 - 8;

            Uint8 cr = 0, cg = 0, cb = 0;
            if (isTealPiece(pc)) {      // teal
                cr = 0; cg = 128; cb = 128;
            } else if (isPurplePiece(pc)) { // purple
                cr = 128; cg = 0; cb = 128;
            }

            SDL_SetRenderDrawColor(renderer, cr, cg, cb, 255);

            // Simple filled circle drawing
            for (int dy = -radius; dy <= radius; ++dy) {
                for (int dx = -radius; dx <= radius; ++dx) {
                    if (dx * dx + dy * dy <= radius * radius) {
                        SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
                    }
                }
            }

            // Outline
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            for (int angle = 0; angle < 360; ++angle) {
                double rad = angle * M_PI / 180.0;
                int x = cx + static_cast<int>(radius * std::cos(rad));
                int y = cy + static_cast<int>(radius * std::sin(rad));
                SDL_RenderDrawPoint(renderer, x, y);
            }

            // Draw crown on kings (roughly 70% of piece size)
            if (pc == TealKing || pc == PurpleKing) {
                int crownWidth  = static_cast<int>(radius * 1.4);   // ~70% of diameter
                int crownHeight = static_cast<int>(radius * 0.8);
                int crownX = cx - crownWidth / 2;
                int crownY = cy - crownHeight / 2;

                // Crown color: bright gold
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);

                // Base of crown
                SDL_Rect base{crownX, crownY + (crownHeight * 2) / 3,
                              crownWidth, crownHeight / 3};
                SDL_RenderFillRect(renderer, &base);

                // Three points
                int pointWidth = crownWidth / 5;
                int pointHeight = (crownHeight * 2) / 3;
                for (int i = 0; i < 3; ++i) {
                    int px = crownX + (i + 1) * crownWidth / 4 - pointWidth / 2;
                    SDL_Rect point{px, crownY, pointWidth, pointHeight};
                    SDL_RenderFillRect(renderer, &point);
                }
            }
        }
    }

    // Sidebar background
    SDL_Rect sidebar{BOARD_SIZE * CELL_SIZE, 0, SIDEBAR_WIDTH, WINDOW_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(renderer, &sidebar);

    // Draw piece count bars (simple visual UI)
    int sidebarX = BOARD_SIZE * CELL_SIZE + 20;
    int topY = 40;

    // Teal label square
    SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
    SDL_Rect tealRect{sidebarX, topY, 30, 30};
    SDL_RenderFillRect(renderer, &tealRect);

    // Purple label square
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
    SDL_Rect purpleRect{sidebarX, topY + 80, 30, 30};
    SDL_RenderFillRect(renderer, &purpleRect);

    // Draw numeric counts via SDL_ttf
    if (font) {
        SDL_Color white{255, 255, 255, 255};

        std::string tealText = "Teal: " + std::to_string(tealCount);
        SDL_Surface *tealSurf = TTF_RenderText_Blended(font, tealText.c_str(), white);
        if (tealSurf) {
            SDL_Texture *tealTex = SDL_CreateTextureFromSurface(renderer, tealSurf);
            if (tealTex) {
                SDL_Rect dst{sidebarX, topY - 30, tealSurf->w, tealSurf->h};
                SDL_RenderCopy(renderer, tealTex, nullptr, &dst);
                SDL_DestroyTexture(tealTex);
            }
            SDL_FreeSurface(tealSurf);
        }

        std::string purpleText = "Purple: " + std::to_string(purpleCount);
        SDL_Surface *purpleSurf = TTF_RenderText_Blended(font, purpleText.c_str(), white);
        if (purpleSurf) {
            SDL_Texture *purpleTex = SDL_CreateTextureFromSurface(renderer, purpleSurf);
            if (purpleTex) {
                SDL_Rect dst{sidebarX, topY + 50, purpleSurf->w, purpleSurf->h};
                SDL_RenderCopy(renderer, purpleTex, nullptr, &dst);
                SDL_DestroyTexture(purpleTex);
            }
            SDL_FreeSurface(purpleSurf);
        }
    }

    SDL_RenderPresent(renderer);
}

/**
 * @brief Opens a popup window for victory/defeat messages.
 * @param result The game result (TealWin or PurpleWin)
 * @return true if popup was successfully created, false otherwise
 */
bool Renderer::openPopup(GameResult result) {
    if (popup.active) return true;

    popup.active = true;
    int pw = 400;
    int ph = 220;
    popup.window = SDL_CreateWindow(
        (result == GameResult::TealWin) ? "Victory" : "Defeat",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        pw, ph,
        SDL_WINDOW_SHOWN
    );
    if (!popup.window) {
        std::cerr << "SDL_CreateWindow (popup) Error: " << SDL_GetError() << "\n";
        popup.active = false;
        return false;
    }
    popup.renderer = SDL_CreateRenderer(
        popup.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!popup.renderer) {
        std::cerr << "SDL_CreateRenderer (popup) Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(popup.window);
        popup.window = nullptr;
        popup.active = false;
        return false;
    }
    // Layout buttons
    int btnW = 140;
    int btnH = 40;
    int centerX = pw / 2;
    int y = ph - 70;
    popup.newGameBtn = {centerX - btnW - 10, y, btnW, btnH};
    popup.exitBtn    = {centerX + 10,       y, btnW, btnH};
    return true;
}

/**
 * @brief Closes the popup window if it's currently open.
 */
void Renderer::closePopup() {
    if (!popup.active) return;
    if (popup.renderer) {
        SDL_DestroyRenderer(popup.renderer);
        popup.renderer = nullptr;
    }
    if (popup.window) {
        SDL_DestroyWindow(popup.window);
        popup.window = nullptr;
    }
    popup.active = false;
}

/**
 * @brief Renders the popup window with victory/defeat message and buttons.
 * @param result The game result to display
 */
void Renderer::renderPopup(GameResult result) {
    if (!popup.active || !popup.renderer) return;

    SDL_SetRenderDrawColor(popup.renderer, 30, 30, 30, 255);
    SDL_RenderClear(popup.renderer);

    // Draw buttons first so labels appear on top
    SDL_SetRenderDrawColor(popup.renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(popup.renderer, &popup.newGameBtn);
    SDL_RenderFillRect(popup.renderer, &popup.exitBtn);

    SDL_SetRenderDrawColor(popup.renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(popup.renderer, &popup.newGameBtn);
    SDL_RenderDrawRect(popup.renderer, &popup.exitBtn);

    if (font) {
        SDL_Color yellow{255, 215, 0, 255};
        const char *msg = (result == GameResult::TealWin) ? "You Win!" : "You Lose!";
        SDL_Surface *surf = TTF_RenderText_Blended(font, msg, yellow);
        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(popup.renderer, surf);
            if (tex) {
                int w = surf->w;
                int h = surf->h;
                SDL_Rect dst{(400 - w) / 2, 40, w, h};
                SDL_RenderCopy(popup.renderer, tex, nullptr, &dst);
                SDL_DestroyTexture(tex);
            }
            SDL_FreeSurface(surf);
        }

        SDL_Color white{255, 255, 255, 255};
        SDL_Surface *ngSurf = TTF_RenderText_Blended(font, "New Game", white);
        SDL_Surface *exSurf = TTF_RenderText_Blended(font, "Exit", white);

        if (ngSurf) {
            SDL_Texture *ngTex = SDL_CreateTextureFromSurface(popup.renderer, ngSurf);
            if (ngTex) {
                SDL_Rect dst{popup.newGameBtn.x + (popup.newGameBtn.w - ngSurf->w) / 2,
                             popup.newGameBtn.y + (popup.newGameBtn.h - ngSurf->h) / 2,
                             ngSurf->w, ngSurf->h};
                SDL_RenderCopy(popup.renderer, ngTex, nullptr, &dst);
                SDL_DestroyTexture(ngTex);
            }
            SDL_FreeSurface(ngSurf);
        }

        if (exSurf) {
            SDL_Texture *exTex = SDL_CreateTextureFromSurface(popup.renderer, exSurf);
            if (exTex) {
                SDL_Rect dst{popup.exitBtn.x + (popup.exitBtn.w - exSurf->w) / 2,
                             popup.exitBtn.y + (popup.exitBtn.h - exSurf->h) / 2,
                             exSurf->w, exSurf->h};
                SDL_RenderCopy(popup.renderer, exTex, nullptr, &dst);
                SDL_DestroyTexture(exTex);
            }
            SDL_FreeSurface(exSurf);
        }
    }

    SDL_RenderPresent(popup.renderer);
}

/**
 * @brief Gets the window ID of the popup window.
 * @return The SDL window ID, or 0 if popup is not active
 */
Uint32 Renderer::getPopupWindowID() const {
    if (!popup.active || !popup.window) return 0;
    return SDL_GetWindowID(popup.window);
}

/**
 * @brief Checks if a click is within the "New Game" button bounds.
 * @param x Mouse X coordinate in popup window
 * @param y Mouse Y coordinate in popup window
 * @return true if click is within button bounds, false otherwise
 */
bool Renderer::isNewGameButton(int x, int y) const {
    return x >= popup.newGameBtn.x && x <= popup.newGameBtn.x + popup.newGameBtn.w &&
           y >= popup.newGameBtn.y && y <= popup.newGameBtn.y + popup.newGameBtn.h;
}

/**
 * @brief Checks if a click is within the "Exit" button bounds.
 * @param x Mouse X coordinate in popup window
 * @param y Mouse Y coordinate in popup window
 * @return true if click is within button bounds, false otherwise
 */
bool Renderer::isExitButton(int x, int y) const {
    return x >= popup.exitBtn.x && x <= popup.exitBtn.x + popup.exitBtn.w &&
           y >= popup.exitBtn.y && y <= popup.exitBtn.y + popup.exitBtn.h;
}

