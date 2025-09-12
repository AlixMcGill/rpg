#include "entity.h"

Rectangle Entity::getCollisionBounds(float futureX, float futureY) const {
    float colliderWidth = 4;
    float colliderHeight = 4;

    float offsetX = -2;
    float offsetY = (colliderHeight) / 2.0f;

    return {futureX + offsetX, futureY + offsetY, colliderWidth, colliderHeight};
}

bool Entity::isColliding(const Rectangle& bounds, const std::vector<std::vector<sTile>>& worldCollisionLayer) {

    // Get Tiles player is overlapping
    int startX = bounds.x / TILE_WIDTH;
    int endX = (bounds.x + bounds.width) / TILE_WIDTH;
    int startY = bounds.y / TILE_HEIGHT;
    int endY = (bounds.y + bounds.height) / TILE_HEIGHT;

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            // check if in bounds
            if (y < 0 || y >= (int)worldCollisionLayer.size() ||
                x < 0 || x >= (int)worldCollisionLayer.size()) {
                continue; // Ignore tiles out of bounds
            }

            const sTile& tile = worldCollisionLayer[y][x];

            if (tile.id != -1) {
                return true;
            }
        }
    }

    return false;
}
