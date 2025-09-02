#include "enemyController.h"
#include <algorithm>
#include <memory>
#include <raylib.h>

void EnemyController::init(std::string map) {
    if (map == "Overworld_Map") {
        enemies.clear();

        // init the textures to be used in level
        textures["Skeleton"] = m_loadTexture("assets/enemy/Skeleton.png");

        m_spawnSkeleton(10, 20, textures["Skeleton"]);
        m_spawnSkeleton(10, 22, textures["Skeleton"]);
        m_spawnSkeleton(10, 24, textures["Skeleton"]);
        m_spawnSkeleton(10, 18, textures["Skeleton"]);
        m_spawnSkeleton(30, 18, textures["Skeleton"]);
        m_spawnSkeleton(10, 38, textures["Skeleton"]);
        m_spawnSkeleton(10, 28, textures["Skeleton"]);
        m_spawnSkeleton(10, 20, textures["Skeleton"]);
        m_spawnSkeleton(10, 22, textures["Skeleton"]);
        m_spawnSkeleton(10, 24, textures["Skeleton"]);
        m_spawnSkeleton(10, 18, textures["Skeleton"]);
        m_spawnSkeleton(30, 18, textures["Skeleton"]);
        m_spawnSkeleton(10, 38, textures["Skeleton"]);
        m_spawnSkeleton(10, 28, textures["Skeleton"]);
        m_spawnSkeleton(10, 20, textures["Skeleton"]);
        m_spawnSkeleton(10, 22, textures["Skeleton"]);
        m_spawnSkeleton(10, 24, textures["Skeleton"]);
        m_spawnSkeleton(10, 18, textures["Skeleton"]);
        m_spawnSkeleton(30, 18, textures["Skeleton"]);
        m_spawnSkeleton(10, 38, textures["Skeleton"]);
        m_spawnSkeleton(10, 28, textures["Skeleton"]);
        m_spawnSkeleton(10, 20, textures["Skeleton"]);
        m_spawnSkeleton(10, 22, textures["Skeleton"]);
        m_spawnSkeleton(10, 24, textures["Skeleton"]);
        m_spawnSkeleton(10, 18, textures["Skeleton"]);
        m_spawnSkeleton(30, 18, textures["Skeleton"]);
        m_spawnSkeleton(10, 38, textures["Skeleton"]);
        m_spawnSkeleton(10, 20, textures["Skeleton"]);
        m_spawnSkeleton(10, 22, textures["Skeleton"]);
        m_spawnSkeleton(10, 24, textures["Skeleton"]);
        m_spawnSkeleton(10, 18, textures["Skeleton"]);
    }
}
void EnemyController::update(float deltaTime, float& playerXPos, float& playerYPos, Player& player, std::vector<std::vector<Tilemap::sTile>>& collisionLayer) {
    for (auto& e : enemies) {
        e->update(deltaTime, playerXPos, playerYPos, collisionLayer, damageTexts, player);
    }

    m_updateDamageTexts(deltaTime);
    m_sortDrawOrder(playerYPos);
}

void EnemyController::drawBehindPlayer() {
    for (auto& e : behindPlayerEnemys) {
            e->draw(damageTexts);
    }

    // Garbage collect any dead enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e) {
                return e->isDead();
            }
        ),
        enemies.end()
    );
}

void EnemyController::drawFrontPlayer() {
    for (auto& e : frontPlayerEnemys) {
            e->draw(damageTexts);
    }
}

void EnemyController::destroy() {
    for (auto& [name, texts] : textures) {
        UnloadTexture(texts);
    }
}

void EnemyController::m_sortDrawOrder(float& playerYPos) {
    float centerPlayerY = playerYPos + 16.0f;

    behindPlayerEnemys.clear();
    frontPlayerEnemys.clear();

    for (auto& e : enemies) {
        if (e->yPos + 16 < centerPlayerY) {
            behindPlayerEnemys.push_back(e.get());
        } else {
            frontPlayerEnemys.push_back(e.get());
        }
    }
}

Texture EnemyController::m_loadTexture(const char* path) {
    Image image = LoadImage(path);
    Texture text = LoadTextureFromImage(image);
    UnloadImage(image);
    return text;
}

void EnemyController::m_cleanEnemys() {
    enemies.clear();
}

void EnemyController::m_updateDamageTexts(float deltaTime) {
    for (auto& dt : damageTexts) {
        dt.timer += deltaTime;
        dt.position.y -= dt.speedY * deltaTime;  // move upward

        float alpha = 1.0f - (dt.timer / dt.duration);
        if (alpha < 0.0f) alpha = 0.0f;
        dt.color.a = static_cast<unsigned char>(alpha * 255);
    }

    damageTexts.erase(
        std::remove_if(damageTexts.begin(), damageTexts.end(),
            [](const DamageText& dt) {
                return dt.timer >= dt.duration;  // remove expired
            }),
        damageTexts.end()
    );

}

// Enemy Spawing Methods
void EnemyController::m_spawnSkeleton(int x, int y, Texture& texture) {
    enemies.push_back(std::make_unique<Skeleton>(x, y, texture));
}
