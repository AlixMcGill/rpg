#pragma once
#include "../utils/project.h"
#include <raylib.h>
#include <vector>

#define MAX_TEXTURES 1

class Tilemap {
public:
    virtual ~Tilemap();

    bool debug = false;

    virtual void load(float playerXPos, float PlayerYPos) = 0;
    virtual void unload() {unloadTilemap();}
    virtual void update(float deltaTime, float playerXPos, float playerYPos) {}
    virtual void draw() {renderMap();}

    Camera2D& getCamera() {return camera;}
    const Camera2D& getCamera() const {return camera;}
    std::vector<std::vector<sTile>>& getWorldCollisionLayer() {return worldCollisionLayer;}
    const std::vector<std::vector<sTile>>& getWorldCollisionLayer() const {return worldCollisionLayer;}

protected:
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
    void unloadTilemap();
private:
    enum m_textrueAsset {
        TEXTURE_TILEMAP = 0
    };

    void m_loadCSV(const std::string& filename, std::vector<std::vector<sTile>>& loadPath);

    void m_renderDebugColl(int startX, int startY, int endX, int endY);

};
