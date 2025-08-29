#include "enemyController.h"
#include <memory>
#include <raylib.h>

void EnemyController::init(std::string map) {
    if (map == "Overworld_Map") {
        enemies.clear();

        // init the textures to be used in level
        textures["Skeleton"] = m_loadTexture("assets/enemy/Skeleton.png");

        m_spawnSkeleton(10, 20, textures["Skeleton"]);
        m_spawnSkeleton(10, 22, textures["Skeleton"]);
        m_spawnSkeleton(10, 24, textures["Skeleton"]);
        m_spawnSkeleton(10, 28, textures["Skeleton"]);
        m_spawnSkeleton(10, 28, textures["Skeleton"]);
        m_spawnSkeleton(10, 28, textures["Skeleton"]);
        m_spawnSkeleton(10, 28, textures["Skeleton"]);
    }
}
void EnemyController::update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer) {
    for (auto& e : enemies) {
        e->update(deltaTime, playerXPos, playerYPos, collisionLayer);
    }

    m_sortDrawOrder(playerYPos);
}

void EnemyController::drawBehindPlayer() {
    for (auto& e : behindPlayerEnemys) {
            e->draw();
    }
}

void EnemyController::drawFrontPlayer() {
    for (auto& e : frontPlayerEnemys) {
            e->draw();
    }
}

void EnemyController::destroy() {
    for (auto& [name, texts] : textures) {
        UnloadTexture(texts);
    }
}

void EnemyController::m_sortDrawOrder(float& playerYPos) {
    float centerPlayerY = playerYPos + 16.0f;

    behindPlayerEnemys.clear();
    frontPlayerEnemys.clear();

    for (auto& e : enemies) {
        if (e->yPos + 16 < centerPlayerY) {
            behindPlayerEnemys.push_back(e.get());
        } else {
            frontPlayerEnemys.push_back(e.get());
        }
    }
}

Texture EnemyController::m_loadTexture(const char* path) {
    Image image = LoadImage(path);
    Texture text = LoadTextureFromImage(image);
    UnloadImage(image);
    return text;
}

void EnemyController::m_cleanEnemys() {
    enemies.clear();
}

// Enemy Spawing Methods
void EnemyController::m_spawnSkeleton(int x, int y, Texture& texture) {
    enemies.push_back(std::make_unique<Skeleton>(x, y, texture));
}
