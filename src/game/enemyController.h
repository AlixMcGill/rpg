#pragma once
#include "../utils/project.h"
#include "enemy.h"
#include "tilemap.h"
#include <vector>

class EnemyController {
public:
    std::vector<Enemy> enemys;

    void init(std::string map);
    void update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer);
    void draw();
    void destroy();
private:
};
