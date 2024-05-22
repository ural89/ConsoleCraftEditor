#include "SceneDataSerializer.h"
#include "CoreStructs/Vector.h"
#include "GameObjectScriptCreator.h"
#include "../ProjectManager.h"
#include <vector>
std::string SceneDataSerializer::Serialize(const SceneData &sceneData, const std::string &projectName)
{
    const std::vector<GameObjectData *> gameObjectDatas = sceneData.GameObjectDatas;
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    writer.StartArray();

    for (const auto &obj : gameObjectDatas)
    {
        writer.StartObject();
        
        writer.String("id");
        writer.Int(obj->id);

        writer.String("name");
        writer.String(obj->name.c_str());
        
        writer.String("script");
        if (obj->script == "None") // if no gameobject script get first ever created script
        {
            auto gameObjectSripts = GameObjectScriptCreator::GetGameObjectScripts(projectName);
            if (gameObjectSripts.size())
                obj->script = gameObjectSripts[0];
        }
        writer.String(obj->script.c_str());

        // pos
        writer.String("position");
        writer.StartArray();

        writer.Double(obj->position.X);
        writer.Double(obj->position.Y);

        writer.EndArray();

        // sprite
        int spriteWidth = obj->spriteWidth;

        writer.String("sprite");
        writer.StartArray();

        for (size_t i = 0; i < obj->sprite.size(); i += spriteWidth) // convert 1D vector to 2D array
        {
            writer.StartArray();
            for (size_t j = 0; j < spriteWidth; ++j)
            {
                writer.Int(obj->sprite[i + j]);
            }
            writer.EndArray();
        }

        writer.EndArray();

        writer.String("symbol");
        writer.String(obj->symbol.c_str());
        writer.String("spriteWidth");
        writer.Int(obj->spriteWidth);

        // components
        writer.String("components");
        writer.StartArray();
        
        for (size_t i = 0; i < obj->components.size(); i++)
        {
            writer.String(obj->components[i].name.c_str());
        }
        writer.EndArray();
        


        //parent
        writer.String("parentId");
        writer.Int(obj->parentId);

        //istemplate
        writer.String("isTemplate");
        writer.Bool(obj->isTemplate);

        writer.EndObject();
    }

    writer.EndArray();

    return buffer.GetString();
}

