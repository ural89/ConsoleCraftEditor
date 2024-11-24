#include "BehaviorTreeSerializer.h"
#include <vector>

bool BehaviorTreeSerializer::Deserialize(const std::string &json, std::vector<BehaviorTreeNodeData *> &behaviorTreeNodeDatas)
{
    std::ifstream file(json);
    if (!file.is_open())
    {
        std::cout << "Failed to open behavior tree node data: " << json << '\n';
        return false;
    }

    std::string fileContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

    file.close();
    Document doc;
    doc.Parse(fileContent.c_str());

    if (!doc.IsArray())
    {
        std::cout << "File data is not an array " + fileContent << '\n';
        return false;
    }

    for (SizeType i = 0; i < doc.Size(); i++)
    {
        const Value &obj = doc[i];

        BehaviorTreeNodeData *behaviorTreeNodeData = new BehaviorTreeNodeData();

        behaviorTreeNodeData->id = obj["id"].GetInt();
        behaviorTreeNodeData->name = obj["name"].GetString();
        behaviorTreeNodeData->script = obj["script"].GetString();
        // position
        const Value &pos = obj["position"];
        if (pos.IsArray() && pos.Size() == 2)
        {
            behaviorTreeNodeData->position.X = pos[0].GetFloat();
            behaviorTreeNodeData->position.Y = pos[1].GetFloat();
        }

        behaviorTreeNodeData->parentId = obj["parentId"].GetInt();
        behaviorTreeNodeDatas.push_back(behaviorTreeNodeData);
    }
    return true;
}