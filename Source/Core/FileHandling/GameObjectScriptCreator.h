#pragma once
#include "FileHandler.h"
#include "Core/Scripting/ScriptRegistry.h"
#include "Directories.h"
#include "../ProjectManager.h"
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/document.h"
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/writer.h"
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/stringbuffer.h"

using namespace rapidjson;
class GameObjectScriptCreator
{
public:
    bool CreateScript(const std::string &gameObjectName, const std::string &projectName)
    {

        if (!fileHandler.CreateDirectory(ProjectManager::PROJECTS_DIR + projectName + GAMEOBJECTS_DIR))
        {
            std::cout << "Failed to create GameObjects directory: " << gameObjectName << std::endl;
        }
        if (!CreateHeaderFileAndFill(gameObjectName, projectName))
        {
            std::cout << "Failed to create header file \n";
            return false;
        }
        if (!CreateCPPFileAndFill(gameObjectName, projectName))
        {
            std::cout << "Failed to create CPP file \n";
            return false;
        }
        if (!AddRegistryIncludeLines(gameObjectName, projectName))
        {
            std::cout << "Failed to add include line \n";
            return false;
        }
        AddGameObjectScriptToJson(gameObjectName, projectName);
        return true;
    }
    static std::vector<std::string> GetGameObjectScripts(const std::string &projectName)
    {
        std::vector<std::string> scriptNames;

        std::string gameObjectScriptsFilePath = ProjectManager::PROJECTS_DIR + projectName + "/GameObjectScripts.json";

        if (!std::filesystem::exists(gameObjectScriptsFilePath))
        {
            std::cerr << "GameObjectScripts.json does not exist for project: " << projectName << std::endl;
            return scriptNames;
        }

        std::ifstream gameObjectScriptsFile(gameObjectScriptsFilePath);
        if (!gameObjectScriptsFile.is_open())
        {
            std::cerr << "Failed to open GameObjectScripts.json for reading." << std::endl;
            return scriptNames;
        }

        std::stringstream buffer;
        buffer << gameObjectScriptsFile.rdbuf();
        std::string content = buffer.str();
        Document scriptsArray;
        scriptsArray.Parse(content.c_str());

        if (scriptsArray.HasParseError() || !scriptsArray.IsArray())
        {
            std::cerr << "Failed to parse GameObjectScripts.json." << std::endl;
            return scriptNames;
        }

        for (SizeType i = 0; i < scriptsArray.Size(); ++i)
        {
            if (scriptsArray[i].IsString())
            {
                scriptNames.push_back(scriptsArray[i].GetString());
            }
        }

        return scriptNames;
    }
 

private:
    void AddGameObjectScriptToJson(const std::string &gameObjectScriptName, const std::string &projectName)
    {

        std::string gameObjectScriptsFilePath = ProjectManager::PROJECTS_DIR + projectName + "/GameObjectScripts.json";

        Document scriptsArray;
        if (std::filesystem::exists(gameObjectScriptsFilePath))
        {

            std::ifstream gameObjectScriptsFile(gameObjectScriptsFilePath);
            std::stringstream buffer;
            buffer << gameObjectScriptsFile.rdbuf();
            std::string content = buffer.str();
            scriptsArray.Parse(content.c_str());
            gameObjectScriptsFile.close();
        }
        else
        {
            scriptsArray.SetArray();
        }

        Value scriptName;
        scriptName.SetString(gameObjectScriptName.c_str(), gameObjectScriptName.length(), scriptsArray.GetAllocator());
        scriptsArray.PushBack(scriptName, scriptsArray.GetAllocator());

        std::ofstream gameObjectScriptsFile(gameObjectScriptsFilePath);
        if (gameObjectScriptsFile.is_open())
        {
            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            scriptsArray.Accept(writer);
            gameObjectScriptsFile << buffer.GetString();
            gameObjectScriptsFile.close();
        }
        else
        {
            std::cerr << "Failed to open GameObjectScripts.json for writing." << std::endl;
        }
    }

    bool CreateHeaderFileAndFill(const std::string &gameObjectName, const std::string &projectName)
    {
        std::string headerFilePath = ProjectManager::PROJECTS_DIR + projectName + GAMEOBJECTS_DIR + "/" + gameObjectName + ".h";

        if (std::filesystem::exists(headerFilePath))
        {
            std::cout << "Header file already exists.\n";
            return false;
        }

        std::ifstream templateFile("../Source/Core/FileHandling/Templates/GameObjectHeaderTemplate.txt");
        if (!templateFile.is_open())
        {
            std::cout << "Failed to open template file.\n";
            return false;
        }

        std::ofstream headerFile(headerFilePath);
        if (!headerFile.is_open())
        {
            std::cout << "Failed to create header file.\n";
            templateFile.close();
            return false;
        }
        fileHandler.ReplacePlaceholders(templateFile, headerFile, "{GameObjectName}", gameObjectName);
        templateFile.close();
        headerFile.close();
        std::cout << "Successfully created header! \n";
        return true;
    }
    bool CreateCPPFileAndFill(const std::string &gameObjectName, const std::string &projectName)
    {
        std::string cppFilePath = ProjectManager::PROJECTS_DIR + projectName + GAMEOBJECTS_DIR + "/" + gameObjectName + ".cpp";

        if (std::filesystem::exists(cppFilePath))
        {
            std::cout << "CPP file already exists.\n";
            return false;
        }

        std::ifstream templateFile("../Source/Core/FileHandling/Templates/GameObjectcppTemplate.txt");
        if (!templateFile.is_open())
        {
            std::cout << "Failed to open template file.\n";
            return false;
        }

        std::ofstream cppFile(cppFilePath);
        if (!cppFile.is_open())
        {
            std::cout << "Failed to create cpp file.\n";
            templateFile.close();
            return false;
        }
        else
        {
            fileHandler.OpenVisualStudioCode(cppFilePath);
        }
        fileHandler.ReplacePlaceholders(templateFile, cppFile, "{GameObjectName}", gameObjectName);
        templateFile.close();
        cppFile.close();
        std::cout << "Successfully created cpp! \n";
        return true;
    }
    bool AddRegistryIncludeLines(const std::string &gameObjectName, const std::string &projectName)
    {

        const std::string includeGameObjectFilePath = ProjectManager::PROJECTS_DIR + projectName + "/Source/" + "IncludeGameObjects.h"; // "../Engine/Source/Core/Scripting/IncludeGameObjectList.h";
        if (!std::filesystem::exists(includeGameObjectFilePath))
        {
            std::ofstream includeFileGameObjectFile(includeGameObjectFilePath);
            std::cout << "creating include file" << '\n';
        }

        fileHandler.AddLineToFile(includeGameObjectFilePath, "#include \"GameObjects/" + gameObjectName + ".h\"");

        const std::string includeRegistryObjectListFilePath = ProjectManager::PROJECTS_DIR + projectName + "/Source/" + "IncludeRegisterObjectList.h"; //"../Engine/Source/Core/Scripting/IncludeRegisterObjectList.h";
        if (!std::filesystem::exists(includeRegistryObjectListFilePath))
        {
            std::ofstream includeRegistryObjectListFile(includeRegistryObjectListFilePath);
        }
        fileHandler.AddLineToFile(includeRegistryObjectListFilePath, "Register(\"" + gameObjectName + "\", [&scene](){ return new " + gameObjectName + "(scene); });");

        return true;
    }

private:
    FileHandler fileHandler;
};
