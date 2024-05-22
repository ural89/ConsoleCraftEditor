#pragma once
#include "Core/../Core.h"
#include "Core/GameObject.h"
#include "Core/Component/RigidbodyComponent.h"
#include "Core/Component/SpringComponent.h"
class GE_API PolygonParticle : public GameObject
{
public:
    PolygonParticle(class Scene &scene) : GameObject("PolygonParticle", scene)
    {
        sprite = {{1}};
        symbol = "X";
    }
    virtual ~PolygonParticle() {}
    void Init() override;
    bool isFixed = false;
    void OnReady();
};