#include "./utils/project.h"
#include "./game/game.h"
#include <raylib.h>

int main() {
    InitWindow(windowWidth, windowHeight, "RPG Game");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    Game game;

    game.init();

    while (!WindowShouldClose()) { // Game loop
        float deltaTime = GetFrameTime();

        game.update(deltaTime);

        BeginDrawing();
        // Game drawing goes here
        ClearBackground(BLACK);
        game.draw();
        DrawFPS(1190, 10);

       /* float scaleX = (float)GetScreenWidth() / windowWidth;
        float scaleY = (float)GetScreenHeight() / windowHeight;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;

        int drawWidth = (int)(windowWidth * scale);
        int drawHeight = (int)(windowHeight * scale);

        int offsetX = (GetScreenWidth() - drawWidth) / 2;
        int offsetY = (GetScreenHeight() - drawHeight) / 2;

        Rectangle source = {0, 0, (float)target.texture.width, (float)-target.texture.height};
        Rectangle dest = {(float)offsetX, (float)offsetY, (float)drawWidth, (float)drawHeight};

        DrawTexturePro(target.texture, source, dest, (Vector2){0,0}, 0.0f, WHITE);
*/
        EndDrawing();
    }

    game.destroy();
    CloseWindow();
    return 0;
}
