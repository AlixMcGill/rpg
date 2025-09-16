#include "./utils/project.h"
#include "./game/game.h"
#include <raylib.h>

int main() {
    InitWindow(windowWidth, windowHeight, "RPG Game");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    Game game;

    game.init();

    Shader crtShader = LoadShader("./src/shader/crt/crt.vs", "./src/shader/crt/crt.fs");

    int timeLoc      = GetShaderLocation(crtShader,"time");
    int curveLoc     = GetShaderLocation(crtShader,"curvature");
    int scanLoc      = GetShaderLocation(crtShader,"scanline");
    int pixelLoc     = GetShaderLocation(crtShader,"pixelSize");
    int glowLoc      = GetShaderLocation(crtShader,"glow");
    int vignetteLoc  = GetShaderLocation(crtShader,"vignette");
    int maskWidthLoc = GetShaderLocation(crtShader,"maskWidth");
    int maskGapLoc   = GetShaderLocation(crtShader,"maskGap");
    int maskIntLoc   = GetShaderLocation(crtShader,"maskIntensity");

    float curvature      = 0.015f;
    float scanline       = 0.001f;
    float pixelSize      = 800.0f;
    float glow           = 0.05f;
    float vignette       = 0.5f;
    float maskWidth      = 1.0/164.0; // 1/3 of pixel for RGB stripes
    float maskGap        = 0.02;
    float maskIntensity  = 0.008f;

    RenderTexture2D target = LoadRenderTexture(windowWidth, windowHeight);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        float t = GetTime();

        SetShaderValue(crtShader,timeLoc,&t,SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader,curveLoc,&curvature,SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader,scanLoc,&scanline,SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader,pixelLoc,&pixelSize,SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader,glowLoc,&glow,SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader,vignetteLoc,&vignette,SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader,maskWidthLoc,&maskWidth,SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader,maskGapLoc,&maskGap,SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader,maskIntLoc,&maskIntensity,SHADER_UNIFORM_FLOAT);

        game.update(dt);

        // --- Draw game into render texture ---
        BeginTextureMode(target);
            ClearBackground(BLACK);
            game.draw();
        EndTextureMode();

        // --- Draw to screen with shader ---
        BeginDrawing();
            ClearBackground(BLACK);

            BeginShaderMode(crtShader);
                // Important: flip vertically (RenderTexture is upside-down in raylib)
                Rectangle src = { 0, 0, (float)target.texture.width, -(float)target.texture.height };
                Rectangle dst = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
                DrawTexturePro(target.texture, src, dst, {0, 0}, 0.0f, WHITE);
            EndShaderMode();

            DrawFPS(10, 10); // UI drawn normally
        EndDrawing();
    }


    UnloadShader(crtShader);
    game.destroy();
    CloseWindow();
    return 0;
}
