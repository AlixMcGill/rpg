#pragma once
#include "../utils/project.h"
#include "object.h"
#include "player.h"
#include "enemyController.h"
#include <memory>
#include <raylib.h>
#include <unordered_map>

class ObjectManager {
public:
    std::unordered_map<std::string, std::unique_ptr<Object>> objects;
    Player* player = nullptr;
    Texture2D* textureAtlas = nullptr;
    std::vector<std::vector<sTile>>* collisionLayer = nullptr;
    //EnemyController* enemyController = nullptr; // may not need this yet but could come in handy

    void init(Player* p, Texture2D* texAtlas, std::vector<std::vector<sTile>>* collLayer);
    void addObject(std::string id, float tileX, float tileY, std::unique_ptr<Object> object);
    void update();
    void draw();
    void destroy();
private:

};
