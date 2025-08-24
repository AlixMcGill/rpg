#include "enemy.h"
#include <cstdlib>
#include <raylib.h>

Enemy::Enemy(int startX, int startY, Texture& textrue)
        : m_enemyTexture(textrue) {
    m_setStartPos(startX, startY);
    m_enemyTileX = 0;
    m_enemyTileY = 0;
    currentState = IDLE_DOWN;
    m_setBoxCollider(4.0f, 4.0f, -2.0f, 2.0f);
}

void Enemy::update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer ) {
    m_frameTimer += deltaTime;
    m_pathfindTimer += deltaTime;

    float moveX = 0.0f;
    float moveY = 0.0f;

    if (m_pathfindTimer >= m_pathfindTime) {
        if (m_isPlayerNear(playerXPos, playerYPos) && !m_isInAttackRange(playerXPos, playerYPos)) { 
            // enemy will pathfind towards the player when near
            m_pathfindTime = m_defaultPathfindTime;
            m_pathfindTimer = 0.0f;

            /*float playerDis = distance(xPos, yPos, playerXPos, playerYPos);

            float dx = playerXPos - xPos;
            float dy = playerYPos - yPos;

            if (playerDis > 0) {
                xPos += (dx / playerDis) * m_moveSpeed;
                yPos += (dy / playerDis) * m_moveSpeed;
            }*/
            std::string whereIsPlayer = m_whereIsPlayer(playerXPos, playerYPos);

            if (whereIsPlayer == "UP") {
                currentState = WALK_UP;
            } else if (whereIsPlayer == "DOWN") {
                currentState = WALK_DOWN;
            } else if (whereIsPlayer == "LEFT") {
                currentState = WALK_LEFT;
            } else if (whereIsPlayer == "RIGHT") {
                currentState = WALK_RIGHT;
            }

        } else if (m_isInAttackRange(playerXPos, playerYPos)) {
            // enemy will attack the player if near
            m_pathfindTime = m_attackPathfindTime;
            m_pathfindTimer = 0.0f;

            std::string whereIsPlayer = m_whereIsPlayer(playerXPos, playerYPos);

            if (whereIsPlayer == "UP") {
                currentState = ATTACK_UP;
            } else if (whereIsPlayer == "DOWN") {
                currentState = ATTACK_DOWN;
            } else if (whereIsPlayer == "LEFT") {
                currentState = ATTACK_LEFT;
            } else if (whereIsPlayer == "RIGHT") {
                currentState = ATTACK_RIGHT;
            }

        } else {
            // enemy will randomly walk around if the player is not near
            m_pathfindTime = m_defaultPathfindTime;
            m_pathfindTimer = 0.0f;
            currentState = m_randomMoveState(); 
        }
    }

    switch (currentState) {
        case IDLE_UP:
            currentState = IDLE_UP;
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 2;
                m_enemyTileX++;

                if (m_enemyTileX > 5) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case IDLE_DOWN:
            currentState = IDLE_DOWN;
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 0;
                m_enemyTileX++;

                if (m_enemyTileX > 5) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case IDLE_LEFT:
            currentState = IDLE_LEFT;
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 1;
                m_enemyTileX++;

                if (m_enemyTileX > 5) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case IDLE_RIGHT:
            currentState = IDLE_RIGHT;
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 1;
                m_enemyTileX++;

                if (m_enemyTileX > 5) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case WALK_UP:
            currentState = WALK_UP;
            moveY += -m_moveSpeed * deltaTime;
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 2;
                m_enemyTileX++;

                if (m_enemyTileX > 5) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case WALK_DOWN:
            currentState = WALK_DOWN;
            moveY += m_moveSpeed * deltaTime;
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 3;
                m_enemyTileX++;

                if (m_enemyTileX > 5) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case WALK_LEFT:
            currentState = WALK_LEFT;
            moveX += -m_moveSpeed * deltaTime;
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 4;
                m_enemyTileX++;

                if (m_enemyTileX > 5) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case WALK_RIGHT:
            currentState = WALK_RIGHT;
            moveX += m_moveSpeed * deltaTime;
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 4;
                m_enemyTileX++;

                if (m_enemyTileX > 5) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case ATTACK_UP:
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 9;
                m_enemyTileX++;

                if (m_enemyTileX > 3) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case ATTACK_DOWN:
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 7;
                m_enemyTileX++;

                if (m_enemyTileX > 3) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case ATTACK_LEFT:
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 8;
                m_enemyTileX++;

                if (m_enemyTileX > 3) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        case ATTACK_RIGHT:
            if (m_frameTimer >= m_frameTime) {
                m_enemyTileY = 8;
                m_enemyTileX++;

                if (m_enemyTileX > 3) {
                    m_enemyTileX = 0;
                }
                m_frameTimer = 0.0f;
            }
            break;
        default:
            break;
    }

    // update enemy position
    // also handles collision
    
    Rectangle futureX = m_getCollisionBounds(xPos + moveX, yPos);
    if (!isColliding(futureX, collisionLayer)) { 
        xPos += moveX;
    }

    Rectangle futureY = m_getCollisionBounds(xPos, yPos + moveY);
    if (!isColliding(futureY, collisionLayer)) {
        yPos += moveY;
    }
}

