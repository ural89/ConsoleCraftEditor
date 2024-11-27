#include "InEditorRenderer.h"
#include <iostream>
#include <vector>
const int TERMINAL_CHAR_HEIGHT = 8;
const int TERMINAL_CHAR_WIDTH = 8;

InEditorRenderer::InEditorRenderer()
{
}
void InEditorRenderer::Render(SceneData &sceneData)
{
    gameViewPosition = ImGui::GetCursorScreenPos();
    RenderGamObjectProperties(sceneData);
    RenderHierarchy(sceneData);
    RenderBehaviorTree(sceneData);

    int cameraOffsetX = 0;
    int cameraOffsetY = 0;

    RenderTemplates(sceneData);
    if (sceneData.GameObjectDatas.size() > 0)
    {
        for (auto it = sceneData.GameObjectDatas.begin(); it != sceneData.GameObjectDatas.end(); ++it)
        {
            if ((*it)->isTemplate)
            {
                continue;
            }
            if ((*it) == nullptr)
                continue;
            const std::vector<int> &sprite = (*it)->sprite;
            const int spriteWidth = (*it)->spriteWidth;
            const std::string symbol = (*it)->symbol;
            int goPositionX = (*it)->position.X + xOffset;
            int goPositionY = (*it)->position.Y;

            if (*it == selectedGameObjectData) // draw bounding box on selected object
            {
                int boundingBoxWidth = spriteWidth * TERMINAL_CHAR_WIDTH;
                int boundingBoxHeight = (sprite.size() / spriteWidth) * TERMINAL_CHAR_HEIGHT + TERMINAL_CHAR_HEIGHT;
                ImVec2 topLeft(goPositionX * TERMINAL_CHAR_WIDTH + gameViewPosition.x + cameraOffsetX,
                               goPositionY * TERMINAL_CHAR_HEIGHT + gameViewPosition.y + cameraOffsetY);
                ImVec2 bottomRight(topLeft.x + boundingBoxWidth, topLeft.y + boundingBoxHeight);

                ImGui::GetWindowDrawList()->AddRect(topLeft, bottomRight, ImColor(1.0f, 1.0f, 1.0f, 0.5f), 0.0f, 0, 2.0f);
            }

            for (int i = 0; i < sprite.size(); i++)
            {
                int color = sprite[i];
                ImVec4 textColor;
                switch (color)
                {
                case 5:
                    textColor = ImVec4(0.6f, 0.1f, 0.9f, 1.0f);
                    break;
                case 4:
                    textColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
                    break;
                case 3:
                    textColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                    break;
                case 2:
                    textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                    break;
                case 1:
                    textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                    break;
                case 0:
                    textColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
                    break;
                default:
                    textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    break;
                }
                int posX = goPositionX * TERMINAL_CHAR_HEIGHT + (i % spriteWidth) * TERMINAL_CHAR_HEIGHT + cameraOffsetX;
                int posY = goPositionY * TERMINAL_CHAR_WIDTH + (i / spriteWidth) * TERMINAL_CHAR_WIDTH + cameraOffsetY;
                ImVec2 charPos = ImVec2(posX + gameViewPosition.x, posY + gameViewPosition.y);
                ImGui::GetWindowDrawList()->AddText(charPos, ImColor(textColor), symbol.c_str());
            }
        }
    }
}

