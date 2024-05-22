#pragma once
#include <string>
#include "Core/FileHandling/SceneDataSerializer.h"
#include "Core/Scripting/SceneDataDeserializer.h"
#include "Core/FileHandling/ProjectCreator.h"
#include "Core/FileHandling/GameObjectScriptCreator.h"
#include "Core/FileHandling/FileHandler.h"
#include "Core/InEditorRenderer.h"
class Window
{
public:
    Window();
    ~Window();
    void Update();

private:
    // rendering
    void RenderMainEditor();

    void RunProject();
    void UpdateDock();
    void RenderCreateProject();
    void LoadSelectedProject(std::string &projectNameToLoad);
    void RenderLoadProjectDirectoryTree();

    void RenderCreateScene();
    void CreateScene();
    void LoadScene();
    void SaveScene();
    void RenderCreateGameObject();
    void RenderCreateGameObjectScript();
    void RenderTerminalWindow(SceneData &sceneData);
    void RenderScene(SceneData &sceneData);

    // file logic
    void UpdateScriptList();
    void UpdateFileLists();
    void CreateGameObject(const std::string &gameObjectName, const std::string &scriptName, Vector2 position, std::string symbol, int spriteWidth, std::vector<int> &sprite);

private:
    char projectNameText[32] = "NewProject";
    char projectName[32] = "NewProject";
    char projectDirectory[256] = "/home/ural/Documents/Projects/";
    char gameObjectNameText[32] = "NewGameObject";
    char gameObjectScriptNameText[32] = "NewGameObjectScript";
    char componentScriptNameText[32] = "NewComponent";
    char sceneNameText[32] = "EntryScene";
    int gridValues[10][10] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

private:
    bool showCreateProject = false;
    bool showCreateGameObject = false;
    bool showCreateGameObjectScript = false;
    bool showCreateScene = false;
    bool showLoadProject = false;
    bool showProjectDirectory = true;


    bool hasProjectLoaded = false;
    std::vector<std::string> directoriesList;
    std::vector<std::string> filesList;

    InEditorRenderer inEditorRenderer;

    FileHandler fileHandler;
    SceneDataSerializer sceneDataSerializer;
    SceneDataDeserializer sceneDataDeserializer;
    ProjectCreator projectCreator;
    GameObjectScriptCreator gameObjectScriptCreator;
    int gameObjectId;

    int GetGameObjectID()
    {
        return gameObjectId++;
    }

private:
    // TODO: TEMP

    //    / GameObjectData player = {0, "Player", "testScript", {5, 5}, {"*"}, {2}, {1, 1, 1, 1, 1, 1}, {ComponentData{"testComponent"}}};
    //  GameObjectData enemy = {"Enemy", {14, 5}, {"*"}, {3}, {1,1,1,1,1,1}};

    SceneData sceneData;
};