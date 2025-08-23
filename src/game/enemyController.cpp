#include "enemyController.h"

void EnemyController::init(std::string map) {
    if (map == "Overworld_Map") {
        Enemy skel1;
        skel1.init(10, 10, "assets/enemy/Skeleton.png");
        enemys.push_back(skel1);
        Enemy skel2;
        skel2.init(10, 12, "assets/enemy/Skeleton.png");
        enemys.push_back(skel2);
        Enemy skel3;
        skel3.init(8, 12, "assets/enemy/Skeleton.png");
        enemys.push_back(skel3);
        Enemy skel4;
        skel4.init(8, 12, "assets/enemy/Skeleton.png");
        enemys.push_back(skel4);
        Enemy skel5;
        skel5.init(8, 12, "assets/enemy/Skeleton.png");
        enemys.push_back(skel5);
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
    for (auto& e : enemys) {
        e.destroy();
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