void InEditorRenderer::RenderTemplates(SceneData &sceneData)
{
    ImGui::Begin("Templates");

    for (size_t i = 0; i < sceneData.GameObjectDatas.size(); ++i)
    {
        GameObjectData *gameObjectData = sceneData.GameObjectDatas[i];

        if (gameObjectData && gameObjectData->isTemplate)
        {
            ImGui::Text("%s", gameObjectData->name.c_str());
            ImGui::SameLine();

            std::string buttonId = "AddToScene###Add" + gameObjectData->name;
            if (ImGui::Button(buttonId.c_str()))
            {
                gameObjectData->isTemplate = false;
            }
            ImGui::SameLine();
            std::string cloneButtonId = "CloneGameObject###Clone" + gameObjectData->name;
            if (ImGui::Button(cloneButtonId.c_str()))
            {
                GameObjectData *god = new GameObjectData{
                    0,
                    gameObjectData->name,
                    gameObjectData->script,
                    gameObjectData->position,
                    gameObjectData->symbol,
                    gameObjectData->spriteWidth,
                    gameObjectData->sprite,
                    gameObjectData->components};
                sceneData.GameObjectDatas.push_back(god);
                god->isTemplate = false;
            }
        }
    }

    ImGui::End();
}
void InEditorRenderer::RenderBehaviorTree(SceneData &sceneData)
{
    //TODO: behavior tree data will be retrieved from behavior tree component
    BehaviorTreeNodeData btNodeData;
    btNodeData.id = 0;
    btNodeData.name = "Selector";
    btNodeData.position = Vector2(125, 5);
    btNodeData.script = "";
    std::vector<BehaviorTreeNodeData*> btNodeDatas;
    btNodeDatas.push_back(&btNodeData);
    behaviorTreeRenderer.Render(btNodeDatas);
}
void InEditorRenderer::RenderGamObjectProperties(SceneData &sceneData)
{
    ImGui::Begin("GameObject");
    gameObjectPropertiesRenderer.Render();
    if (selectedGameObjectData != nullptr)
        if (ImGui::Button("Destory"))
        {
            DestroySelectedGameObject(sceneData);
        }
    ImGui::End();
}
void InEditorRenderer::RenderHierarchy(SceneData &sceneData)
{
    ImGui::Begin("HierarchyView");

    static GameObjectData *dragSourceGameObject = nullptr;

    std::function<void(GameObjectData *, int)> RenderGameObjectNode = [&](GameObjectData *gameObject, int indentLevel)
    {
        ImGui::Indent(indentLevel * 10.0f);
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;

        if (gameObject == selectedGameObjectData)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        std::string nodeId = gameObject->name + "##" + std::to_string(reinterpret_cast<uintptr_t>(gameObject));
        bool nodeOpen = ImGui::TreeNodeEx(nodeId.c_str(), flags);

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("GAMEOBJECT", &gameObject, sizeof(GameObjectData));
            dragSourceGameObject = gameObject;
            ImGui::Text("%s", gameObject->name.c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
            {
                GameObjectData *droppedGameObject = *(GameObjectData **)payload->Data;
                if (droppedGameObject != gameObject && droppedGameObject->parentId != gameObject->id)
                {
                    droppedGameObject->parentId = gameObject->id;
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::IsItemClicked())
        {
            selectedGameObjectData = gameObject;
            gameObjectPropertiesRenderer.SelectGameObject(gameObject);
        }

        if (nodeOpen)
        {

            for (const auto &child : sceneData.GameObjectDatas)
            {
                if (child->parentId == gameObject->id)
                {
                    RenderGameObjectNode(child, indentLevel + 1);
                }
            }
            ImGui::TreePop();
        }

        ImGui::Unindent(indentLevel * 10.0f);
    };

    for (const auto &gameObject : sceneData.GameObjectDatas)
    {
        if (gameObject->parentId == -1 && !gameObject->isTemplate)
        {
            RenderGameObjectNode(gameObject, 0);
        }
    }

    ImGui::End();
}

void InEditorRenderer::DestroySelectedGameObject(SceneData &sceneData)
{
    ClearSelection();
    auto it = std::find(sceneData.GameObjectDatas.begin(), sceneData.GameObjectDatas.end(), selectedGameObjectData);

    if (it != sceneData.GameObjectDatas.end())
    {
        delete *it;
        sceneData.GameObjectDatas.erase(it);
        selectedGameObjectData = nullptr;
    }
}
void InEditorRenderer::Update(SceneData &sceneData)
{
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

        accumulatedDeltaX += delta.x;
        accumulatedDeltaY += delta.y;

        int deltaX = static_cast<int>(accumulatedDeltaX / TERMINAL_CHAR_WIDTH);
        int deltaY = static_cast<int>(accumulatedDeltaY / TERMINAL_CHAR_HEIGHT);

        if (deltaX != 0 || deltaY != 0)
        {
            accumulatedDeltaX -= deltaX * TERMINAL_CHAR_WIDTH;
            accumulatedDeltaY -= deltaY * TERMINAL_CHAR_HEIGHT;

            if (selectedGameObjectData)
            {
                for (auto &gameObjectData : sceneData.GameObjectDatas)
                {
                    if (gameObjectData->parentId == selectedGameObjectData->id)
                    {
                        gameObjectData->position.X += deltaX;
                        gameObjectData->position.Y += deltaY;
                    }
                }
                selectedGameObjectData->position.X += deltaX;
                selectedGameObjectData->position.Y += deltaY;
            }
        }

        ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
    }
    else
    {
        bool buttonClicked = false;

        if (!ImGui::IsAnyItemActive() && !ImGui::IsAnyItemHovered())
        {
            buttonClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
            if (buttonClicked)
            {
                ImVec2 mousePos = ImGui::GetMousePos();
                mousePos.x -= gameViewPosition.x;
                mousePos.y -= gameViewPosition.y;

                // selectedGameObjectData = nullptr;
                // gameObjectPropertiesRenderer.SelectGameObject(nullptr);

                for (auto it = sceneData.GameObjectDatas.begin(); it != sceneData.GameObjectDatas.end(); ++it)
                {
                    if ((*it) == nullptr)
                        continue;
                    if (mousePos.x >= (*it)->position.X * TERMINAL_CHAR_WIDTH &&
                        mousePos.x <= ((*it)->position.X + (*it)->spriteWidth) * TERMINAL_CHAR_WIDTH &&
                        mousePos.y >= (*it)->position.Y * TERMINAL_CHAR_HEIGHT &&
                        mousePos.y <= ((*it)->position.Y + (*it)->sprite.size() / (*it)->spriteWidth) * TERMINAL_CHAR_HEIGHT)
                    {
                        selectedGameObjectData = (*it);
                        break;
                    }
                }
                gameObjectPropertiesRenderer.SelectGameObject(selectedGameObjectData);
            }
        }
    }
}

void InEditorRenderer::ClearSelection()
{
    gameObjectPropertiesRenderer.SelectGameObject(nullptr);
}

void InEditorRenderer::SetGameObjectScripts(std::vector<std::string> gameObjectScripts)
{
    gameObjectPropertiesRenderer.SetGameObjectScripts(gameObjectScripts);
}

void InEditorRenderer::SetComponentScripts(std::vector<std::string> componentScripts)
{
    gameObjectPropertiesRenderer.SetComponentScripts(componentScripts);
}
