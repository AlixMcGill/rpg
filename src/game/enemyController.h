#pragma once
#include "../utils/project.h"
#include "enemy.h"
#include "tilemap.h"

class EnemyController {
public:
    std::vector<Enemy> enemys;
    std::vector<Enemy> behindPlayerEnemys;
    std::vector<Enemy> frontPlayerEnemys;

    std::unordered_map<std::string, Texture> textures;

    void init(std::string map);
    void update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer);
    void drawBehindPlayer();
    void drawFrontPlayer();
    void destroy();
private:
    void m_sortDrawOrder(float& playerYPos);
    Texture m_loadTexture(const char* path);
    void m_cleanEnemys();
};
