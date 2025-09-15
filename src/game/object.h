#pragma once
#include "../utils/project.h"
#include <raylib.h>
#include <vector>

class Object {
public:

    virtual void initObject(float tileX, float tileY, Texture2D* textrueAtlas, std::vector<std::vector<sTile>>* collisionLayer);
    virtual void update();
    virtual void draw();
    //void destroyObject();

    sTile getObjPosition();
    const sTile getObjPosition() const;

protected:
    sTile objPosition;
    Texture2D* textureAtlas = nullptr;
    std::vector<std::vector<sTile>>* collisionLayer;

    /*
     * choose wether to activate or deactivate the collision tile
     * True = collision tile is set
     * False = remove the collision tile if its not set
     * */
    void updateCollisionTile(bool activateTile);
private:

};
