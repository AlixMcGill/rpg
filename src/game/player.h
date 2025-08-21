#pragma once
#include "../utils/project.h"
#include "entity.h"
#include "tilemap.h"
#include <raylib.h>

class Player : public Entity {
public:
    float stamina;
    float health;

    bool isAnimating = false;

    enum state {
        IDLE,
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
        RUN
    };

    state currentState;

    void init();
    void update(float deltaTime,const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer);
    void draw();
    void destroy();

    bool isColliding(const Rectangle& playerBounds, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer);
private:
    Texture2D m_playerTexture;
    int m_playerTileX;
    int m_playerTileY;

    float m_frameTimer = 0.0f;
    float m_frameTime = 0.12f;
    float m_defaultFrameTime = 0.12f;

    float m_speed;
    float m_accel = 800.0f;
    float m_drag = 1600.0f;

    float m_walkSpeed = 80.0f;
    float m_runSpeed = 130.0f;
    float m_maxStamina = 100.0f;

    void m_setStartPos(int x, int y);
    void m_loadPlayerTexture(const char* imgPath);
    Rectangle m_getCollisionBounds(float futureX, float futureY) const;
    std::string m_getMouseDirection();
};
