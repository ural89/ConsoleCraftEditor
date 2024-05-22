#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/document.h" //TODO: fix
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/writer.h"
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/stringbuffer.h"
#include "../../../Engine/Source/Core/DataStructs.h"
#include "FileHandler.h"
using namespace rapidjson;

class SceneDataSerializer
{
public:
    SceneDataSerializer(){};
    std::string Serialize(const SceneData &sceneData, const std::string& projectName);
    bool Deserialize(const std::string &json, SceneData &SceneData);
    bool CreateSceneScript(const std::string &sceneName, const std::string &projectName);
    void AddSceneScriptToJson(const std::string &sceneScriptName, const std::string &projectName);
    static std::vector<std::string> GetScenesListFromJson(const std::string &projectName);


private:
    bool CreateHeaderFileAndFill(const std::string &sceneName, const std::string &projectName);
    bool CreateCPPFileAndFill(const std::string &sceneName, const std::string &projectName);
    FileHandler fileHandler;
};
