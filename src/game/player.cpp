#include "player.h"
#include "tilemap.h"
#include <ostream>
#include <raylib.h>

void Player::init() {
    m_setStartPos(3, 3);
    m_loadPlayerTexture("assets/player/player.png");

    m_setHitboxCollider(16.0f, 25.0f, -8.0f, -15.0f);
    m_setAttackCollider(20.0f, 16.0f, -10.0f, 16.0f); // default is the down collider

    m_playerTileX = 6;
    m_playerTileY = 0;
    stamina = 100.0f;
}

void Player::update(float deltaTime, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer,
                    std::vector<std::unique_ptr<Enemy>>& enemies) {
    m_frameTimer += deltaTime;
    if (m_frameTimer >= m_frameTime) { // Handles animations based off a timer
        m_frameTimer = 0.0f;

        if (currentState == IDLE) {
            m_playerTileY = 0;
            m_playerTileX++;

            if (m_playerTileX > 5) {
                m_playerTileX = 0;
            }
        } else if (currentState == WALK_DOWN) {
            m_playerTileY = 0;
            m_playerTileX++;

            if (m_playerTileX > 7) {
                m_playerTileX = 0;
            }
        } else if (currentState == WALK_UP) {
            m_playerTileY = 1;
            m_playerTileX++;

            if (m_playerTileX > 7) {
                m_playerTileX = 0;
            }
        } else if (currentState == WALK_RIGHT || currentState == WALK_LEFT) {
            m_playerTileY = 2;
            m_playerTileX++;

            if (m_playerTileX > 8) {
                m_playerTileX = 0;
            }
        } else if (currentState == IDLE_UP) {
            m_playerTileY = 1;
            m_playerTileX++;

            if (m_playerTileX > 1) {
                m_playerTileX = 0;
            }
        } else if (currentState == IDLE_DOWN) {
            m_playerTileY = 0;
            m_playerTileX++;

            if (m_playerTileX > 1) {
                m_playerTileX = 0;
            }
        } else if (currentState == IDLE_RIGHT || currentState == IDLE_LEFT) {
            m_playerTileY = 2;
            m_playerTileX++;

            if (m_playerTileX > 1) {
                m_playerTileX = 0;
            }
        }

        if (isAnimating) {
            if (currentState == ATTACK_DOWN) {
                m_playerTileY = 3;
                m_playerTileX++;

                if (m_playerTileX > 4) {
                    isAnimating = false;
                    m_playerTileX = 3;
                }

                m_updateAttack(ATTACK_DOWN, enemies);

            } else if (currentState == ATTACK_UP) {
                m_playerTileY = 3;
                m_playerTileX++;

                if (m_playerTileX > 4) {
                    isAnimating = false;
                    m_playerTileX = 3;
                }

                m_updateAttack(ATTACK_UP, enemies);

            } else if (currentState == ATTACK_RIGHT) {
                m_playerTileY = 3;
                m_playerTileX++;

                if (m_playerTileX > 4) {
                    isAnimating = false;
                    m_playerTileX = 3;
                }

                m_updateAttack(ATTACK_RIGHT, enemies);

            } else if (currentState == ATTACK_LEFT) {
                m_playerTileY = 3;
                m_playerTileX++;

                if (m_playerTileX > 4) {
                    isAnimating = false;
                    m_playerTileX = 3;
                }

                m_updateAttack(ATTACK_LEFT, enemies);
            }

        }
    }


    accel = {0,0};

    // handle running
    if ((IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT)) && stamina > 1.0f && (vel.x != 0 || vel.y != 0)) {
        if (stamina > 0.0f) {
            stamina -= 0.06f;
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
    // also handles collision
    
    Rectangle futureX = m_getCollisionBounds(xPos + vel.x * deltaTime, yPos);
    if (!isColliding(futureX, worldCollisionLayer)) { 
        xPos += vel.x * deltaTime;
    } else {
        vel.x = 0;
    }

    Rectangle futureY = m_getCollisionBounds(xPos, yPos + vel.y * deltaTime);
    if (!isColliding(futureY, worldCollisionLayer)) {
        yPos += vel.y * deltaTime;
    } else {
        vel.y = 0;
    }


    if (stamina > m_maxStamina) {
        stamina = m_maxStamina;
    } else {
        stamina += 0.01f;
    }

    // mouse / player face
    std::string mouseDir = m_getMouseDirection(); 

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !isAnimating && stamina >= 5.0f) {
        if (mouseDir == "UP") currentState = ATTACK_UP;
        else if (mouseDir == "DOWN") currentState = ATTACK_DOWN;
        else if (mouseDir == "LEFT") currentState = ATTACK_LEFT;
        else if (mouseDir == "RIGHT") currentState = ATTACK_RIGHT;
        isAnimating = true;
        canAttack = true;
        m_playerTileX = 0;
        stamina -= 5.0f;
    } else if (!isAnimating) {
        if ((vel.y != 0 || vel.x != 0) && mouseDir == "UP") {
            currentState = WALK_UP;
        } else if ((vel.y != 0 || vel.x != 0) && mouseDir == "DOWN") {
            currentState = WALK_DOWN;
        } else if ((vel.x != 0 || vel.y != 0) && mouseDir == "LEFT") {
            currentState = WALK_LEFT;
        } else if ((vel.x != 0 || vel.y != 0) && mouseDir == "RIGHT") {
            currentState = WALK_RIGHT;
        } else if ((vel.x == 0 && vel.y == 0) && mouseDir == "UP") {
            currentState = IDLE_UP;
        } else if ((vel.x == 0 && vel.y == 0) && mouseDir == "DOWN") {
            currentState = IDLE_DOWN;
        } else if ((vel.x == 0 && vel.y == 0) && mouseDir == "LEFT") {
            currentState = IDLE_LEFT;
        } else if ((vel.x == 0 && vel.y == 0) && mouseDir == "RIGHT") {
            currentState = IDLE_RIGHT;
        } else {
            currentState = IDLE;
        } 
    }

    // Health Regen
    m_regenHealth();
}

