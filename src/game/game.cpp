#include "game.h"

void Game::init() {
    std::cout << "Game class init" << std::endl;
    InitAudioDevice();
    m_tilemap.loadTexture("assets/Dungeon_Tileset-2.png");
    m_tilemap.loadCSV("assets/maps/rpgMainMap.csv");
    m_tilemap.initCamera();

    player.init();
    m_tilemap.updateCameraTarget(player.xPos, player.yPos);
}

void Game::update(float deltaTime) {
    std::cout << "Game class update" << std::endl;
    player.update(deltaTime);
    m_tilemap.updateCameraTarget(player.xPos, player.yPos);
    m_tilemap.cameraZoom();
}

void Game::draw() {
    std::cout << "Game class draw" << std::endl;
    BeginMode2D(m_tilemap.camera);
    m_tilemap.renderMap();
    player.draw();

    EndMode2D();

    dev.devstats(m_tilemap.camera, player);
}

void Game::destroy() {
    std::cout << "Game class destroy" << std::endl;
    CloseAudioDevice();
    m_tilemap.destroyTextures();
    player.destroy();
}
