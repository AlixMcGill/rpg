#include "skeleton.h"

Skeleton::Skeleton(int startX, int startY, Texture& texture)
    : Enemy(startX, startY, texture) {

}

void Skeleton::update(float deltaTime, float& playerXPos, float& playerYPos, std::vector<std::vector<Tilemap::sTile>>& collisionLayer) {
    m_frameTimer += deltaTime;
    m_pathfindTimer += deltaTime;

    float moveX = 0.0f;
    float moveY = 0.0f;

    m_stateHandling(playerXPos, playerYPos, collisionLayer);
    m_stateCheck(deltaTime, moveY, moveX);
    updateAndCollide(moveX, moveY, collisionLayer);
}

/*
void Skeleton::m_stateHandling(float& playerXPos, float& playerYPos, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer) {
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

void Skeleton::m_stateCheck(float& deltaTime, float& moveY, float& moveX) {
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
} */
