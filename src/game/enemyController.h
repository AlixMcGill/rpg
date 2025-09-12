#pragma once
#include "../utils/project.h"
#include "enemy.h"
#include "tilemap.h"
#include "../enemy/skeleton.h"
#include <algorithm>
#include <memory>
#include "player.h"

class EnemyController {
public:
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Enemy*> behindPlayerEnemys;
    std::vector<Enemy*> frontPlayerEnemys;

    std::unordered_map<std::string, Texture> textures;

    std::vector<DamageText> damageTexts;

    void init(std::string map);
    void update(float deltaTime, float& playerXPos, float& playerYPos, Player& player, std::vector<std::vector<sTile>>& collisionLayer);
    void drawBehindPlayer();
    void drawFrontPlayer();
    void destroy(); 
private:
    void m_sortDrawOrder(float& playerYPos);
    Texture m_loadTexture(const char* path);
    void m_cleanEnemys();
    void m_updateDamageTexts(float deltaTime);
    void m_unload();

    // Enemy Spawning Function
    void m_spawnSkeleton(int x, int y, Texture& textrue);
};
