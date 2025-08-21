#include "player.h"
#include <raylib.h>

void Player::init() {
    m_setStartPos(3, 3);
    m_loadPlayerTexture("assets/player/Player.png");

    m_playerTileX = 6;
    m_playerTileY = 0;
    stamina = 100.0f;
}

void Player::update(float deltaTime) {
    m_frameTimer += deltaTime;
    if (m_frameTimer >= m_frameTime) {
        m_frameTimer = 0.0f;

        if (currentState == IDLE) {
            m_playerTileY = 0;
            m_playerTileX++;

            if (m_playerTileX > 5) {
                m_playerTileX = 0;
            }
        } else if (currentState == WALK_DOWN) {
            m_playerTileY = 3;
            m_playerTileX++;

            if (m_playerTileX > 5) {
                m_playerTileX = 0;
            }
        } else if (currentState == WALK_UP) {
            m_playerTileY = 5;
            m_playerTileX++;

            if (m_playerTileX > 5) {
                m_playerTileX = 0;
            }
        } else if (currentState == WALK_RIGHT) {
            m_playerTileY = 4;
            m_playerTileX++;

            if (m_playerTileX > 5) {
                m_playerTileX = 0;
            }
        } else if (currentState == WALK_LEFT) {
            m_playerTileY = 4;
            m_playerTileX--;

            if (m_playerTileX < 0) {
                m_playerTileX = 5;
            }
        }
    }


    accel = {0,0};

    // handle running
    if (IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT) && stamina > 1.0f) {
        if (stamina > 0.0f) {
            stamina -= 0.1f;
        }
        m_speed = m_runSpeed;
        m_frameTime = 0.05f;
    } else {
        m_speed = m_walkSpeed;
        m_frameTime = m_defaultFrameTime;
    }

    // input acceleration
    if (IsKeyDown(KEY_W)) {
        accel.y = -m_accel;
    }
    if (IsKeyDown(KEY_S)) {
        accel.y = m_accel;
    }
    if (IsKeyDown(KEY_A)) {
        accel.x = -m_accel;
    }
    if (IsKeyDown(KEY_D)) {
        accel.x = m_accel;
    }

    // update velocity
    vel.x += accel.x * deltaTime;
    vel.y += accel.y * deltaTime;

    // apply friction
    if (accel.x == 0) vel.x = (vel.x > 0) ? std::max(0.0f, vel.x - m_drag*deltaTime) : std::min(0.0f, vel.x + m_drag*deltaTime);
    if (accel.y == 0) vel.y = (vel.y > 0) ? std::max(0.0f, vel.y - m_drag*deltaTime) : std::min(0.0f, vel.y + m_drag*deltaTime);

    // clamp the velocity to max speed;
    if (vel.x > m_speed) vel.x = m_speed;
    if (vel.x < -m_speed) vel.x = -m_speed;
    if (vel.y > m_speed) vel.y = m_speed;
    if (vel.y < -m_speed) vel.y = -m_speed;

    // update player position
    //
    xPos += vel.x * deltaTime;
    yPos += vel.y * deltaTime;

    if (stamina > m_maxStamina) {
        stamina = m_maxStamina;
    } else {
        stamina += 0.01f;
    }

    if (IsKeyDown(KEY_W)) {
        currentState = WALK_UP;
    } else if (IsKeyDown(KEY_S)) {
        currentState = WALK_DOWN;
    } else if (IsKeyDown(KEY_A)) {
        currentState = WALK_LEFT;
    } else if (IsKeyDown(KEY_D)) {
        currentState = WALK_RIGHT;
    }
    else {
        currentState = IDLE;
    }
}

void Player::draw() {
    Rectangle source = { (float)m_playerTileX * PLAYER_TILE_WIDTH, (float)m_playerTileY * PLAYER_TILE_HEIGHT, (float)PLAYER_TILE_WIDTH, (float)PLAYER_TILE_HEIGHT};
    if (vel.x < 0) {
        source.width = -source.width;
        source.x += m_playerTileX * PLAYER_TILE_WIDTH;
    }
    Rectangle dest = {(float)(xPos), (float)(yPos), (float)PLAYER_TILE_WIDTH, (float)PLAYER_TILE_HEIGHT};
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
