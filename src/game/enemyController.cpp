#include "enemyController.h"
#include <raylib.h>

void EnemyController::init(std::string map) {
    if (map == "Overworld_Map") {
        enemys.clear();

        // init the textures to be used in level
        textures["Skeleton"] = m_loadTexture("assets/enemy/Skeleton.png");

        enemys.emplace_back(10, 10, textures["Skeleton"]);
        enemys.emplace_back(10, 12, textures["Skeleton"]);
        enemys.emplace_back(8, 12, textures["Skeleton"]);
        enemys.emplace_back(8, 12, textures["Skeleton"]);
        enemys.emplace_back(8, 12, textures["Skeleton"]);
    }
}
void EnemyController::update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer) {
    for (auto& e : enemys) {
        e.update(deltaTime, playerXPos, playerYPos, collisionLayer);
    }

    m_sortDrawOrder(playerYPos);
}

void EnemyController::drawBehindPlayer() {
    for (auto& e : behindPlayerEnemys) {
            e.draw();
    }
}

void EnemyController::drawFrontPlayer() {
    for (auto& e : frontPlayerEnemys) {
            e.draw();
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

    for (auto& e : enemys) {
        if (e.yPos + 16 < centerPlayerY) {
            behindPlayerEnemys.push_back(e);
        } else {
            frontPlayerEnemys.push_back(e);
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
    enemys.clear();
}
