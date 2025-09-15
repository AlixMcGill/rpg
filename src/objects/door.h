#pragma once
#include "../utils/project.h"
#include "../game/object.h"

class Door: public Object {
public:
    void update() override;
    void draw() override;
};
