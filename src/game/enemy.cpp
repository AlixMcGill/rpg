#include "enemy.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <raylib.h>

Enemy::Enemy(int startX, int startY, Texture& textrue)
        : m_enemyTexture(textrue) {
    m_setStartPos(startX, startY);
    m_enemyTileX = 0;
    m_enemyTileY = 0;
    currentState = IDLE_DOWN;
    setBoxCollider(2.0f, 2.0f, -1.0f, 2.0f);
    setHitbox(16.0f, 20.0f, -8.0f, -10.0f);
    tileSize.width = 32;
    tileSize.height = 32;
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
    Rectangle source = { (float)m_enemyTileX * tileSize.width, (float)m_enemyTileY * tileSize.height, (float)tileSize.width, (float)tileSize.height};

    if (currentState == IDLE_LEFT || currentState == WALK_LEFT || currentState == ATTACK_LEFT) {
        source.x = (float)(m_enemyTileX) * tileSize.width; // right edge of current frame
        source.width = -(float)tileSize.width;                  // flip horizontally
    } else {
        source.x = (float)m_enemyTileX * tileSize.width;      // left edge of current frame
        source.width = (float)tileSize.width;
    }

    Rectangle dest = {(float)(xPos), (float)(yPos), (float)tileSize.width, (float)tileSize.width};
    Vector2 origin = {16,16};
    DrawTexturePro(m_enemyTexture, source, dest, origin, 0.0f, WHITE);

    if (renderDebug == true) {
    // draw the players collider bounds
        Rectangle collider = m_getCollisionBounds(xPos, yPos);
        DrawRectangleLinesEx(collider, 1, RED);

        Rectangle hitBounds = getHitboxbounds(xPos, yPos);
        DrawRectangleLinesEx(hitBounds, 1, BLUE);

        debugPathDraw(); // Draws pathfinding 
        debugSeePlayerDraw();
    }
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
        path.clear();
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
            } else {
                // store the x and y where the player was seen last to pathfind to
                seenPlayerLast.x = tileX;
                seenPlayerLast.y = tileY;
                seenPlayer = true;
                //std::cout << "seen: " << seenPlayerLast.x << ", " << seenPlayerLast.y << std::endl;
            }
        }
    }
    return true;
}

