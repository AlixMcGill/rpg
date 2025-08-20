#pragma once
#include "../utils/project.h"
#include "../dev/devstats.h"
#include "tilemap.h"

class Game {
public:
    void init();
    void update(float deltaTime);
    void draw();
    void destroy();

private:
    Tilemap m_tilemap;
    DevStats dev;
};
