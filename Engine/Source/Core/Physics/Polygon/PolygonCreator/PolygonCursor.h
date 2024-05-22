#pragma once
#include "Core/GameObject.h"
#include "Core/../Core.h"
class GE_API Cursor : public GameObject
{
public:
    Cursor(class Scene &scene);
    ~Cursor(){}

    void Init() override;
private:

};