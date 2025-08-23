#include "enemy.h"
#include <cstdlib>
#include <raylib.h>

void Enemy::init(int startX, int startY, const char* spriteSheet) {
    m_setStartPos(startX, startY);
    m_spriteSheet = spriteSheet;
    m_loadEnemyTexture(m_spriteSheet);

    m_enemyTileX = 0;
    m_enemyTileY = 0;
}

void Enemy::update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer ) {
    m_frameTimer += deltaTime;
    m_pathfindTimer += deltaTime;

    if (m_pathfindTimer >= m_pathfindTime) {
        std::cout << "Check" << std::endl;
        if (m_isPlayerNear(playerXPos, playerYPos) && !m_isInAttackRange(playerXPos, playerYPos)) { 
            std::cout << "Near" << std::endl;
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
            std::cout << "Attack" << std::endl;
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
            std::cout << "Move" << std::endl;
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
            yPos += -m_moveSpeed;
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
            yPos += m_moveSpeed;
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
            xPos += -m_moveSpeed;
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
            xPos += +m_moveSpeed;
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
            yPos += 0.0f;
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
            yPos += 0.0f;
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
            xPos += 0.0f;
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
            xPos += 0.0f;
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

void Enemy::destroy() {
    UnloadTexture(m_enemyTexture);
}


void Enemy::m_setStartPos(int x, int y) {
    xPos = x * TILE_WIDTH;
    yPos = y * TILE_HEIGHT;
}

void Enemy::m_loadEnemyTexture(const char* imgPath) {
    Image image = LoadImage(imgPath);
    m_enemyTexture = LoadTextureFromImage(image);
    UnloadImage(image);
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
