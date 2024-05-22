#pragma once
#include "FileHandler.h"
#include "Core/Scripting/ScriptRegistry.h"
#include "Directories.h"
#include "../ProjectManager.h"
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/document.h"
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/writer.h"
#include "../../../Engine/Vendor/rapidjson/include/rapidjson/stringbuffer.h"

using namespace rapidjson;

class ComponentScriptCreator
{
public:
    bool CreateComponentScript(std::string componentName, std::string projectName)
    {
        if (!fileHandler.CreateDirectory(ProjectManager::PROJECTS_DIR + projectName + COMPONENTS_DIR))
        {
            std::cout << "Failed to create Components directory: " << componentName << std::endl;
        }
        if (!CreateHeaderFileAndFill(componentName, projectName))
        {
            std::cout << "Failed to create header file \n";
            return false;
        }
        if (!CreateCPPFileAndFill(componentName, projectName))
        {
            std::cout << "Failed to create CPP file \n";
            return false;
        }
        if (!AddRegistryIncludeLines(componentName, projectName))
        {
            std::cout << "Failed to add include line \n";
            return false;
        }
        AddComponentScriptToJson(componentName, projectName);
        return true;
    }
    static std::vector<std::string> GetComponentScripts(const std::string &projectName)
    {
        std::vector<std::string> scriptNames;

        std::string componentScriptsFilePath = ProjectManager::PROJECTS_DIR + projectName + "/ComponentScripts.json";

        if (!std::filesystem::exists(componentScriptsFilePath))
        {

            return scriptNames;
        }

        std::ifstream componentScriptsFile(componentScriptsFilePath);
        if (!componentScriptsFile.is_open())
        {
            std::cerr << "Failed to open GameObjectScripts.json for reading." << std::endl;
            return scriptNames;
        }

        std::stringstream buffer;
        buffer << componentScriptsFile.rdbuf();
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
    void AddComponentScriptToJson(const std::string &componentScriptName, const std::string &projectName)
    {

        std::string componentScriptsFilePath = ProjectManager::PROJECTS_DIR + projectName + "/ComponentScripts.json";

        Document scriptsArray;
        if (std::filesystem::exists(componentScriptsFilePath))
        {

            std::ifstream componentScriptsFile(componentScriptsFilePath);
            std::stringstream buffer;
            buffer << componentScriptsFile.rdbuf();
            std::string content = buffer.str();
            scriptsArray.Parse(content.c_str());
            componentScriptsFile.close();
        }
        else
        {
            scriptsArray.SetArray();
        }

        Value scriptName;
        scriptName.SetString(componentScriptName.c_str(), componentScriptName.length(), scriptsArray.GetAllocator());
        scriptsArray.PushBack(scriptName, scriptsArray.GetAllocator());

        std::ofstream componentScriptsFile(componentScriptsFilePath);
        if (componentScriptsFile.is_open())
        {
            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            scriptsArray.Accept(writer);
            componentScriptsFile << buffer.GetString();
            componentScriptsFile.close();
        }
        else
        {
            std::cerr << "Failed to open ComponentsScripts.json for writing." << std::endl;
        }
    }

    bool CreateHeaderFileAndFill(const std::string &componentName, const std::string &projectName)
    {
        std::string headerFilePath = ProjectManager::PROJECTS_DIR + projectName + COMPONENTS_DIR + "/" + componentName + ".h";

        if (std::filesystem::exists(headerFilePath))
        {
            std::cout << "Header file already exists.\n";
            return false;
        }

        std::ifstream templateFile("../Source/Core/FileHandling/Templates/ComponentHeaderTemplate.txt");
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
        fileHandler.ReplacePlaceholders(templateFile, headerFile, "{ComponentName}", componentName);
        templateFile.close();
        headerFile.close();
        std::cout << "Successfully created header! \n";
        return true;
    }
    bool CreateCPPFileAndFill(const std::string &componentName, const std::string &projectName)
    {
        std::string cppFilePath = ProjectManager::PROJECTS_DIR + projectName + COMPONENTS_DIR + "/" + componentName + ".cpp";

        if (std::filesystem::exists(cppFilePath))
        {
            std::cout << "CPP file already exists.\n";
            return false;
        }

        std::ifstream templateFile("../Source/Core/FileHandling/Templates/ComponentcppTemplate.txt");
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
        fileHandler.ReplacePlaceholders(templateFile, cppFile, "{ComponentName}", componentName);
        templateFile.close();
        cppFile.close();
        std::cout << "Successfully created cpp! \n";
        return true;
    }
    bool AddRegistryIncludeLines(const std::string &component, const std::string &projectName)
    {

        const std::string includeGameObjectFilePath = ProjectManager::PROJECTS_DIR + projectName + "/Source/" + "IncludeComponents.h";
        if (!std::filesystem::exists(includeGameObjectFilePath))
        {
            std::ofstream includeFileGameObjectFile(includeGameObjectFilePath);
        }

        fileHandler.AddLineToFile(includeGameObjectFilePath, "#include \"Components/" + component + ".h\"");
        std::cout << "WRITING COMPONENT include file" << '\n';

        const std::string includeRegistryObjectListFilePath = ProjectManager::PROJECTS_DIR + projectName + "/Source/" + "IncludeRegisterComponentList.h";
        if (!std::filesystem::exists(includeRegistryObjectListFilePath))
        {
            std::ofstream includeRegistryObjectListFile(includeRegistryObjectListFilePath);
        }
        fileHandler.AddLineToFile(includeRegistryObjectListFilePath,
                                  "Register(\"" + component + "\", [&ownerGameObject]() { return new " + component + "(ownerGameObject); });");

        return true;
    }

private:
    FileHandler fileHandler;
};