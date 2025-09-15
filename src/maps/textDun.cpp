#include "testDun.h"

void TestDun::load(Player* p) {
    // map inits
    loadTexture("assets/OverworldHandTileset.png");
    loadCSV("assets/maps/testdungeonMap/dungeonTest_Tile Layer 1.csv");
    loadCSVAssetLayer("assets/maps/testdungeonMap/dungeonTest_Asset_Layer.csv");
    loadCSVCollisionLayer("assets/maps/testdungeonMap/dungeonTest_Coll_Layer.csv");
    initCamera();
    updateCameraTarget(p->xPos, p->yPos);

    // object inits
    objManager.init(p, &getTilemapTextrue(), &getWorldCollisionLayer());
}

void TestDun::update(float deltaTime, float playerXPos, float playerYPos) {
    updateCameraTarget(playerXPos, playerYPos);
    cameraZoom();
}

void TestDun::draw() {
    Tilemap::draw();
}