void Enemy::draw() {
    Rectangle source = { (float)m_enemyTileX * PLAYER_TILE_WIDTH, (float)m_enemyTileY * PLAYER_TILE_HEIGHT, (float)PLAYER_TILE_WIDTH, (float)PLAYER_TILE_HEIGHT};

    if (currentState == IDLE_LEFT || currentState == WALK_LEFT || currentState == ATTACK_LEFT) {
        source.x = (float)(m_enemyTileX) * PLAYER_TILE_WIDTH; // right edge of current frame
        source.width = -(float)PLAYER_TILE_WIDTH;                  // flip horizontally
    } else {
        source.x = (float)m_enemyTileX * PLAYER_TILE_WIDTH;      // left edge of current frame
        source.width = (float)PLAYER_TILE_WIDTH;
    }

    Rectangle dest = {(float)(xPos), (float)(yPos), (float)PLAYER_TILE_WIDTH, (float)PLAYER_TILE_HEIGHT};
    Vector2 origin = {16,16};
    DrawTexturePro(m_enemyTexture, source, dest, origin, 0.0f, WHITE);

    // draw the players collider bounds
    //Rectangle collider = m_getCollisionBounds(xPos, yPos);
    //DrawRectangleLinesEx(collider, 1, RED);
}

void Enemy::m_setStartPos(int x, int y) {
    xPos = x * TILE_WIDTH;
    yPos = y * TILE_HEIGHT;
}

Enemy::state Enemy::m_randomMoveState() {

    std::vector<Enemy::state> validStates = {
        IDLE_UP, IDLE_DOWN, IDLE_LEFT, IDLE_RIGHT,
        WALK_UP, WALK_DOWN, WALK_LEFT, WALK_RIGHT
    };

    return validStates[std::rand() % validStates.size()];
}

bool Enemy::m_isPlayerNear(float& playerXPos, float& playerYPos) {
    float playerDistance = distance(xPos, yPos, playerXPos, playerYPos);

    if (playerDistance <= m_playerDistance && playerDistance > 0) {
        return true;
    } else {
        return false;
    }
}
bool Enemy::m_isInAttackRange(float& playerXPos, float& playerYPos) {
    float playerDistance = distance(xPos, yPos, playerXPos, playerYPos);

    if (playerDistance <= m_attackRange) {
        return true;
    } else {
        return false;
    }

}

std::string Enemy::m_whereIsPlayer(float& playerXPos, float& playerYPos) {
    if (std::abs(playerXPos - xPos) > std::abs(playerYPos - yPos)) {
        return (playerXPos > xPos) ? "RIGHT" : "LEFT";
    } else {
        return (playerYPos > yPos) ? "DOWN" : "UP";
    }
}

void Enemy::m_setBoxCollider(float width, float height, float offsetX, float offsetY) {
    m_collisionRect.width = width;
    m_collisionRect.height = height;
    m_collisionOffset.x = offsetX;
    m_collisionOffset.y = offsetY;
}

Rectangle Enemy::m_getCollisionBounds(float futureX, float futureY) const {
    return {futureX + m_collisionOffset.x, futureY + m_collisionOffset.y, m_collisionRect.width, m_collisionRect.height};
}

bool Enemy::isColliding(const Rectangle& bounds, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer) {

    // Get Tiles player is overlapping
    int startX = bounds.x / TILE_WIDTH;
    int endX = (bounds.x + bounds.width) / TILE_WIDTH;
    int startY = bounds.y / TILE_HEIGHT;
    int endY = (bounds.y + bounds.height) / TILE_HEIGHT;

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
