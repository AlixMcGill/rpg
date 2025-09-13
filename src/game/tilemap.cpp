#include "tilemap.h"
#include <raylib.h>
#include <string>

Tilemap::~Tilemap() {
    unloadTilemap();
}

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

// Very important that collision map is loaded after regular map
void Tilemap::loadCSVCollisionLayer(const std::string& filename) {
    std::cout << "WorldY: " << (int)worldCollisionLayer.size() << 
        " WorldX: " << (int)worldCollisionLayer.size() << std::endl;
    m_loadCSV(filename, worldCollisionLayer);
    std::cout << "WorldY: " << (int)worldCollisionLayer.size() << 
        " WorldX: " << (int)worldCollisionLayer.size() << std::endl;
}

void Tilemap::loadCSVAssetLayer(const std::string& filename) {
    m_loadCSV(filename, worldAssetLayer);
}

void Tilemap::loadCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<int> tileIDs;

    int width = 0;
    int height = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;

        int rowWidth = 0;

        std::vector<int> row;
        while (std::getline(ss, value, ',')) {
            if (!value.empty()) {
                row.push_back(std::stoi(value));
                rowWidth++;
            }

        }

        if (width == 0) width = rowWidth;
        height++;
        tileIDs.insert(tileIDs.end(), row.begin(), row.end());

    }

    mapWidth = width;
    mapHeight = height;

    std::cout << "Loading: " << filename << std::endl;
    std::cout << mapHeight << " " << mapWidth << std::endl;

    world.resize(mapHeight, std::vector<sTile>(mapWidth));

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int index = y * mapWidth + x;
            world[y][x] = {x, y, tileIDs[index]};
        }
    }
}

void Tilemap::renderMap() {
    float camLeft = camera.target.x - GetScreenWidth() * 0.5f / camera.zoom;
    float camTop = camera.target.y - GetScreenHeight() * 0.5f / camera.zoom;
    float camRight = camLeft + GetScreenWidth() / camera.zoom;
    float camBottom = camTop + GetScreenWidth() / camera.zoom;

    int startX = std::max(0, (int)(camLeft / TILE_HEIGHT));
    int startY = std::max(0, (int)(camTop / TILE_HEIGHT));
    int endX = std::min(mapWidth, (int)(camRight / TILE_WIDTH) + 1);
    int endY = std::min(mapHeight, (int)(camBottom / TILE_HEIGHT) + 1);

    for (int y = startY; y < endY; y++) { // Main Layer
        for (int x = startX; x < endX; x++) {

            sTile& tile = world[y][x];
            if (tile.id == 0) continue;

            int tilesPerRow = textures[TEXTURE_TILEMAP].width / TILE_WIDTH;
            int tileIndex = tile.id;

            int srcX = (tileIndex % tilesPerRow) * TILE_WIDTH;
            int srcY = (tileIndex / tilesPerRow) * TILE_HEIGHT;

            Rectangle source = { (float)srcX, (float)srcY, (float)TILE_WIDTH, (float)TILE_HEIGHT};
            Rectangle dest = {(float)(tile.x * TILE_WIDTH), (float)(tile.y * TILE_HEIGHT), (float)TILE_WIDTH, (float)TILE_HEIGHT};
            Vector2 origin = {0,0};
            DrawTexturePro(textures[TEXTURE_TILEMAP], source, dest, origin, 0.0f, WHITE);
        }
    }

    for (int y = startY; y < endY; y++) { // Asset layer for below player
        for (int x = startX; x < endX; x++) {

            sTile& tile = worldAssetLayer[y][x];
            if (tile.id == 0) continue;

            int tilesPerRow = textures[TEXTURE_TILEMAP].width / TILE_WIDTH;
            int tileIndex = tile.id;

            int srcX = (tileIndex % tilesPerRow) * TILE_WIDTH;
            int srcY = (tileIndex / tilesPerRow) * TILE_HEIGHT;

            Rectangle source = { (float)srcX, (float)srcY, (float)TILE_WIDTH, (float)TILE_HEIGHT};
            Rectangle dest = {(float)(tile.x * TILE_WIDTH), (float)(tile.y * TILE_HEIGHT), (float)TILE_WIDTH, (float)TILE_HEIGHT};
            Vector2 origin = {0,0};
            DrawTexturePro(textures[TEXTURE_TILEMAP], source, dest, origin, 0.0f, WHITE);
        }
    }

    if (debug) {
        m_renderDebugColl(startX, startY, endX, endY);
    }
}

void Tilemap::initCamera() {
    camera = { 0 };
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ (float)windowWidth / 2, (float)windowHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 2.5f;
}

void Tilemap::cameraZoom() {
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        const float zoomIncrement = 0.125f;
        camera.zoom += (wheel * zoomIncrement);

        if (camera.zoom < 2.5f) camera.zoom = 2.5f;
        if (camera.zoom > 4.0f) camera.zoom = 4.0f;
    }

    camera.target = cameraTarget;
}

void Tilemap::updateCameraTarget(float x, float y) {
    camera.target = (Vector2){ x, y };
    cameraTarget.x = x;
    cameraTarget.y = y;
}

void Tilemap::unloadTilemap() {
    destroyTextures();
    world.clear();
    worldCollisionLayer.clear();
    worldAssetLayer.clear();
    mapWidth = 0;
    mapHeight = 0;
}

void Tilemap::m_renderDebugColl(int startX, int startY, int endX, int endY) {
    for (int y = startY; y < endY; y++) { // Asset layer for below player
        for (int x = startX; x < endX; x++) {

            sTile& tile = worldCollisionLayer[y][x];
            if (tile.id == -1) continue;

            int srcX = x * TILE_WIDTH;
            int srcY = y * TILE_HEIGHT;

            Rectangle source = { (float)srcX, (float)srcY, (float)TILE_WIDTH, (float)TILE_HEIGHT};
            DrawRectangleLinesEx(source, 1.0f, BLUE);
        }
    }

}

void Tilemap::m_loadCSV(const std::string& filename, std::vector<std::vector<sTile>>& loadPath) {
    std::ifstream file(filename);
    std::string line;
    std::vector<int> tileIDs;

    int width = 0;
    int height = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;

        int rowWidth = 0;

        std::vector<int> row;
        while (std::getline(ss, value, ',')) {
            if (!value.empty()) {
                row.push_back(std::stoi(value));
                rowWidth++;
            }

        }

        if (width == 0) width = rowWidth;
        height++;
        tileIDs.insert(tileIDs.end(), row.begin(), row.end());

    }

    if (mapWidth != width || mapHeight != height) {
        std::cout << "[ERROR]: The collision map w and height are invalid" << std::endl;
    }

    std::cout << "Loading: " << filename << std::endl;
    std::cout << mapHeight << " " << mapWidth << std::endl;

    loadPath.assign(mapHeight, std::vector<sTile>(mapWidth));

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int index = y * mapWidth + x;
            loadPath[y][x] = {x, y, tileIDs[index]};
        }
    }
}
