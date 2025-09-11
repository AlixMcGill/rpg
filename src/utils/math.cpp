#include "math.h"

float distance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

float lerp(float current, float target, float speed) {
    return current + (target - current) * speed;
}

float lerpDelta(float current, float target, float deltaTime, float speed) {
    float t = 1.0f - std::exp(-speed * deltaTime);
    return lerp(current, target, t);
}

bool inRange(float a, float b, float range) {
    return std::fabs(a - b) <= range;
}
