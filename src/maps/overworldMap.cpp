#include "overworldMap.h"

void OverworldMap::load(float playerXPos, float playerYPos) {
    loadTexture("assets/OverworldHandTileset.png");
    loadCSV("assets/maps/testMap/rpgmaptest_Tile Layer 1.csv");
    loadCSVAssetLayer("assets/maps/testMap/rpgmaptest_asset_Layer.csv");
    loadCSVCollisionLayer("assets/maps/testMap/rpgmaptest_coll_Layer.csv");
    initCamera();
    updateCameraTarget(playerXPos, playerYPos);
}

void OverworldMap::update(float deltaTime, float playerXPos, float playerYPos) {
    updateCameraTarget(playerXPos, playerYPos);
    cameraZoom();
}

void OverworldMap::draw() {
    Tilemap::draw();
}
