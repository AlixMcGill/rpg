#include "game.h"
#include "tilemap.h"

void Game::init() {
    std::cout << "Game class init" << std::endl;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    InitAudioDevice();
    m_tilemap.loadTexture("assets/OverworldHandTileset.png");
    m_tilemap.loadCSV("assets/maps/testMap/rpgmaptest_Tile Layer 1.csv");
    m_tilemap.loadCSVAssetLayer("assets/maps/testMap/rpgmaptest_asset_Layer.csv");
    m_tilemap.loadCSVCollisionLayer("assets/maps/testMap/rpgmaptest_coll_Layer.csv");
    m_tilemap.initCamera();

    player.init();
    m_tilemap.updateCameraTarget(player.xPos, player.yPos);

    m_enemyController.init("Test_Map");
    //skely.init(20, 10, "assets/enemy/Skeleton.png");
}

void Game::update(float deltaTime) {
    //std::cout << "Game class update" << std::endl;
    player.update(deltaTime, m_tilemap.worldCollisionLayer, m_enemyController.enemies);
    m_tilemap.updateCameraTarget(player.xPos, player.yPos);
    m_tilemap.cameraZoom();
    m_enemyController.update(deltaTime, player.xPos, player.yPos, player, m_tilemap.worldCollisionLayer);
    //skely.update(deltaTime, player.xPos, player.yPos, m_tilemap.worldCollisionLayer);
}

void Game::draw() {
    //std::cout << "Game class draw" << std::endl;
    BeginMode2D(m_tilemap.camera);
    m_tilemap.renderMap();
    m_enemyController.drawBehindPlayer();
    player.draw();
    m_enemyController.drawFrontPlayer();
    //skely.draw();
    EndMode2D();

    dev.devstats(m_tilemap.camera, player);
}

void Game::destroy() {
    std::cout << "Game class destroy" << std::endl;
    CloseAudioDevice();
    m_tilemap.destroyTextures();
    player.destroy();
    m_enemyController.destroy();
    //skely.destroy();
}