void Player::draw() {
    std::string mouseDir = m_getMouseDirection();
    Rectangle source = { (float)m_playerTileX * PLAYER_TILE_WIDTH, (float)m_playerTileY * PLAYER_TILE_HEIGHT, (float)PLAYER_TILE_WIDTH, (float)PLAYER_TILE_HEIGHT};

    if (mouseDir == "LEFT") {
        source.x = (float)(m_playerTileX) * PLAYER_TILE_WIDTH; // right edge of current frame
        source.width = -(float)PLAYER_TILE_WIDTH;                  // flip horizontally
    } else {
        source.x = (float)m_playerTileX * PLAYER_TILE_WIDTH;      // left edge of current frame
        source.width = (float)PLAYER_TILE_WIDTH;
    }

    Rectangle dest = {(float)(xPos), (float)(yPos), (float)PLAYER_TILE_WIDTH, (float)PLAYER_TILE_HEIGHT};
    Vector2 origin = {32,32};
    DrawTexturePro(m_playerTexture, source, dest, origin, 0.0f, WHITE);


    // debug drawing
    if (renderDebug == true) {
        Rectangle collider = m_getCollisionBounds(xPos, yPos);
        DrawRectangleLinesEx(collider, 1, RED);

        Rectangle hitColl = m_getHitboxBounds(xPos, yPos, hitBoxCollider);
        DrawRectangleLinesEx(hitColl, 1, BLUE);

        if (currentState == ATTACK_UP || currentState == ATTACK_DOWN ||
            currentState == ATTACK_LEFT || currentState == ATTACK_RIGHT) { // only draw the debug lines when attacking
            Rectangle attackColl = m_getHitboxBounds(xPos, yPos, attackCollider); 
            DrawRectangleLinesEx(attackColl, 1, RED);
        }
    }
    // draw the players collider bounds
}

void Player::destroy() {
    UnloadTexture(m_playerTexture);
}

