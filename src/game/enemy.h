#pragma once
#include "entity.h"
#include "tilemap.h"
#include "../utils/math.h"
#include "../utils/project.h"
#include <raylib.h>
#include <cmath>

struct TileNode {
    int x, y;
    float gCost, hCost;
    TileNode* parent = nullptr;

    float fcost() const {return gCost + hCost;}
};

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

    Rectangle tileSize;

    Enemy(int startX, int startY, Texture& textrue);
    virtual void update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer);
    void draw();
    void setBoxCollider(float width, float height, float offsetX, float offsetY);

    Texture2D& m_enemyTexture;
    int m_enemyTileX;
    int m_enemyTileY;

    Rectangle m_collisionRect;
    Vector2 m_collisionOffset;

    float m_frameTimer = 0.0f;
    float m_frameTime = 0.12f;
    float m_defaultFrameTime = 0.12f;

    float m_pathfindTimer = 0.0f;
    float m_pathfindTime = 0.1f;
    float m_attackPathfindTime = 0.1f;
    float m_defaultPathfindTime = 0.1f;
    float m_defaultRandomTime = 0.5f;

    float m_speed = 80.0f; // Max move speed
    float m_moveSpeed = 50.0f;

    float m_playerDistance = 130.0f;
    float m_attackRange = 25.0f;

    bool seenPlayer = false;
    Vector2 seenPlayerLast;

    std::vector<Vector2> path; // path for a* algorithm

    void m_setStartPos(int x, int y);
    state m_randomMoveState();
    void m_moveTowardsPlayer(float& playerXPos, float& playerYPos);
    bool m_isPlayerNear(float& playerXPos, float& playerYPos);
    bool m_isInAttackRange(float& playerXPos, float& playerYPos);
    bool canSeePlayer(float& playerXPos, float& playerYPos, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer);
    std::string m_whereIsPlayer(const float& playerXPos, const float& playerYPos);
    void m_arrivedLastSeen();
    Rectangle m_getCollisionBounds(float futureX, float futureY) const;
    bool isColliding(const Rectangle& bounds, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer);

    void updateAndCollide(float& moveX, float& moveY, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer);

    // tells the renderer what tiles to render on the sprite sheet
    void animate(float startY, float endX);

    // A* algorithm
    void computePath(int targetTileX, int targetTileY, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer);
    void followPath(float deltaTime, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer);

    void debugPathDraw();
    void debugSeePlayerDraw();
protected:
    virtual void m_stateHandling(float& playerXPos, float& playerYPos, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer);
    virtual void m_stateCheck(float& deltaTime, float& moveY, float& moveX);
};
