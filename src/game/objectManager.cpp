#include "objectManager.h"
#include <memory>

void ObjectManager::init(Player* p, Texture2D* texAtlas, std::vector<std::vector<sTile>>* collLayer) {
    player = p;
    textureAtlas = texAtlas;
    collisionLayer = collLayer;
}

void ObjectManager::addObject(std::string id, float tileX, float tileY, std::unique_ptr<Object> object) {
   objects[id] = std::move(object);
   objects[id]->initObject(tileX, tileY, textureAtlas, collisionLayer);
}

void ObjectManager::update() {
    for (auto& [id, obj] : objects) {
        obj->update();
    }
}

void ObjectManager::draw() {
    for (auto& [id, obj] : objects) {
        obj->draw();
    }
}