bool Player::isColliding(const Rectangle& playerBounds, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer) {

    // Get Tiles player is overlapping
    int startX = playerBounds.x / TILE_WIDTH;
    int endX = (playerBounds.x + playerBounds.width) / TILE_WIDTH;
    int startY = playerBounds.y / TILE_HEIGHT;
    int endY = (playerBounds.y + playerBounds.height) / TILE_HEIGHT;

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            // check if in bounds
            if (y < 0 || y >= (int)worldCollisionLayer.size() ||
                x < 0 || x >= (int)worldCollisionLayer.size()) {
                continue; // Ignore tiles out of bounds
            }

            const Tilemap::sTile& tile = worldCollisionLayer[y][x];

            if (tile.id != -1) {
                return true;
            }
        }
    }

    return false;
}

void Player::damagePlayer(float damage) {
    health -= damage;
}


// Private Function


void Player::m_setStartPos(int x, int y) {
    xPos = x * TILE_WIDTH;
    yPos = y * TILE_HEIGHT;
}

void Player::m_loadPlayerTexture(const char* imgPath) {
    Image image = LoadImage(imgPath);
    m_playerTexture = LoadTextureFromImage(image);
    UnloadImage(image);
}

Rectangle Player::m_getCollisionBounds(float futureX, float futureY) const {
    float colliderWidth = 4;
    float colliderHeight = 4;

    float offsetX = -(colliderWidth / 2.0f);
    float offsetY = (colliderHeight) / 2.0f;

    return {futureX + offsetX, futureY + offsetY, colliderWidth, colliderHeight};
}

Rectangle Player::m_getHitboxBounds(float futureX, float futureY, Rectangle& hitbox) const {
    return {futureX + hitbox.x, futureY + hitbox.y, hitbox.width, hitbox.height};
}

std::string Player::m_getMouseDirection() {
    Vector2 screenMid = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    Vector2 mousePos = GetMousePosition();

    float mouseAngle = std::atan2f(mousePos.y - screenMid.y, mousePos.x - screenMid.x);

    if (mouseAngle > -PI/4 && mouseAngle <= PI/4) {
        return "RIGHT";
    } else if (mouseAngle > PI/4 && mouseAngle <= 3*PI/4) {
        return "DOWN";
    } else if (mouseAngle <= -PI/4 && mouseAngle > -3*PI/4) {
        return "UP";
    } else {
        return "LEFT";
    }
}

void Player::m_regenHealth() {
    health += healthRegen;

    if (health >= maxHealth) {
        health = maxHealth;
    } else if (health <= 0.0f) {
        m_killPlayer();
    }
}

void Player::m_killPlayer() {

}

void Player::m_setHitboxCollider(float width, float height, float baseOffsetX, float baseOffsetY) {
    setCollider(width, height, baseOffsetX, baseOffsetX, hitBoxCollider);
}

void Player::m_setAttackCollider(float width, float height, float baseOffsetX, float baseOffsetY) {
    setCollider(width, height, baseOffsetX, baseOffsetY, attackCollider);
}

void Player::m_updateAttack(state AttackState, std::vector<std::unique_ptr<Enemy>>& enemies) {
    switch (AttackState) { // updates the collider position
        case ATTACK_UP: 
            m_setAttackCollider(25.0f, 16.0f, -12.5f, -23.0f);
            break;
        case ATTACK_DOWN:
            m_setAttackCollider(25.0f, 16.0f, -12.5f, 16.0f);
            break;
        case ATTACK_LEFT: 
            m_setAttackCollider(16.0f, 25.0f, -23.0f, -8.0f);
            break;
        case ATTACK_RIGHT: 
            m_setAttackCollider(16.0f, 25.0f, 8.0f, -8.0f);
            break;
        default:
            break;
    }

    // handle the attack collision
    //
    // Check if the players attack hitbox is colliding with an enemys hitbox
    if (canAttack) {
        for (auto& enemy : enemies) {
            Rectangle playerAttackBox = m_getHitboxBounds(xPos, yPos, attackCollider);
            Rectangle enemyHitBox = m_getHitboxBounds(enemy->xPos, enemy->yPos, enemy->hitBox);

            if (CheckCollisionRecs(playerAttackBox, enemyHitBox)) {
                //std::cout << "Hit Enemy" << std::endl;
                enemy->takeDamage(38.0f);
            }
        }
    }
    canAttack = false; // can attack flag used to stop multiple hits from being registerd on one enemy per attack click
}
