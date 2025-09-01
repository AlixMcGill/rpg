#pragma once
#include "../utils/project.h"
#include "entity.h"
#include "tilemap.h"
#include "enemyController.h"
#include <raylib.h>

class Player : public Entity {
public:
    bool renderDebug = false;

    float stamina;
    float health = 100.0f;
    float maxHealth = 100.0f;

    float healthRegen = 0.01f;

    bool isAnimating = false; // used for atttack animations / state changes prob should change later
    bool canAttack = true; // flag to stop multiple attacks from rendering off of one hit 

    enum state {
        IDLE,
        IDLE_UP,
        IDLE_DOWN,
        IDLE_LEFT,
        IDLE_RIGHT,
        WALK_UP,
        WALK_DOWN,
        WALK_LEFT,
        WALK_RIGHT,
        ATTACK_UP,
        ATTACK_DOWN,
        ATTACK_LEFT,
        ATTACK_RIGHT,
        RUN
    };

    state currentState;

    Rectangle hitBoxCollider;
    Rectangle attackCollider;

    void init();
    void update(float deltaTime,
                const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer,
                std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw();
    void destroy();

    bool isColliding(const Rectangle& playerBounds, const std::vector<std::vector<Tilemap::sTile>>& worldCollisionLayer);

    void damagePlayer(float damage);
private:
    Texture2D m_playerTexture;
    int m_playerTileX;
    int m_playerTileY;

    float m_frameTimer = 0.0f;
    float m_frameTime = 0.10f;
    float m_defaultFrameTime = 0.10f;

    float m_speed;
    float m_accel = 800.0f;
    float m_drag = 1600.0f;

    float m_walkSpeed = 80.0f;
    float m_runSpeed = 130.0f;
    float m_maxStamina = 100.0f;

    void m_setStartPos(int x, int y);
    void m_loadPlayerTexture(const char* imgPath);
    Rectangle m_getCollisionBounds(float futureX, float futureY) const;
    Rectangle m_getHitboxBounds(float futureX, float futureY, Rectangle& hitbox) const;
    std::string m_getMouseDirection();
    void m_regenHealth();
    void m_killPlayer();

    void m_setHitboxCollider(float width, float height, float baseOffsetX, float baseOffsetY);
    void m_setAttackCollider(float width, float height, float baseOffsetX, float baseOffsetY);

    void m_updateAttack(state AttackState,std::vector<std::unique_ptr<Enemy>>& enemies);

    float m_getMeleeDamage(float distance);

    std::vector<DamageText> damageTexts;
    void m_updateDamageTexts(float deltaTime);
};
