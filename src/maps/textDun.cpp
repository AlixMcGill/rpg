#include "testDun.h"

void TestDun::load(float playerXPos, float playerYPos) {
    loadTexture("assets/OverworldHandTileset.png");
    loadCSV("assets/maps/testdungeonMap/dungeonTest_Tile Layer 1.csv");
    loadCSVAssetLayer("assets/maps/testdungeonMap/dungeonTest_Asset_Layer.csv");
    loadCSVCollisionLayer("assets/maps/testdungeonMap/dungeonTest_Coll_Layer.csv");
    initCamera();
    updateCameraTarget(playerXPos, playerYPos);
}

void TestDun::update(float deltaTime, float playerXPos, float playerYPos) {
    updateCameraTarget(playerXPos, playerYPos);
    cameraZoom();
}

void TestDun::draw() {
    Tilemap::draw();
}