std::string Enemy::m_whereIsPlayer(const float& playerXPos, const float& playerYPos) {
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

void Enemy::setHitbox(float width, float height, float offsetX, float offsetY) {
    setCollider(width, height, offsetX, offsetY, hitBox);
}

Rectangle Enemy::m_getCollisionBounds(float futureX, float futureY) const {
    return {futureX + m_collisionOffset.x, futureY + m_collisionOffset.y, m_collisionRect.width, m_collisionRect.height};
}

Rectangle Enemy::getHitboxbounds(float futureX, float futureY) const {
    return {futureX + hitBox.x, futureY + hitBox.y, hitBox.width, hitBox.height};
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
        if ((m_isPlayerNear(playerXPos, playerYPos) && !m_isInAttackRange(playerXPos, playerYPos) &&
            canSeePlayer(playerXPos, playerYPos, worldCollisionLayer)) || seenPlayer) { 
            // enemy will pathfind towards the player when near
            m_pathfindTime = m_defaultPathfindTime;
            m_pathfindTimer = 0.0f;

            if (path.empty()) {
                computePath((int)(seenPlayerLast.x), (int)(seenPlayerLast.y), worldCollisionLayer);
            }

            if (!path.empty()) {
                Vector2 nextTile = path.front();

                float targetX = (nextTile.x + 0.5f) * TILE_WIDTH;
                float targetY = (nextTile.y + 0.5f) * TILE_HEIGHT;

                float dx = targetX - (xPos + m_collisionRect.width / 2);
                float dy = targetY - (yPos + m_collisionRect.height / 2);

                if (std::abs(dx) > std::abs(dy)) {
                    currentState = (dx > 0) ? WALK_RIGHT : WALK_LEFT;
                } else if (dy != 0) { 
                    currentState = (dy > 0) ? WALK_DOWN : WALK_UP;
                }

                float dist = std::sqrt(dx*dx + dy*dy);
                if (dist < 5.0f) {
                    path.erase(path.begin());
                }
            }
            
            m_arrivedLastSeen();

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
            m_pathfindTime = m_defaultRandomTime;
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

void Enemy::m_arrivedLastSeen() {
    float range = 1.0f;
    float dx = (xPos / TILE_WIDTH) - seenPlayerLast.x;
    float dy = (yPos / TILE_HEIGHT) - seenPlayerLast.y;

    if (std::fabs(dx) < range && std::fabs(dy) < range) {
        seenPlayer = false;
        //seenPlayerLast = {0.0f, 0.0f};
        path.clear();
        //std::cout << "Arrived" << std::endl;
    }
}

void Enemy::computePath(int targetTileX, int targetTileY, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer) {
    int startX = (int)(xPos / TILE_WIDTH);
    int startY = (int)(yPos / TILE_HEIGHT);

    struct Node {
        int x, y;
        float gCost, hCost;
        Node* parent; 
    };

    std::vector<std::vector<bool>> closed(worldCollisionLayer.size(), std::vector<bool>(worldCollisionLayer[0].size(), false));
    std::vector<std::vector<Node*>> allNodes(worldCollisionLayer.size(), std::vector<Node*>(worldCollisionLayer[0].size(), nullptr));

    auto heuristic = [](int x1, int y1, int x2, int y2) {
        float dx = (x1 > x2) ? x1 - x2 : x2 - x1;
        float dy = (y1 > y2) ? y1 - y2 : y2 - y1;
        return dx + dy;
    };

    std::vector<Node*> openList;

    Node* start = new Node{ startX, startY, 0.0f, heuristic(startX, startY, targetTileX, targetTileY)};
    openList.push_back(start);
    allNodes[startY][startX] = start;

    Node* endNode = nullptr;

    while (!openList.empty()) {
        // get the node with the lowest f cost
        auto it = std::min_element(openList.begin(), openList.end(), [](Node* a, Node* b){return (a->gCost + a->hCost) < (b->gCost + b->hCost);});
        Node* current = *it;
        openList.erase(it);

        if (current->x == targetTileX && current->y == targetTileY) {
            endNode = current;
            break;
        }

        closed[current->y][current->x] = true;

        // check the neighbors
        const int dirs[4][2] = {{0,-1},{0,1},{-1,0},{1,0}};

        for (auto& d : dirs) {
            int nx = current->x + d[0];
            int ny = current->y + d[1];

            if (nx < 0 || ny < 0 || ny >= (int)worldCollisionLayer.size() || nx >= (int)worldCollisionLayer[0].size())
                continue;
            if (worldCollisionLayer[ny][nx].id != -1 || closed[ny][nx])
                continue;

            float gNew = current->gCost + 1.0f;
            Node* neighbor;

            if (allNodes[ny][nx] == nullptr) {
                neighbor = new Node{nx, ny, gNew, heuristic(nx, ny, targetTileX, targetTileY), current};
                allNodes[ny][nx] = neighbor;
                openList.push_back(neighbor);
            } else {
                neighbor = allNodes[ny][nx];
                if (gNew < neighbor->gCost) {
                    neighbor->gCost = gNew;
                    neighbor->parent = current;
                }
            } 
        }
    }

    // Reconstruct the path
    path.clear();
    if (endNode) {
        Node* n = endNode;
        while (n != nullptr && !(n->x == startX && n->y == startY)) {
            path.insert(path.begin(), Vector2{(float)n->x, (float)n->y});
            n = n->parent;
        }
    }

    // cleanup
    for (auto& row : allNodes)
        for (auto& n : row)
            delete n;
}

void Enemy::followPath(float deltaTime, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer) {

}

void Enemy::debugPathDraw() {
    for (auto& tile : path) {
        Rectangle tileRect = { tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };
        DrawRectangleLinesEx(tileRect, 2, RED); // Draw red outline of path
    }
}

void Enemy::debugSeePlayerDraw() {
// Draw detection radius
    Vector2 center = { xPos + m_collisionRect.width / 2, yPos + m_collisionRect.height / 2 };
    DrawCircleLines(center.x, center.y, m_playerDistance, RED); // outline
    DrawCircle(center.x, center.y, 2.0f, YELLOW); // enemy center
}
