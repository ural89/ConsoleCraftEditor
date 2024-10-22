#pragma once
#include "Core/DataStructs.h"
#include "imgui.h"
#include "GameObjectPropertiesRenderer.h"
class InEditorRenderer
{
public:
    InEditorRenderer();

    void Render(SceneData &sceneData);
    void RenderGamObjectProperties(SceneData &sceneData);
    void DestroySelectedGameObject(SceneData &sceneData);
    void Update(SceneData &sceneData);
    void ClearSelection(); // this is to prevent crashes on null gameobject selection
    void SetGameObjectScripts(std::vector<std::string> gameObjectScripts);
    void SetComponentScripts(std::vector<std::string> componentScripts);

private:
    void RenderHierarchy(SceneData &sceneData);
    void RenderTemplates(SceneData& sceneData);
    void RenderBehaviorTree(SceneData& sceneData);
    
    GameObjectPropertiesRenderer gameObjectPropertiesRenderer;
    GameObjectData *dragSourceGameObject;
    
    float xOffset = 0;
    float accumulatedDeltaX = 0.0f;
    float accumulatedDeltaY = 0.0f;

    class ImVec2 gameViewPosition;
    GameObjectData *selectedGameObjectData = nullptr;
};