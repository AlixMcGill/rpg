#include "mapManager.h"
#include <memory>
#include <raylib.h>
#include <stdexcept>

void MapManager::init(Player* p, EnemyController* e) {
    player = p;
    enemyController = e;

    addMap("Overworld_Map", std::make_unique<OverworldMap>());
    addMap("TestDungeon_Map", std::make_unique<TestDun>());

    loadMap("Overworld_Map");
    //loadMap("TestDungeon_Map");
}

void MapManager::addMap(const std::string& id, std::unique_ptr<Tilemap> map) {
    maps[id] = std::move(map);
}

void MapManager::loadMap(const std::string& id) {
    if (activeMap) activeMap->unload();

    auto it = maps.find(id);
    if (it != maps.end()) {
        activeMap = it->second.get();
        activeMap->load(player);
        enemyController->init(id);
    }
}

void MapManager::update(float deltaTime) {
    if (activeMap) activeMap->update(deltaTime, player->xPos, player->yPos);
}

void MapManager::draw() {
    if (activeMap) activeMap->draw();
}

// getting tilemap stuff

Camera2D& MapManager::getCamera() {
    if (!activeMap) throw std::runtime_error("No active map: Tried to ref camera");
    return activeMap->getCamera();
}

const Camera2D& MapManager::getCamera() const {
    if (!activeMap) throw std::runtime_error("No active map: Tried to ref const camera");
    return activeMap->getCamera();
}
std::vector<std::vector<sTile>>& MapManager::getWorldCollisionLayer() {
    if (!activeMap) throw std::runtime_error("No active map: Tried to ref world collision layer");
    return activeMap->getWorldCollisionLayer();
}

const std::vector<std::vector<sTile>>& MapManager::getWorldCollisionLayer() const {
    if (!activeMap) throw std::runtime_error("No active map: Tried to ref const world collision layer");
    return activeMap->getWorldCollisionLayer();

}
