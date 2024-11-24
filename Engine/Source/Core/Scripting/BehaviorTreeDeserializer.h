#pragma once
#include <iostream>
#include <fstream>
#include "../../../Vendor/rapidjson/include/rapidjson/document.h"
#include "../../../Vendor/rapidjson/include/rapidjson/writer.h"
#include "../../../Vendor/rapidjson/include/rapidjson/stringbuffer.h"
#include "../DataStructs.h"
// #include "FileHandler.h"

using namespace rapidjson;

class BehaviorTreeDeserializer
{
public:
    BehaviorTreeDeserializer(){};
    bool Deserialize(const std::string &json, std::vector<BehaviorTreeNodeData*> &behaviorTreeNodeData);
};