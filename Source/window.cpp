#include "window.h"
#include "imgui.h"
#include "Core/ProjectManager.h"

Window::Window()
{
    // sceneData.GameObjectDatas.push_back(&player);
    UpdateScriptList();
    gameObjectId = 0;
    // UpdateFileLists();
}

Window::~Window()
{
    for (auto &data : sceneData.GameObjectDatas)
    {
        delete data;
        data = nullptr;
    }
    sceneData.GameObjectDatas.clear();
}

void Window::Update()
{
    UpdateDock();
    if (!hasProjectLoaded)
    {
        RenderCreateProject();
    }
    if (showProjectDirectory)
        RenderLoadProjectDirectoryTree();
    if (hasProjectLoaded)
        RenderMainEditor();
}

void Window::RenderMainEditor()
{

    RenderTerminalWindow(sceneData);
    inEditorRenderer.Update(sceneData);

    //  ImGui::ShowDemoWindow();

    ImGui::Begin("Editor");

    if (ImGui::Button("Gameobject"))
    {
        showCreateGameObject = !showCreateGameObject;
    }
    // if (ImGui::Button("Scene"))
    // {
    //     showCreateScene = !showCreateScene;
    // }
    if (ImGui::Button("Run"))
    {
        RunProject();
    }
    // if (ImGui::Button("DeleteProject")) // TODO: Temp
    // {

    //     projectCreator.DeleteProjectFolder(projectName);
    //     UpdateFileLists();
    //     UpdateScriptList();
    // }

    if (showCreateGameObject)
    {
        RenderCreateGameObject();
    }
    if (showCreateScene)
    {
        RenderCreateScene();
    }
    if (showCreateGameObjectScript)
    {
        RenderCreateGameObjectScript();
    }
    ImGui::End();
}

void Window::RunProject()
{

    SaveScene();
    fileHandler.CreateDirectory(ProjectManager::PROJECTS_DIR + projectName + "/build");
    fileHandler.BuildAndRun(ProjectManager::PROJECTS_DIR + projectName + "/build/", projectName);
}
void Window::UpdateScriptList()
{

    inEditorRenderer.SetGameObjectScripts(gameObjectScriptCreator.GetGameObjectScripts(projectName));
}
void Window::UpdateFileLists()
{
    filesList.clear();
    directoriesList.clear();

    fileHandler.GetFiles(directoriesList, filesList);
    for (auto &file : filesList)
    {
        // std::cout << file << '\n';
    }
    for (auto &directory : directoriesList)
    {
        // std::cout << directory << '\n';
    }
}
void Window::RenderLoadProjectDirectoryTree()
{
    ImGui::Begin("Project Files");

    for (const auto &file : filesList)
    {
        // Check if the file has a .project extension
        if (file.substr(file.find_last_of(".") + 1) == "project")
        {
            std::size_t found = file.find_last_of("/\\");
            std::string fileName = file.substr(found + 1);

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::TreeNodeEx(fileName.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet);
            ImGui::PopStyleColor();

            if (ImGui::IsItemClicked())
            {

                std::string projectNameToLoad = fileName.substr(0, fileName.find_last_of("."));

                LoadSelectedProject(projectNameToLoad);
            }
            ImGui::TreePop(); // Pop for the file node
        }
    }

    ImGui::End();
}

