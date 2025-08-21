#pragma once
#include "../utils/project.h"
#include "entity.h"
#include <raylib.h>

class Player : public Entity {
public:
    float stamina;
    float health;

    enum state {
        IDLE,
        WALK_UP,
        WALK_DOWN,
        WALK_LEFT,
        WALK_RIGHT,
        RUN
    };

    state currentState;

    void init();
    void update(float deltaTime);
    void draw();
    void destroy();
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
};
