#include "object.h"
#include <raylib.h>

void Object::initObject(float tileX, float tileY, Texture2D* texAtlas, std::vector<std::vector<sTile>>* collLayer) {
    objPosition.x = tileX;
    objPosition.y = tileY;
    textureAtlas = texAtlas;
    collisionLayer = collLayer;
}

void Object::update(){}

void Object::draw(){}


void Object::updateCollisionTile(bool activateTile) {
    if (activateTile) {
        (*collisionLayer)[objPosition.y][objPosition.x].id = 0;
    } else if (!activateTile) {
        (*collisionLayer)[objPosition.y][objPosition.x].id = -1;
    }
}
