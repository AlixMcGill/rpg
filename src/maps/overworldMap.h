#pragma once
#include "../utils/project.h"
#include "../game/tilemap.h"

class OverworldMap : public Tilemap {
public:
    void load(float PlayerXPos, float playerYPos) override;
    void update(float deltaTime, float playerXPos, float playerYPos) override;
    void draw() override;
};
