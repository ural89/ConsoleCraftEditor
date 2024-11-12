#pragma once
#include <iostream>
#include <fstream>
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/document.h" //TODO: fix
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/writer.h"
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/stringbuffer.h"
#include "../../../Engine/Source/Core/DataStructs.h"
#include "FileHandler.h"

using namespace rapidjson;

class BehaviorTreeSerializer
{
public:
    BehaviorTreeSerializer(){};
    bool Deserialize(const std::string &json, std::vector<BehaviorTreeNodeData*> &behaviorTreeNodeData);
};