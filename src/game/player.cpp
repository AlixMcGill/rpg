#include "player.h"
#include <raylib.h>

void Player::init() {
    m_setStartPos(3, 3);
    m_loadPlayerTexture("assets/Dungeon_Character.png");

    m_playerTileX = 6;
    m_playerTileY = 0;
}

void Player::update(float deltaTime) {
    m_frameTimer += deltaTime;
    if (m_frameTimer >= m_frameTime) {
        m_frameTimer = 0.0f;

        if (m_playerTileY > 0) {
            m_playerTileY = 0;
        } else {
            m_playerTileY = 2;
        }
    }

    if (IsKeyDown(KEY_W)) {
        yPos -= m_speed;
    }
    if (IsKeyDown(KEY_S)) {
        yPos += m_speed;
    }
    if (IsKeyDown(KEY_A)) {
        xPos -= m_speed;
    }
    if (IsKeyDown(KEY_D)) {
        xPos += m_speed;
    }
}

void Player::draw() {
    Rectangle source = { (float)m_playerTileX * TILE_WIDTH, (float)m_playerTileY * TILE_HEIGHT, (float)TILE_WIDTH, (float)TILE_HEIGHT};
    Rectangle dest = {(float)(xPos), (float)(yPos), (float)TILE_WIDTH, (float)TILE_HEIGHT};
    Vector2 origin = {0,0};
    DrawTexturePro(m_playerTexture, source, dest, origin, 0.0f, WHITE);
}

void Player::destroy() {
    UnloadTexture(m_playerTexture);
}

void Player::m_setStartPos(int x, int y) {
    xPos = x * TILE_WIDTH;
    yPos = y * TILE_HEIGHT;
}

void Player::m_loadPlayerTexture(const char* imgPath) {
    Image image = LoadImage(imgPath);
    m_playerTexture = LoadTextureFromImage(image);
    UnloadImage(image);
}
