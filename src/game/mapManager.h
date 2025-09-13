#pragma once
#include "../utils/project.h"
#include "tilemap.h"
#include <memory>
#include <raylib.h>
#include <unordered_map>
#include "player.h"
#include "enemyController.h"
#include "../maps/overworldMap.h"
#include "../maps/testDun.h"

class MapManager {
    std::unordered_map<std::string, std::unique_ptr<Tilemap>> maps;
    Tilemap* activeMap = nullptr;
    Player* player = nullptr;
    EnemyController* enemyController = nullptr;
public:
    void init(Player* p, EnemyController* e);
    void addMap(const std::string& id, std::unique_ptr<Tilemap> map);
    void loadMap(const std::string& id);
    void update(float deltaTime);
    void draw();

    Camera2D& getCamera();
    const Camera2D& getCamera() const;
    std::vector<std::vector<sTile>>& getWorldCollisionLayer();
    const std::vector<std::vector<sTile>>& getWorldCollisionLayer() const;
private:
};
