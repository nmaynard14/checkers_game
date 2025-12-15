#include "Renderer.h"
#include "rlgl.h"

#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Constructs a new Renderer and initializes Raylib.
 * Creates the main game window and sets up 3D camera.
 * @return true if initialization succeeded, false otherwise
 */
bool Renderer::initialize() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "8x8 Checkers - Teal vs Purple (Raylib 3D)");
    
    if (!IsWindowReady()) {
        std::cerr << "Failed to initialize Raylib window\n";
        return false;
    }

    SetTargetFPS(60);

    // Initialize camera
    camera.position = (Vector3){0.0f, 8.0f, 8.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Load default font
    font = GetFontDefault();

    return true;
}

/**
 * @brief Destructor that cleans up all Raylib resources.
 */
Renderer::~Renderer() {
    CloseWindow();
}

/**
 * @brief Updates camera position based on active player.
 * @param isTealActive Whether Teal is the active player
 */
void Renderer::updateCamera(bool isTealActive) {
    float angleRad = cameraAngle * M_PI / 180.0f;
    float cameraY = cameraDistance * std::sin(angleRad);
    float cameraZDist = cameraDistance * std::cos(angleRad);
    
    // Position camera behind active player
    // Teal (bottom): camera behind = negative Z (looking toward positive Z)
    // Purple (top): camera behind = positive Z (looking toward negative Z)
    float cameraZ = isTealActive ? -cameraZDist : cameraZDist;
    
    camera.position = (Vector3){0.0f, cameraY, cameraZ};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
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
    
    Color squareColor;
    if (isSelected) {
        squareColor = YELLOW;
    } else if (isDark) {
        squareColor = (Color){118, 150, 86, 255};  // Dark green
    } else {
        squareColor = (Color){238, 238, 210, 255}; // Light beige
    }
    
    // Draw board square as a thin box
    DrawCube((Vector3){x, 0.0f, z}, CELL_SIZE, 0.1f, CELL_SIZE, squareColor);
    
    // Draw selection highlight
    if (isSelected) {
        DrawCubeWires((Vector3){x, 0.05f, z}, CELL_SIZE, 0.1f, CELL_SIZE, YELLOW);
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
    Color pieceColor = isTeal ? (Color){0, 128, 128, 255} : (Color){128, 0, 128, 255};
    
    // Draw cylinder (using a cube as approximation, or we could use a model)
    // For simplicity, using a cube with rounded appearance
    float yPos = PIECE_HEIGHT / 2.0f;
    DrawCylinder((Vector3){x, yPos, z}, PIECE_RADIUS, PIECE_RADIUS, PIECE_HEIGHT, 32, pieceColor);
    DrawCylinderWires((Vector3){x, yPos, z}, PIECE_RADIUS, PIECE_RADIUS, PIECE_HEIGHT, 32, BLACK);
    
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
    float crownY = PIECE_HEIGHT + 0.1f;
    float crownWidth = PIECE_RADIUS * 1.4f;
    float crownHeight = PIECE_HEIGHT * 0.6f;
    
    Color gold = (Color){255, 215, 0, 255};
    
    // Crown base
    DrawCube((Vector3){x, crownY, z}, crownWidth, 0.1f, crownWidth * 0.6f, gold);
    
    // Three crown points
    for (int i = 0; i < 3; ++i) {
        float offset = (i - 1) * crownWidth * 0.5f;
        float pointWidth = crownWidth * 0.2f;
        DrawCube((Vector3){x + offset, crownY + crownHeight / 2.0f, z}, 
                 pointWidth, crownHeight, pointWidth, gold);
    }
}

/**
 * @brief Renders the 2D UI overlay (sidebar with piece counts).
 * @param tealCount Number of teal pieces
 * @param purpleCount Number of purple pieces
 */
void Renderer::renderUIOverlay(int tealCount, int purpleCount) {
    int sidebarX = WINDOW_WIDTH - SIDEBAR_WIDTH;
    
    // Sidebar background
    DrawRectangle(sidebarX, 0, SIDEBAR_WIDTH, WINDOW_HEIGHT, (Color){40, 40, 40, 255});
    
    // Teal indicator
    DrawRectangle(sidebarX + 20, 40, 30, 30, (Color){0, 128, 128, 255});
    DrawText(TextFormat("Teal: %d", tealCount), sidebarX + 20, 10, 20, WHITE);
    
    // Purple indicator
    DrawRectangle(sidebarX + 20, 120, 30, 30, (Color){128, 0, 128, 255});
    DrawText(TextFormat("Purple: %d", purpleCount), sidebarX + 20, 90, 20, WHITE);
}

/**
 * @brief Renders the game board, pieces, and UI elements in 3D using Raylib.
 * Draws a 3D checkered board, 3D cylindrical pieces with crown models for kings,
 * the sidebar with piece counts, and highlights the currently selected piece.
 * @param state The current game state to render
 */
void Renderer::renderGame(const GameState &state) {
    beginDrawing();
    
    ClearBackground((Color){51, 51, 64, 255});  // Dark blue-grey background
    
    // Set viewport for 3D rendering (left portion, excluding sidebar)
    int boardWidth = WINDOW_WIDTH - SIDEBAR_WIDTH;
    rlViewport(0, 0, boardWidth, WINDOW_HEIGHT);
    
    // Determine active player and update camera
    bool isTealActive = (state.currentPlayer == TealMan || state.currentPlayer == TealKing);
    updateCamera(isTealActive);
    
    // Create a temporary camera with adjusted aspect ratio for the viewport
    Camera3D viewportCamera = camera;
    // Note: Raylib's BeginMode3D automatically uses the current viewport for aspect ratio
    BeginMode3D(viewportCamera);
    
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
    
    EndMode3D();
    
    // Reset viewport to full window for 2D rendering
    rlViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Render UI overlay
    renderUIOverlay(tealCount, purpleCount);
    
    endDrawing();
}

/**
 * @brief Checks if the window should close.
 * @return true if window close was requested, false otherwise
 */
bool Renderer::shouldClose() const {
    return WindowShouldClose();
}

/**
 * @brief Begins drawing frame (call before rendering).
 */
void Renderer::beginDrawing() {
    BeginDrawing();
}

/**
 * @brief Ends drawing frame and swaps buffers (call after rendering).
 */
void Renderer::endDrawing() {
    EndDrawing();
}

/**
 * @brief Converts screen coordinates to 3D board coordinates.
 * Used for mouse picking in the 3D scene.
 * @param mouseX Screen X coordinate
 * @param mouseY Screen Y coordinate
 * @param outRow Output parameter for board row (-1 if invalid)
 * @param outCol Output parameter for board column (-1 if invalid)
 */
void Renderer::screenToBoard(int mouseX, int mouseY, int &outRow, int &outCol, bool isTealActive) const {
    outRow = -1;
    outCol = -1;
    
    // Ignore clicks in sidebar
    int boardWidth = WINDOW_WIDTH - SIDEBAR_WIDTH;
    if (mouseX >= boardWidth) {
        return;
    }
    
    // CRITICAL: Update camera to match the rendering state
    // This ensures the ray calculation uses the same camera position as rendering
    Camera3D viewportCamera = camera;
    float angleRad = cameraAngle * M_PI / 180.0f;
    float cameraY = cameraDistance * std::sin(angleRad);
    float cameraZDist = cameraDistance * std::cos(angleRad);
    // Teal (bottom): camera behind = negative Z (looking toward positive Z)
    // Purple (top): camera behind = positive Z (looking toward negative Z)
    float cameraZ = isTealActive ? -cameraZDist : cameraZDist;
    viewportCamera.position = (Vector3){0.0f, cameraY, cameraZ};
    viewportCamera.target = (Vector3){0.0f, 0.0f, 0.0f};
    
    // Set viewport to match 3D rendering viewport
    rlViewport(0, 0, boardWidth, WINDOW_HEIGHT);
    
    // Set up 3D projection to match rendering
    // This ensures GetMouseRay uses the same projection matrix
    BeginMode3D(viewportCamera);
    
    // ROOT CAUSE ANALYSIS:
    // GetMouseRay internally uses GetScreenWidth() and GetScreenHeight() which
    // return the WINDOW dimensions (800x800), not the viewport dimensions (600x800).
    // This causes the aspect ratio calculation to be wrong, resulting in
    // the ray being calculated incorrectly.
    //
    // SOLUTION: We need to scale the mouse coordinates to account for the
    // difference between window size and viewport size, OR we need to ensure
    // GetMouseRay uses the viewport dimensions.
    //
    // Since we can't modify GetMouseRay, we'll adjust the mouse coordinates
    // to compensate. The viewport is boardWidth wide, but GetMouseRay thinks
    // the screen is WINDOW_WIDTH wide. So we need to scale mouseX.
    float scaleX = (float)WINDOW_WIDTH / (float)boardWidth;
    float adjustedMouseX = mouseX * scaleX;
    
    // GetMouseRay calculates the ray from mouse coordinates
    // The adjusted coordinates account for the viewport/window size mismatch
    Ray ray = GetMouseRay((Vector2){adjustedMouseX, (float)mouseY}, viewportCamera);
    
    EndMode3D();
    
    // Restore full window viewport
    rlViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Check intersection with board plane (y = 0)
    if (std::abs(ray.direction.y) > 0.001f) {
        float t = -ray.position.y / ray.direction.y;
        if (t > 0) {
            Vector3 hitPoint = {
                ray.position.x + ray.direction.x * t,
                ray.position.y + ray.direction.y * t,
                ray.position.z + ray.direction.z * t
            };
            
            // Convert 3D hit point to board coordinates
            // Board squares are centered at: x = (col - 3.5) * CELL_SIZE
            // Inverse: col = (x / CELL_SIZE) + 3.5
            float colFloat = (hitPoint.x / CELL_SIZE) + (BOARD_SIZE / 2.0f) - 0.5f;
            float rowFloat = (hitPoint.z / CELL_SIZE) + (BOARD_SIZE / 2.0f) - 0.5f;
            
            // Round to nearest integer
            int col = static_cast<int>(std::round(colFloat));
            int row = static_cast<int>(std::round(rowFloat));
            
            if (inBounds(row, col)) {
                outRow = row;
                outCol = col;
            }
        }
    }
}

/**
 * @brief Gets the mouse position.
 * @return Vector2 with mouse x and y coordinates
 */
Vector2 Renderer::getMousePosition() const {
    return GetMousePosition();
}

/**
 * @brief Checks if the left mouse button was pressed this frame.
 * @return true if left mouse button was pressed, false otherwise
 */
bool Renderer::isMouseButtonPressed() const {
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

/**
 * @brief Renders the difficulty selection menu.
 * @param selectedDifficulty The currently selected difficulty (0-2 for Easy, Medium, Hard)
 * @return The selected difficulty index, or -1 if none selected
 */
int Renderer::renderDifficultyMenu(int selectedDifficulty) {
    beginDrawing();
    
    ClearBackground((Color){51, 51, 64, 255});  // Dark blue-grey background
    
    // Draw title
    const char* title = "Select Difficulty";
    int titleWidth = MeasureText(title, 50);
    DrawText(title, (WINDOW_WIDTH - titleWidth) / 2, 150, 50, YELLOW);
    
    // Difficulty options
    const char* difficulties[] = {"Easy", "Medium", "Hard"};
    const char* descriptions[] = {
        "AI makes best moves 30% of the time",
        "AI makes best moves 60% of the time",
        "AI makes best moves 100% of the time"
    };
    
    int buttonWidth = 200;
    int buttonHeight = 80;
    int buttonSpacing = 30;
    int startY = 280;
    int startX = (WINDOW_WIDTH - buttonWidth) / 2;
    
    int selectedIndex = -1;
    Vector2 mousePos = GetMousePosition();
    
    for (int i = 0; i < 3; ++i) {
        int buttonY = startY + i * (buttonHeight + buttonSpacing);
        bool isHovered = (mousePos.x >= startX && mousePos.x <= startX + buttonWidth &&
                         mousePos.y >= buttonY && mousePos.y <= buttonY + buttonHeight);
        bool isSelected = (selectedDifficulty == i);
        
        // Button background
        Color buttonColor = isSelected ? (Color){100, 150, 200, 255} : 
                           isHovered ? (Color){80, 80, 100, 255} : 
                           (Color){60, 60, 80, 255};
        DrawRectangle(startX, buttonY, buttonWidth, buttonHeight, buttonColor);
        DrawRectangleLines(startX, buttonY, buttonWidth, buttonHeight, WHITE);
        
        // Difficulty name
        int textWidth = MeasureText(difficulties[i], 30);
        DrawText(difficulties[i], startX + (buttonWidth - textWidth) / 2, 
                 buttonY + 15, 30, WHITE);
        
        // Description
        int descWidth = MeasureText(descriptions[i], 18);
        DrawText(descriptions[i], startX + (buttonWidth - descWidth) / 2, 
                 buttonY + 50, 18, (Color){200, 200, 200, 255});
        
        // Check if clicked
        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            selectedIndex = i;
        }
    }
    
    endDrawing();
    return selectedIndex;
}
