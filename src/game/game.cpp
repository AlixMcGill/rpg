#include "game.h"
#include "tilemap.h"

void Game::init() {
    std::cout << "Game class init" << std::endl;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    InitAudioDevice();
    m_mapManager.init(&player, &m_enemyController);
    player.init();

    m_enemyController.init("Test_Map");
    //skely.init(20, 10, "assets/enemy/Skeleton.png");
}

void Game::update(float deltaTime) {
    //std::cout << "Game class update" << std::endl;
    player.update(deltaTime, m_mapManager.getWorldCollisionLayer(), m_enemyController.enemies);
    m_mapManager.update(deltaTime);
    m_enemyController.update(deltaTime, player.xPos, player.yPos, player, m_mapManager.getWorldCollisionLayer());
    //skely.update(deltaTime, player.xPos, player.yPos, m_tilemap.worldCollisionLayer);
}

void Game::draw() {
    //std::cout << "Game class draw" << std::endl;
    BeginMode2D(m_mapManager.getCamera());
    m_mapManager.draw();
    m_enemyController.drawBehindPlayer();
    player.draw();
    m_enemyController.drawFrontPlayer();
    //skely.draw();
    EndMode2D();

    dev.devstats(m_mapManager.getCamera(), player);
}

void Game::destroy() {
    //std::cout << "Game class destroy" << std::endl;
    CloseAudioDevice();
    //m_tilemap.destroyTextures();
    player.destroy();
    m_enemyController.destroy();
}
