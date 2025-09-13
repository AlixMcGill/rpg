#include "./utils/project.h"
#include "./game/game.h"
#include <raylib.h>

int main() {
    InitWindow(windowWidth, windowHeight, "RPG Game");
    SetTargetFPS(60);

    Game game;

    game.init();

    while (!WindowShouldClose()) { // Game loop
        float deltaTime = GetFrameTime();

        game.update(deltaTime);

        BeginDrawing();
        ClearBackground(BLACK);
        game.draw();
        DrawFPS(1190, 10);
        EndDrawing();
    }
    game.destroy();
    CloseWindow();
    return 0;
}
