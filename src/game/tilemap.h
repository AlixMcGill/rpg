#pragma once
#include "../utils/project.h"
#include <raylib.h>

#define MAX_TEXTURES 1

class Tilemap {
public:
    struct sTile {
        int x;
        int y;
        int id;
    };
    Texture2D textures[MAX_TEXTURES];
    std::vector<std::vector<sTile>> world;
    std::vector<std::vector<sTile>> worldCollisionLayer;
    std::vector<std::vector<sTile>> worldAssetLayer;
    int mapWidth = 0;
    int mapHeight = 0;
    Camera2D camera;
    Vector2 cameraTarget;

    void loadTexture(const char* imgPath);
    void destroyTextures();
    void loadCSV(const std::string& filename);
    void loadCSVAssetLayer(const std::string& filename);
    void loadCSVCollisionLayer(const std::string& filename);
    void renderMap();
    void initCamera();
    void cameraZoom();
    void updateCameraTarget(float x, float y);
private:
    enum m_textrueAsset {
        TEXTURE_TILEMAP = 0
    };

    void m_loadCSV(const std::string& filename, std::vector<std::vector<sTile>>& loadPath);

};
