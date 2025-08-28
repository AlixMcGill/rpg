#include "enemy.h"
#include <cstdlib>
#include <raylib.h>

Enemy::Enemy(int startX, int startY, Texture& textrue)
        : m_enemyTexture(textrue) {
    m_setStartPos(startX, startY);
    m_enemyTileX = 0;
    m_enemyTileY = 0;
    currentState = IDLE_DOWN;
    setBoxCollider(4.0f, 4.0f, -2.0f, 2.0f);
}

void Enemy::update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer) {
    m_frameTimer += deltaTime;
    m_pathfindTimer += deltaTime;

    float moveX = 0.0f;
    float moveY = 0.0f;

    m_stateHandling(playerXPos, playerYPos, collisionLayer);
    m_stateCheck(deltaTime, moveY, moveX);
    updateAndCollide(moveX, moveY, collisionLayer);
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

bool Enemy::canSeePlayer(float& playerXPos, float& playerYPos, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer) {

    // start and end position for ray
    float startX = xPos + m_collisionRect.width / 2;
    float startY = yPos + m_collisionRect.height / 2;

    float dx = playerXPos - startX;
    float dy = playerYPos - startY;

    float distanceToPlayer = distance(startX, startY, playerXPos, playerYPos);

    int steps = (int)(distanceToPlayer / TILE_WIDTH);

    float stepX = dx / steps;
    float stepY = dy / steps;

    for (int i = 0; i < steps; i++) {
        float checkX = startX + stepX * i;
        float checkY = startY + stepY * i;

        int tileX = (int)(checkX / TILE_WIDTH);
        int tileY = (int)(checkY / TILE_HEIGHT);

        if (tileY >= 0 && tileY < (int)worldCollisionLayer.size() &&
            tileX >= 0 && tileX < (int)worldCollisionLayer[0].size()) {
            if (worldCollisionLayer[tileY][tileX].id != -1) {
                return false;
            }
        }
    }

    return true;
}

std::string Enemy::m_whereIsPlayer(float& playerXPos, float& playerYPos) {
    if (std::abs(playerXPos - xPos) > std::abs(playerYPos - yPos)) {
        return (playerXPos > xPos) ? "RIGHT" : "LEFT";
    } else {
        return (playerYPos > yPos) ? "DOWN" : "UP";
    }
}

void Enemy::setBoxCollider(float width, float height, float offsetX, float offsetY) {
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

void Enemy::animate(float startY, float endX) {
    if (m_frameTimer >= m_frameTime) {
        m_enemyTileY = startY;
        m_enemyTileX++;

        if (m_enemyTileX > endX) {
            m_enemyTileX = 0;
        }
        m_frameTimer = 0.0f;
    }
}

void Enemy::updateAndCollide(float& moveX, float& moveY, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer) {
    Rectangle futureX = m_getCollisionBounds(xPos + moveX, yPos);
    if (!isColliding(futureX, worldCollisionLayer)) { 
        xPos += moveX;
    }

    Rectangle futureY = m_getCollisionBounds(xPos, yPos + moveY);
    if (!isColliding(futureY, worldCollisionLayer)) {
        yPos += moveY;
    }

}

void Enemy::m_stateHandling(float& playerXPos, float& playerYPos, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer) {
    if (m_pathfindTimer >= m_pathfindTime) {
        if (m_isPlayerNear(playerXPos, playerYPos) && !m_isInAttackRange(playerXPos, playerYPos) &&
            canSeePlayer(playerXPos, playerYPos, worldCollisionLayer)) { 
            // enemy will pathfind towards the player when near
            m_pathfindTime = m_defaultPathfindTime;
            m_pathfindTimer = 0.0f;

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

}

void Enemy::m_stateCheck(float& deltaTime, float& moveY, float& moveX) {
    switch (currentState) {
        case IDLE_UP:
            currentState = IDLE_UP;
            animate(2.0f, 5.0f);
            break;
        case IDLE_DOWN:
            currentState = IDLE_DOWN;
            animate(0.0f, 5.0f);
            break;
        case IDLE_LEFT:
            currentState = IDLE_LEFT;
            animate(1.0f, 5.0f);
            break;
        case IDLE_RIGHT:
            currentState = IDLE_RIGHT;
            animate(1, 5.0f);
            break;
        case WALK_UP:
            currentState = WALK_UP;
            moveY += -m_moveSpeed * deltaTime;
            animate(2.0f, 5.0f);
            break;
        case WALK_DOWN:
            currentState = WALK_DOWN;
            moveY += m_moveSpeed * deltaTime;
            animate(3.0f, 5.0f);
            break;
        case WALK_LEFT:
            currentState = WALK_LEFT;
            moveX += -m_moveSpeed * deltaTime;
            animate(4.0f, 5.0f);
            break;
        case WALK_RIGHT:
            currentState = WALK_RIGHT;
            moveX += m_moveSpeed * deltaTime;
            animate(4.0f, 5.0f);
            break;
        case ATTACK_UP:
            animate(9.0f, 3.0f);
            break;
        case ATTACK_DOWN:
            animate(7.0f, 3.0f);
            break;
        case ATTACK_LEFT:
            animate(8.0f, 3.0f);
            break;
        case ATTACK_RIGHT:
            animate(8.0f, 3.0f);
            break;
        default:
            break;
    }
}
