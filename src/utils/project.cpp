#include "project.h"

int windowWidth = 1280;
int windowHeight = 720;

void setCollider(float width, float height, float offsetX, float offsetY, Rectangle& colliderRect) {
    colliderRect.width = width;
    colliderRect.height = height;
    colliderRect.x = offsetX;
    colliderRect.y = offsetY;
}
