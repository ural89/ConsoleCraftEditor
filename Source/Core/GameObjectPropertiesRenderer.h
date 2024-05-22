#pragma once

#include "Core/DataStructs.h"
#include "FileHandling/ComponentScriptCreator.h"
#include <iostream>
#include "imgui.h"
#include <algorithm>

class GameObjectPropertiesRenderer
{
public:
    void SelectGameObject(GameObjectData *gameObjectData)
    {

        selectedGameObjectData = gameObjectData;
        if (gameObjectData != nullptr)
        {
            selectedGameObjectScript = gameObjectData->script;
        }
        UpdateComponentScripts();
    }

    void Render()
    {
        if (selectedGameObjectData != nullptr)
        {

            RenderGameObjectScriptSelectionMenu();
            RenderComponents();
            ImGui::InputText("Name", &selectedGameObjectData->name[0], selectedGameObjectData->name.size() + 1);

            if (ImGui::Button("Add Component"))
            {
                showCreateComponentMenu = true;
            }

            if (showCreateComponentMenu)
            {
                RenderComponentCreateMenu();
            }
            ImGui::Text("Id: %d", selectedGameObjectData->id);
            ImGui::InputFloat("Position X", &selectedGameObjectData->position.X);
            ImGui::InputFloat("Position Y", &selectedGameObjectData->position.Y);
            ImGui::InputInt("ParentId", &selectedGameObjectData->parentId);
            ImGui::InputText("Symbol", &selectedGameObjectData->symbol[0], selectedGameObjectData->symbol.size() + 1);
            ImGui::InputInt("Sprite Width", &selectedGameObjectData->spriteWidth);

            ImGui::Text("Sprite");

            ImVec2 itemSize(25.0f, 25.0f); // size of each sprite item in the grid
            ImGui::BeginChild("SpriteGrid", ImVec2(0, itemSize.y * 4 + ImGui::GetTextLineHeightWithSpacing()), true);

            for (int i = 0; i < selectedGameObjectData->sprite.size(); ++i)
            {
                if (i % selectedGameObjectData->spriteWidth == 0)
                    ImGui::NewLine();

                ImGui::PushID(i);
                ImGui::PushItemWidth(100);
                ImGui::InputInt("", &selectedGameObjectData->sprite[i]);
                ImGui::PopItemWidth();
                ImGui::PopID();

                ImGui::SameLine();
            }

            ImGui::EndChild();
            if (ImGui::Button("Create template"))
            {
                selectedGameObjectData->isTemplate = true;
            }
        }
    }

    void CreateComponent()
    {

        componentScriptCreator.CreateComponentScript(componentScriptNameText, ProjectManager::PROJECT_NAME);
        UpdateComponentScripts();
        std::string componentName = componentScriptNameText;
        AddComponent(componentName); //add component after creating it directly
    }

    void SetGameObjectScripts(std::vector<std::string> &gameObjectScripts)
    {
        this->gameObjectScripts = gameObjectScripts;
    }

    void SetComponentScripts(std::vector<std::string> componentScripts)
    {
        this->componentScripts = componentScripts;
    }

private:
    void UpdateComponentScripts()
    {
        SetComponentScripts(componentScriptCreator.GetComponentScripts(ProjectManager::PROJECT_NAME));
    }
    void RenderGameObjectScriptSelectionMenu()
    {
        if (ImGui::BeginCombo("Select Script", selectedGameObjectScript.c_str()))
        {
            for (const auto &script : gameObjectScripts)
            {
                bool isSelected = (selectedGameObjectScript == script);
                if (ImGui::Selectable(script.c_str(), isSelected))
                {
                    selectedGameObjectScript = script;
                    if (selectedGameObjectData != nullptr)
                        selectedGameObjectData->script = selectedGameObjectScript;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
    void RenderComponentCreateMenu()
    {
        ImGui::Begin("Create Component");
        if (ImGui::BeginCombo("Add Component", "Select Component Script"))
        {
            for (const auto &script : componentScripts)
            {

                auto componentAlreadyAdded = [&script](const ComponentData &component)
                {
                    return component.name == script;
                };
                if (std::find_if(selectedGameObjectData->components.begin(), selectedGameObjectData->components.end(),
                                 componentAlreadyAdded) == selectedGameObjectData->components.end()) // dont list if already added component
                {

                    if (ImGui::Selectable(script.c_str()))
                    {
                        selectedComponentScript = script;
                        if (selectedGameObjectData != nullptr)
                        {
                            AddComponent(selectedComponentScript);
                        }
                    }
                }
            }
            ImGui::EndCombo();
        }
        ImGui::InputText("Component script name", componentScriptNameText, IM_ARRAYSIZE(componentScriptNameText));
        if (ImGui::Button("Create Component"))
        {
            CreateComponent();
            showCreateComponentMenu = false;
        }
        if (ImGui::Button("Close"))
        {
            showCreateComponentMenu = false;
        }
        ImGui::End();
    }

    void AddComponent(std::string &componentName)
    {
        ComponentData componentToAdd{componentName};
        selectedGameObjectData->components.push_back(componentToAdd);
    }

    void RenderComponents()
    {
        if (selectedGameObjectData->components.size() == 0)
            return;
        ImGui::Text("Components:");
        for (size_t i = 0; i < selectedGameObjectData->components.size(); ++i)
        {
            auto &component = selectedGameObjectData->components[i];
            ImGui::BulletText("%s", component.name.c_str());
            ImGui::SameLine();
            ImGui::PushID(i);
            if (ImGui::Button("X")) // delete button for component
            {
                selectedGameObjectData->components.erase(selectedGameObjectData->components.begin() + i);
                --i;
            }
            ImGui::PopID();
        }
    }

    GameObjectData *selectedGameObjectData = nullptr;
    ComponentScriptCreator componentScriptCreator;

    std::string selectedGameObjectScript = "";
    std::string selectedComponentScript = "";

    char componentScriptNameText[32] = "NewComponent";

    std::vector<std::string> gameObjectScripts;
    std::vector<std::string> componentScripts;

    bool showCreateComponentMenu = false;
};
