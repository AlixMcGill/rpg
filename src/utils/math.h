#pragma once
#include "project.h"

float distance(float x1, float y1, float x2, float y2);

float lerp(float current, float target, float speed);

float lerpDelta(float current, float target, float deltaTime, float speed);

// returns true if a is withing the range of b
bool inRange(float a, float b, float range);
