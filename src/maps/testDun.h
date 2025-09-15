#pragma once
#include "../utils/project.h"
#include "../game/tilemap.h"
#include "../game/objectManager.h"

class TestDun : public Tilemap {
public:
    ObjectManager objManager;
    void load(Player* p) override;
    void update(float deltaTime, float playerXPos, float playerYPos) override;
    void draw() override;
};
