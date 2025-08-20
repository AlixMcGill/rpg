#include "../utils/project.h"
#include <raylib.h>

#define MAX_TEXTURES 1

class Tilemap {
public:
    struct sTile {
        int x;
        int y;
    };
    Texture2D textures[MAX_TEXTURES];
    sTile world[WORLD_WIDTH][WORLD_HEIGHT];
    Camera2D camera;

    void loadTexture(const char* imgPath);
    void destroyTextures();
    void initMap();
    void renderMap();
    void initCamera();
    void cameraZoom();
private:
    enum m_textrueAsset {
        TEXTURE_TILEMAP = 0
    };

};