void Window::RenderCreateScene()
{
    ImGui::Begin("CreateScene");

    // ImGui::InputText("Scene name", sceneNameText, IM_ARRAYSIZE(sceneNameText));

    if (ImGui::Button("Load Scene"))
    {
        LoadScene();
    }
    if (ImGui::Button("Save Scene"))
    {
        SaveScene();
    }

    if (ImGui::Button("Close"))
    {
        showCreateScene = false;
    }

    ImGui::End();
}
void Window::CreateScene()
{
    // Remove ".scene" extension from sceneNameText
    std::string sceneNameWithoutExtension = sceneNameText;
    if (sceneNameWithoutExtension.size() >= 6 && sceneNameWithoutExtension.substr(sceneNameWithoutExtension.size() - 6) == ".scene")
    {
        sceneNameWithoutExtension = sceneNameWithoutExtension.substr(0, sceneNameWithoutExtension.size() - 6);
    }

    std::string serializedData = sceneDataSerializer.Serialize(sceneData, projectName);
    sceneDataSerializer.CreateSceneScript(sceneNameWithoutExtension, projectName);
}
void Window::LoadScene()
{
    int length = strlen(sceneNameText);
    if (strcmp(sceneNameText + length - 6, ".scene") != 0)
    {
        strcat(sceneNameText, ".scene");
    }
    if (sceneDataSerializer.Deserialize(ProjectManager::PROJECTS_DIR + projectName + SCENE_DATA_DIR + sceneNameText, sceneData))
    {
        std::cout << "Loaded scene! \n";
        int biggestId = 0;
        for (auto &data : sceneData.GameObjectDatas)
        {
            if (data->id >= biggestId)
                biggestId = data->id;
        }
        gameObjectId = biggestId + 1;
    }
}
void Window::SaveScene()
{
    std::string serializedData = sceneDataSerializer.Serialize(sceneData, projectName);
    int length = strlen(sceneNameText);
    if (strcmp(sceneNameText + length - 6, ".scene") != 0)
    {
        strcat(sceneNameText, ".scene");
    }
    fileHandler.CreateDirectory(ProjectManager::PROJECTS_DIR + projectName + SCENE_DATA_DIR);
    fileHandler.WriteToFile(ProjectManager::PROJECTS_DIR + projectName + SCENE_DATA_DIR + sceneNameText, projectName, serializedData);
}
void Window::RenderCreateGameObject()
{
    static int gridWidth = 3;
    static int gridHeight = 3;

    ImGui::Begin("Create gameobject");
    ImGui::PushItemWidth(50);
    ImGui::InputInt("Grid Width", &gridWidth);
    ImGui::InputInt("Grid Height", &gridHeight);

    if (gridWidth <= 0)
        gridWidth = 1;
    if (gridHeight <= 0)
        gridHeight = 1;

    float inputWidth = 50; // ImGui::CalcItemWidth() / static_cast<float>(gridWidth);

    // Render the grid
    ImGui::BeginChild("Grid", ImVec2(300, 200), true, ImGuiWindowFlags_HorizontalScrollbar);

    // Iterate through rows
    for (int y = 0; y < gridHeight; ++y)
    {
        ImGui::BeginGroup();

        for (int x = 0; x < gridWidth; ++x)
        {
            std::string id = "##" + std::to_string(x) + std::to_string(y); // we have to give unique ID's to work for some reason

            ImGui::PushItemWidth(inputWidth);
            ImGui::InputInt(id.c_str(), &gridValues[x][y], 0, 9);
            ImGui::PopItemWidth();

            ImGui::SameLine();
        }

        ImGui::EndGroup();
    }

    ImGui::EndChild();
    ImGui::PushItemWidth(150);
    ImGui::InputText("Gameobject Name", gameObjectNameText, IM_ARRAYSIZE(gameObjectNameText));
    std::vector<int> sprite;
    if (ImGui::Button("Create GameObject"))
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            for (int x = 0; x < gridWidth; ++x)
            {
                sprite.push_back(gridValues[x][y]);
            }
        }

        CreateGameObject(gameObjectNameText, "None", Vector2(5, 5), "*", gridWidth, sprite);
    }

    if (ImGui::Button("Create GameObject Script"))
    {
        showCreateGameObjectScript = true;
    }

    if (ImGui::Button("Close Window"))
    {
        showCreateGameObject = false;
    }
    ImGui::End();
}
void Window::RenderCreateGameObjectScript()
{
    ImGui::Begin("Create Gameobject script");
    ImGui::InputText("GameObject Script name", gameObjectScriptNameText, IM_ARRAYSIZE(gameObjectScriptNameText));
    if (ImGui::Button("Create GameObject Script"))
    {
        gameObjectScriptCreator.CreateScript(gameObjectScriptNameText, projectName);
        UpdateFileLists();
        UpdateScriptList();
        showCreateGameObjectScript = false;
    }
    if (ImGui::Button("Close"))
    {
        showCreateGameObjectScript = false;
    }

    ImGui::End();
}
void Window::RenderCreateProject()
{
    ImGui::Begin("CreateProject");

    ImGui::InputText("Project name", projectNameText, IM_ARRAYSIZE(projectNameText));
    ImGui::InputText("ProjectDirectory", projectDirectory, 100);
    ProjectManager::SetProjectDirectory(projectDirectory);
    if (ImGui::Button("Create Project"))
    {

        projectCreator.CreateProject(projectNameText);
        fileHandler.CreateComponentsIncludeFile(projectNameText);
        fileHandler.CreateGameObjectsIncludeFile(projectNameText);
        UpdateFileLists();
        UpdateScriptList();
    }
    if (ImGui::Button("Update File Tree"))
    {
        UpdateFileLists();
    }

    if (ImGui::Button("DeleteProject"))
    {
        // TODO: promt again!
        projectCreator.DeleteProjectFolder(projectNameText);
        UpdateFileLists();
        UpdateScriptList();
    }

    ImGui::End();
}
void Window::LoadSelectedProject(std::string &projectNameToLoad)
{
    ProjectManager::PROJECT_NAME = projectNameToLoad;
    const char *projectNameBuffer = projectNameToLoad.c_str();
    std::strcpy(projectName, projectNameBuffer);
    hasProjectLoaded = true;
    showProjectDirectory = false;
    // UpdateFileLists();
    LoadScene();
    CreateScene();
    UpdateScriptList();
}
void Window::RenderScene(SceneData &sceneData)
{
    inEditorRenderer.Render(sceneData);
}
void Window::RenderTerminalWindow(SceneData &sceneData)
{

    ImGui::Begin("GameView", nullptr);
    RenderScene(sceneData);
    if (showCreateScene)
    {
        inEditorRenderer.ClearSelection();
    }
    ImGui::BeginChild("GameView", ImVec2(0, 0), true);
    ImGui::EndChild();
    ImGui::End();
}

void Window::CreateGameObject(const std::string &gameObjectName, const std::string &scriptName, Vector2 position, std::string symbol, int spriteWidth, std::vector<int> &sprite)
{
    GameObjectData *gameObjectData = new GameObjectData{// TODO: fix this
                                                        GetGameObjectID(),
                                                        gameObjectName,
                                                        scriptName,
                                                        position,
                                                        symbol,
                                                        spriteWidth,
                                                        sprite};
    sceneData.GameObjectDatas.push_back(gameObjectData);
}
void Window::UpdateDock()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode;
            }
            if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit;
            }
            if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking;
            }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
            }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
            }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen))
            {
                dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
            }
            ImGui::Separator();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    ImGui::End();
}
