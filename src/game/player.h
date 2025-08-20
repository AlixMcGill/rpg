#pragma once
#include "../utils/project.h"
#include "entity.h"
#include <raylib.h>

class Player : public Entity {
public:
    void init();
    void update(float deltaTime);
    void draw();
    void destroy();
private:
    Texture2D m_playerTexture;
    int m_playerTileX;
    int m_playerTileY;

    float m_frameTimer = 0.0f;
    float m_frameTime = 1.0f;

    float m_speed = 1.0f;

    void m_setStartPos(int x, int y);
    void m_loadPlayerTexture(const char* imgPath);
};
