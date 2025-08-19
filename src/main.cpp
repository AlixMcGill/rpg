#include "./utils/project.h"
#include "./game/game.h"
#include <raylib.h>

int main() {
    InitWindow(windowWidth, windowHeight, "RPG Game");
    SetTargetFPS(240);

    Game game;

    game.init();

    while (!WindowShouldClose()) { // Game loop
        float deltaTime = GetFrameTime();

        game.update(deltaTime);

        BeginDrawing();
        ClearBackground(GRAY);
        game.draw();
        EndDrawing();
    }
    game.destroy();
    CloseWindow();
    return 0;
}
