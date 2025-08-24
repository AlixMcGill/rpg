#pragma once
#include "entity.h"
#include "tilemap.h"
#include "../utils/math.h"
#include "../utils/project.h"
#include <raylib.h>

class Enemy : public Entity {
public:
    enum state {
        IDLE_UP,
        IDLE_DOWN,
        IDLE_LEFT,
        IDLE_RIGHT,
        WALK_UP,
        WALK_DOWN,
        WALK_LEFT,
        WALK_RIGHT,
        ATTACK_UP,
        ATTACK_DOWN,
        ATTACK_LEFT,
        ATTACK_RIGHT,
        DIE
    };

    state currentState;

    Enemy(int startX, int startY, Texture& textrue);
    void update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer);
    void draw();
private:

    Texture2D& m_enemyTexture;
    int m_enemyTileX;
    int m_enemyTileY;

    float m_moveSpeed = 1.0f;

    float m_frameTimer = 0.0f;
    float m_frameTime = 0.12f;
    float m_defaultFrameTime = 0.12f;

    float m_pathfindTimer = 0.0f;
    float m_pathfindTime = 0.4f;
    float m_attackPathfindTime = 0.1f;
    float m_defaultPathfindTime = 0.4f;

    float m_speed = 1.0f;

    float m_playerDistance = 100.0f;
    float m_attackRange = 25.0f;

    void m_setStartPos(int x, int y);
    state m_randomMoveState();
    void m_moveTowardsPlayer(float& playerXPos, float& playerYPos);
    bool m_isPlayerNear(float& playerXPos, float& playerYPos);
    bool m_isInAttackRange(float& playerXPos, float& playerYPos);
    std::string m_whereIsPlayer(float& playerXPos, float& playerYPos);
};