bool SceneDataSerializer::Deserialize(const std::string &filePath, SceneData &sceneData) //TODO: fix duplication of this method
{

    std::vector<GameObjectData *> gameObjectsDatas;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    std::string fileContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
    file.close();

    Document doc;
    doc.Parse(fileContent.c_str());

    if (!doc.IsArray())
    {
        std::cout << "File data is not an array " + fileContent << std::endl;
        return false;
    }

    for (SizeType i = 0; i < doc.Size(); i++)
    {
        const Value &obj = doc[i];

        GameObjectData *gameObjectData = new GameObjectData();

        gameObjectData->name = obj["name"].GetString();
        gameObjectData->id = obj["id"].GetInt();
        gameObjectData->script = obj["script"].GetString();
        // position
        const Value &pos = obj["position"];
        if (pos.IsArray() && pos.Size() == 2)
        {
            gameObjectData->position.X = pos[0].GetFloat();
            gameObjectData->position.Y = pos[1].GetFloat();
        }
        gameObjectData->symbol = obj["symbol"].GetString();
        // sprite
        const Value &spriteArray = obj["sprite"];
        if (spriteArray.IsArray())
        {
            gameObjectData->sprite.clear();
            for (SizeType j = 0; j < spriteArray.Size(); j++)
            {
                for (SizeType i = 0; i < spriteArray[j].Size(); i++)
                {

                    gameObjectData->sprite.push_back(spriteArray[j][i].GetInt());
                }
            }
        }

        gameObjectData->spriteWidth = obj["spriteWidth"].GetInt();

        // components
        const Value &componentsArray = obj["components"];
        if (componentsArray.IsArray())
        {

            gameObjectData->components.clear();
            for (SizeType i = 0; i < componentsArray.Size(); i++)
            {

                ComponentData componentData{componentsArray[i].GetString()};
                gameObjectData->components.push_back(componentData);
            }
        }
        //parent
        gameObjectData->parentId = obj["parentId"].GetInt();
        //isTemplate
        gameObjectData->isTemplate = obj["isTemplate"].GetBool();


        gameObjectsDatas.push_back(gameObjectData);
    }

    sceneData.GameObjectDatas = gameObjectsDatas;

    return true;
}
bool SceneDataSerializer::CreateSceneScript(const std::string &sceneName, const std::string &projectName)
{

    fileHandler.CreateDirectory(ProjectManager::PROJECTS_DIR + projectName + SCENE_SCRIPTS_DIR);
    AddSceneScriptToJson(sceneName, projectName);
    CreateHeaderFileAndFill(sceneName, projectName);
    CreateCPPFileAndFill(sceneName, projectName);

    return false;
}
bool SceneDataSerializer::CreateHeaderFileAndFill(const std::string &sceneName, const std::string &projectName)
{
    std::string headerFilePath = ProjectManager::PROJECTS_DIR + projectName + SCENE_SCRIPTS_DIR + "/" + sceneName + ".h";

    if (std::filesystem::exists(headerFilePath))
    {
        std::cout << "Header file already exists.\n";
        return false;
    }

    std::ifstream templateFile(SCENE_HEADER_TEMPLATE_DIR);
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
    fileHandler.ReplacePlaceholders(templateFile, headerFile, "{SceneName}", sceneName);
    templateFile.close();
    headerFile.close();
    std::cout << "Successfully created header! \n";
    return true;
}
void SceneDataSerializer::AddSceneScriptToJson(const std::string &sceneScriptName, const std::string &projectName)
{
    std::string gameObjectScriptsFilePath = ProjectManager::PROJECTS_DIR + projectName + "/SceneScripts.json";

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

    // Check if the script name already exists in the array
    bool scriptExists = false;
    for (auto &script : scriptsArray.GetArray())
    {
        if (script.IsString() && script.GetString() == sceneScriptName)
        {
            scriptExists = true;
            break;
        }
    }

    if (!scriptExists)
    {
        Value scriptName;
        scriptName.SetString(sceneScriptName.c_str(), sceneScriptName.length(), scriptsArray.GetAllocator());
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
    else
    {
        // std::cerr << "Scene script with name '" << sceneScriptName << "' already exists." << std::endl;
    }
}

std::vector<std::string> SceneDataSerializer::GetScenesListFromJson(const std::string &projectName)
{
    std::vector<std::string> scriptNames;

    std::string gameObjectScriptsFilePath = ProjectManager::PROJECTS_DIR + projectName + "/GameObjectScripts.json";

    if (!std::filesystem::exists(gameObjectScriptsFilePath))
    {
        //  std::cerr << "GameObjectScripts.json does not exist for project: " << projectName << std::endl;
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

bool SceneDataSerializer::CreateCPPFileAndFill(const std::string &sceneName, const std::string &projectName)
{
    std::string cppFilePath = ProjectManager::PROJECTS_DIR + projectName + SCENE_SCRIPTS_DIR + "/" + sceneName + ".cpp";

    if (std::filesystem::exists(cppFilePath))
    {
        std::cout << "CPP file already exists.\n";
        return false;
    }

    std::ifstream templateFile(SCENE_CPP_TEMPLATE_DIR);
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
    const std::string placeholders[] = {"{ProjectName}", "{SceneName}"};
    const std::string values[] = {projectName, sceneName};
    const size_t size = sizeof(placeholders) / sizeof(placeholders[0]);
    fileHandler.ReplacePlaceholders(templateFile, cppFile, placeholders, values, size);
    // fileHandler.ReplacePlaceholders(templateFile, cppFile, "{SceneName}", sceneName);
    templateFile.close();
    cppFile.close();
    std::cout << "Successfully created cpp! \n";
    return true;
}