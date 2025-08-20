#include "tilemap.h"
#include <raylib.h>

void Tilemap::loadTexture(const char* imgPath) {
    std::cout << "Loading Texture" << std::endl;
    Image image = LoadImage(imgPath);
    textures[TEXTURE_TILEMAP] = LoadTextureFromImage(image);
    UnloadImage(image);
}

void Tilemap::destroyTextures() {
    for (int i = 0; i < MAX_TEXTURES; i++) {
        UnloadTexture(textures[i]);
    }
}

void Tilemap::initMap() {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        for (int j = 0; j < WORLD_HEIGHT; j++) {
            world[i][j] = (sTile) {
                .x = i,
                .y = j
            };
        }
    }
}

void Tilemap::renderMap() {
    sTile tile;

    int texture_index_x = 0;
    int texture_index_y = 0;

    for (int i = 0; i < WORLD_WIDTH; i++) {
        for (int j = 0; j < WORLD_HEIGHT; j++) {
            tile = world[i][j];
            texture_index_x = i * TILE_WIDTH;
            texture_index_y = j * TILE_HEIGHT;

            Rectangle source = { (float)texture_index_x, (float)texture_index_y, (float)TILE_WIDTH, (float)TILE_HEIGHT};
            Rectangle dest = {(float)(tile.x * TILE_WIDTH), (float)(tile.y * TILE_HEIGHT), (float)TILE_WIDTH, (float)TILE_HEIGHT};
            Vector2 origin = {0,0};
            DrawTexturePro(textures[TEXTURE_TILEMAP], source, dest, origin, 0.0f, WHITE);
        }
    }
}

void Tilemap::initCamera() {
    camera = { 0 };
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ (float)windowWidth / 2, (float)windowHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
}

void Tilemap::cameraZoom() {
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        const float zoomIncrement = 0.125f;
        camera.zoom += (wheel * zoomIncrement);

        if (camera.zoom < 0.25f) camera.zoom = 0.25f;
        if (camera.zoom > 4.0f) camera.zoom = 4.0f;
    }

    camera.target = cameraTarget;
}

void Tilemap::updateCameraTarget(float x, float y) {
    camera.target = (Vector2){ x, y };
    cameraTarget.x = x;
    cameraTarget.y = y;
}
