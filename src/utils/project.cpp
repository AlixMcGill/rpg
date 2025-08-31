#include "project.h"

int windowWidth = 1280;
int windowHeight = 720;

void setCollider(float width, float height, float offsetX, float offsetY, Rectangle& colliderRect) {
    colliderRect.width = width;
    colliderRect.height = height;
    colliderRect.x = offsetX;
    colliderRect.y = offsetY;
}


// Dice rolls

int rollD20() {
    return (std::rand() % 20) + 1;
}

int rollD10() {
    return (std::rand() % 10) + 1;
}
