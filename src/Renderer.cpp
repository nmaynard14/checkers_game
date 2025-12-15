#include "Renderer.h"

#include <cmath>
#include <iostream>
#include <string>
#include <GL/glu.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Constructs a new Renderer and initializes SDL video, OpenGL, and TTF subsystems.
 * Creates the main game window with OpenGL context and loads the font.
 * @return true if initialization succeeded, false otherwise
 */
bool Renderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Set OpenGL attributes before creating window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(
        "8x8 Checkers - Teal vs Purple (SDL2 + OpenGL 3D)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Enable VSync
    SDL_GL_SetSwapInterval(1);

    setupOpenGL();

    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << "\n";
    }

    // Use a common system font path; adjust if needed for your system.
    font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << "\n";
    }

    return true;
}

/**
 * @brief Destructor that cleans up all SDL and OpenGL resources (window, context, font).
 */
Renderer::~Renderer() {
    closePopup();

    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();

    if (glContext) {
        SDL_GL_DeleteContext(glContext);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

/**
 * @brief Sets up OpenGL rendering state (lighting, depth testing, etc.)
 */
void Renderer::setupOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Set up lighting
    GLfloat lightPos[] = {5.0f, 8.0f, 5.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
    glShadeModel(GL_SMOOTH);
}

/**
 * @brief Renders a single board square in 3D.
 * @param row Board row (0-7)
 * @param col Board column (0-7)
 * @param isDark Whether this is a dark square
 * @param isSelected Whether this square is currently selected
 */
void Renderer::renderBoardSquare(int row, int col, bool isDark, bool isSelected) {
    float x = (col - BOARD_SIZE / 2.0f + 0.5f) * CELL_SIZE;
    float z = (row - BOARD_SIZE / 2.0f + 0.5f) * CELL_SIZE;
    float halfCell = CELL_SIZE * 0.5f;

    // Set color based on square type and selection
    if (isSelected) {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow for selection
    } else if (isDark) {
        glColor3f(0.46f, 0.59f, 0.34f); // Dark green
    } else {
        glColor3f(0.93f, 0.93f, 0.82f); // Light beige
    }

    // Draw the top face of the square
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x - halfCell, BOARD_HEIGHT, z - halfCell);
    glVertex3f(x + halfCell, BOARD_HEIGHT, z - halfCell);
    glVertex3f(x + halfCell, BOARD_HEIGHT, z + halfCell);
    glVertex3f(x - halfCell, BOARD_HEIGHT, z + halfCell);
    glEnd();

    // Draw edges for depth
    if (isSelected) {
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(x - halfCell, BOARD_HEIGHT + 0.01f, z - halfCell);
        glVertex3f(x + halfCell, BOARD_HEIGHT + 0.01f, z - halfCell);
        glVertex3f(x + halfCell, BOARD_HEIGHT + 0.01f, z + halfCell);
        glVertex3f(x - halfCell, BOARD_HEIGHT + 0.01f, z + halfCell);
        glEnd();
        glLineWidth(1.0f);
    }
}

/**
 * @brief Renders a 3D cylindrical piece.
 * @param x X position in 3D space
 * @param z Z position in 3D space
 * @param isTeal Whether this is a teal piece (false = purple)
 * @param isKing Whether this is a king piece
 */
void Renderer::renderPiece(float x, float z, bool isTeal, bool isKing) {
    const int segments = 32;
    const float yBase = BOARD_HEIGHT + 0.01f;
    const float yTop = yBase + PIECE_HEIGHT;

    // Set piece color
    if (isTeal) {
        glColor3f(0.0f, 0.5f, 0.5f); // Teal
    } else {
        glColor3f(0.5f, 0.0f, 0.5f); // Purple
    }

    // Draw cylinder body
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float nx = std::cos(angle);
        float nz = std::sin(angle);
        float px = x + nx * PIECE_RADIUS;
        float pz = z + nz * PIECE_RADIUS;

        glNormal3f(nx, 0.0f, nz);
        glVertex3f(px, yBase, pz);
        glVertex3f(px, yTop, pz);
    }
    glEnd();

    // Draw top cap
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x, yTop, z);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float px = x + std::cos(angle) * PIECE_RADIUS;
        float pz = z + std::sin(angle) * PIECE_RADIUS;
        glVertex3f(px, yTop, pz);
    }
    glEnd();

    // Draw bottom cap
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x, yBase, z);
    for (int i = segments; i >= 0; --i) {
        float angle = 2.0f * M_PI * i / segments;
        float px = x + std::cos(angle) * PIECE_RADIUS;
        float pz = z + std::sin(angle) * PIECE_RADIUS;
        glVertex3f(px, yBase, pz);
    }
    glEnd();

    // Draw crown if king
    if (isKing) {
        renderCrown(x, z);
    }
}

/**
 * @brief Renders a 3D crown on top of a king piece.
 * @param x X position in 3D space
 * @param z Z position in 3D space
 */
