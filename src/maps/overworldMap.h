#pragma once
#include "../utils/project.h"
#include "../game/tilemap.h"
#include "../game/objectManager.h"

class OverworldMap : public Tilemap {
public:
    void load(Player* p) override;
    void update(float deltaTime, float playerXPos, float playerYPos) override;
    void draw() override;
};
