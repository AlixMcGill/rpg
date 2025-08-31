#pragma once
#include <raylib.h>
#include <iostream>
#include <ostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <cstdlib>

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
#define PLAYER_TILE_WIDTH 64
#define PLAYER_TILE_HEIGHT 64
#define WORLD_WIDTH 20
#define WORLD_HEIGHT 20

#define DEVTOOLS true

extern int windowWidth;
extern int windowHeight;

struct DamageText {
    Vector2 position;
    std::string text;
    float timer;
    float duration;
    Color color;
    float speedY;
};

void setCollider(float width, float height, float offsetX, float offsetY, Rectangle& colliderRect);

// Dice rolls
int rollD20();
int rollD10();