void Renderer::renderCrown(float x, float z) {
    const float crownHeight = PIECE_HEIGHT * 0.6f;
    const float crownY = BOARD_HEIGHT + PIECE_HEIGHT + 0.01f;
    const float crownBaseY = crownY + crownHeight * 0.4f;
    const float crownWidth = PIECE_RADIUS * 1.4f;

    glColor3f(1.0f, 0.84f, 0.0f); // Gold

    // Crown base
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x - crownWidth, crownBaseY, z - crownWidth * 0.3f);
    glVertex3f(x + crownWidth, crownBaseY, z - crownWidth * 0.3f);
    glVertex3f(x + crownWidth, crownBaseY, z + crownWidth * 0.3f);
    glVertex3f(x - crownWidth, crownBaseY, z + crownWidth * 0.3f);
    glEnd();

    // Three crown points
    for (int i = 0; i < 3; ++i) {
        float offset = (i - 1) * crownWidth * 0.5f;
        float pointWidth = crownWidth * 0.2f;

        glBegin(GL_TRIANGLES);
        // Front face
        glNormal3f(0.0f, 0.5f, 1.0f);
        glVertex3f(x + offset - pointWidth, crownBaseY, z + crownWidth * 0.3f);
        glVertex3f(x + offset + pointWidth, crownBaseY, z + crownWidth * 0.3f);
        glVertex3f(x + offset, crownY + crownHeight, z + crownWidth * 0.3f);
        // Back face
        glNormal3f(0.0f, 0.5f, -1.0f);
        glVertex3f(x + offset - pointWidth, crownBaseY, z - crownWidth * 0.3f);
        glVertex3f(x + offset, crownY + crownHeight, z - crownWidth * 0.3f);
        glVertex3f(x + offset + pointWidth, crownBaseY, z - crownWidth * 0.3f);
        glEnd();
    }
}

/**
 * @brief Renders the 2D UI overlay (sidebar with piece counts).
 * Uses immediate mode rendering over the 3D scene.
 * @param tealCount Number of teal pieces
 * @param purpleCount Number of purple pieces
 */
void Renderer::renderUIOverlay(int /*tealCount*/, int /*purpleCount*/) {
    // Switch to 2D orthographic projection for UI
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Sidebar background
    glColor3f(0.16f, 0.16f, 0.16f);
    glBegin(GL_QUADS);
    glVertex2f(WINDOW_WIDTH - SIDEBAR_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(WINDOW_WIDTH - SIDEBAR_WIDTH, WINDOW_HEIGHT);
    glEnd();

    // Color squares and text would go here using SDL_ttf
    // For now, we'll keep it simple with colored rectangles

    // Teal indicator
    glColor3f(0.0f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 20, 40);
    glVertex2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 50, 40);
    glVertex2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 50, 70);
    glVertex2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 20, 70);
    glEnd();

    // Purple indicator
    glColor3f(0.5f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 20, 120);
    glVertex2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 50, 120);
    glVertex2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 50, 150);
    glVertex2f(WINDOW_WIDTH - SIDEBAR_WIDTH + 20, 150);
    glEnd();

    // Note: Text rendering with TTF would require more complex setup
    // For now, the colored squares indicate the sides

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

/**
 * @brief Renders the game board, pieces, and UI elements in 3D using OpenGL.
 * Draws a 3D checkered board, 3D cylindrical pieces with crown models for kings,
 * the sidebar with piece counts, and highlights the currently selected piece.
 * @param state The current game state to render
 */
void Renderer::renderGame(const GameState &state) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up 3D projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up camera
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
    glTranslatef(-cameraX, -cameraY, -cameraZ);

    // Render board squares
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            bool dark = isDarkSquare(r, c);
            bool selected = (r == state.selectedRow && c == state.selectedCol);
            renderBoardSquare(r, c, dark, selected);
        }
    }

    // Render pieces
    int tealCount = 0;
    int purpleCount = 0;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            Piece pc = state.board[r][c];
            if (pc == Empty) continue;

            if (isTealPiece(pc)) ++tealCount;
            if (isPurplePiece(pc)) ++purpleCount;

            float x = (c - BOARD_SIZE / 2.0f + 0.5f) * CELL_SIZE;
            float z = (r - BOARD_SIZE / 2.0f + 0.5f) * CELL_SIZE;

            bool isTeal = isTealPiece(pc);
            bool isKing = (pc == TealKing || pc == PurpleKing);
            renderPiece(x, z, isTeal, isKing);
        }
    }

    // Render UI overlay
    renderUIOverlay(tealCount, purpleCount);

    SDL_GL_SwapWindow(window);
}

/**
 * @brief Converts screen coordinates to 3D board coordinates.
 * Used for mouse picking in the 3D scene.
 * @param mouseX Screen X coordinate
 * @param mouseY Screen Y coordinate
 * @param outRow Output parameter for board row (-1 if invalid)
 * @param outCol Output parameter for board column (-1 if invalid)
 */
void Renderer::screenToBoard(int mouseX, int mouseY, int &outRow, int &outCol) const {
    outRow = -1;
    outCol = -1;

    // Ignore clicks in sidebar
    if (mouseX >= WINDOW_WIDTH - SIDEBAR_WIDTH) {
        return;
    }

    // Simple projection: assume orthographic view of board
    // Convert screen coords to normalized device coordinates
    float ndcX = (2.0f * mouseX / WINDOW_WIDTH) - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY / WINDOW_HEIGHT);

    // Approximate board position in screen space
    // This is a simplified calculation - a full implementation would use
    // proper 3D picking with ray casting
    float boardX = ndcX * 4.0f; // Approximate scale
    float boardZ = ndcY * 4.0f;

    int col = static_cast<int>((boardX / CELL_SIZE) + BOARD_SIZE / 2.0f);
    int row = static_cast<int>((boardZ / CELL_SIZE) + BOARD_SIZE / 2.0f);

    if (inBounds(row, col)) {
        outRow = row;
        outCol = col;
    }
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
