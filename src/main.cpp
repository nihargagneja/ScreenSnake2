/*******************************************************************************************
*
*   SCREEN SNAKE USING RAYLIB
*   Copyright (c) 2024 Nihar Gagneja
*
********************************************************************************************/

#include <raylib.h>
#include "Direction.h"
#include "Board.h"

int main() {
    // GRAPHICS CONSTANTS
    constexpr Vector2i SCREEN_DIMENSIONS = {800, 800};
    constexpr Vector2i TILES_PER_DIMENSION = {20, 20};
    constexpr int TARGET_FPS = 6;

    InitWindow(SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, "Just so you know, I'm straighk.");
    SetTargetFPS(TARGET_FPS);

    // initialize game
    auto board = Board {SCREEN_DIMENSIONS, TILES_PER_DIMENSION};
    board.initialize();

    auto inputDirection = Direction::RIGHT;
    while(!WindowShouldClose()) {
        // update the game
        board.processPlayerInput(inputDirection);

        // UPDATE THE GAME
        board.update();

        // DRAW THE GAME
        BeginDrawing();
        ClearBackground(GRAY);
        board.draw();
        EndDrawing();

        // CAPTURE USER INPUT
        if     (IsKeyDown(KEY_UP))   inputDirection = Direction::UP;
        else if(IsKeyDown(KEY_DOWN)) inputDirection = Direction::DOWN;
        else if(IsKeyDown(KEY_LEFT)) inputDirection = Direction::LEFT;
        else if(IsKeyDown(KEY_RIGHT))inputDirection = Direction::RIGHT;
    }

    return 0;
}

