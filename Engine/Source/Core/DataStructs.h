#pragma once
#include <vector>
#include "../CoreStructs/Vector.h"


struct ComponentData
{
    std::string name;
};
struct GameObjectData
{
    int id = 0;
    std::string name;
    std::string script;
    Vector2 position;
    std::string symbol;
    int spriteWidth;
    std::vector<int> sprite;
    std::vector<ComponentData> components;
    int parentId= -1;
    bool isTemplate = false;

};

struct SceneData
{
    std::vector<GameObjectData*> GameObjectDatas; 
};