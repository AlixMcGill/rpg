#pragma once
#include "../utils/project.h"
#include "../game/enemy.h"
#include <raylib.h>

class Skeleton : public Enemy {
public:
    Skeleton(int startX, int startY, Texture& textrue);
    void update(float deltaTime, float& playerXPos, 
            float& playerYPos, 
            std::vector<std::vector<sTile>>& collisionLayer, 
            std::vector<DamageText>& damageTexts,
            Player& player) override;
private:
    //void m_stateHandling(float& playerXPos, float& playerYPos, const std::vector<std::vector<sTile>>& worldCollisionLayer) override;
    //void m_stateCheck(float& deltaTime, float& moveY, float& moveX) override;

};
