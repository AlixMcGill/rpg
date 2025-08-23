#pragma once
#include "../utils/project.h"
#include "enemy.h"
#include "tilemap.h"
#include <vector>

class EnemyController {
public:
    std::vector<Enemy> enemys;
    std::vector<Enemy> behindPlayerEnemys;
    std::vector<Enemy> frontPlayerEnemys;

    void init(std::string map);
    void update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer);
    void drawBehindPlayer();
    void drawFrontPlayer();
    void destroy();
private:
    void m_sortDrawOrder(float& playerYPos);
};
