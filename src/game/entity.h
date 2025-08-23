#pragma once
#include "../utils/project.h"
#include <raylib.h>
#include "tilemap.h"

class Entity {
public:
    float xPos;
    float yPos;

    Vector2 vel;
    Vector2 accel;

    Rectangle getCollisionBounds(float futureX, float futureY) const;
    bool isColliding(const Rectangle& bounds, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer);

private:
};
