#pragma once
#include "Core/GameObject.h"
#include "Core/../Core.h"
class GE_API LineParticle : public GameObject
{
public:
    LineParticle(class Scene &scene) : GameObject("LineParticle", scene)
    {
        hasCollider = false;
    }
    void Init() override
    {
        sprite = {{1}};
        symbol = "o";
    }
    bool isFixed = false;
};