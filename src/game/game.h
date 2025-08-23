#pragma once
#include "../utils/project.h"
#include "../dev/devstats.h"
#include "enemy.h"
#include "tilemap.h"
#include "player.h"
#include "enemyController.h"

class Game {
public:
    void init();
    void update(float deltaTime);
    void draw();
    void destroy();

private:
    Tilemap m_tilemap;
    DevStats dev;
    Player player;
    EnemyController m_enemyController;
    Enemy skely;
};
