#include "SceneDataDeserializer.h"
#include <iostream>
#include "../../../Vendor/rapidjson/include/rapidjson/document.h"
#include "../../../Vendor/rapidjson/include/rapidjson/writer.h"
#include "../../../Vendor/rapidjson/include/rapidjson/stringbuffer.h"
using namespace rapidjson;
bool SceneDataDeserializer::Deserialize(const std::string &filePath, SceneData &sceneData) // TODO: fix duplication of this method 
{                                                                                           //Unfortunatly I cant fix. One is needed for engine one for editor

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
        gameObjectData->id = obj["id"].GetInt();
        gameObjectData->name = obj["name"].GetString();

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
        // parent
        gameObjectData->parentId = obj["parentId"].GetInt();

        // isTemplate
        gameObjectData->isTemplate = obj["isTemplate"].GetBool();

        gameObjectsDatas.push_back(gameObjectData);
    }

    sceneData.GameObjectDatas = gameObjectsDatas;

    return true;
}
