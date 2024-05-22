#pragma once
#include "FileHandler.h"
#include "Directories.h"
#include "../ProjectManager.h"
class ProjectCreator
{
public:
    FileHandler fileHandler;
    bool CreateProject(const std::string &projectName)
    {
        ProjectManager::PROJECT_NAME = projectName;
        fileHandler.CreateDirectory(ProjectManager::PROJECTS_DIR);

        if (!fileHandler.CreateDirectory(ProjectManager::PROJECTS_DIR + projectName))
        {
            std::cout << "Failed to create project directory: " << projectName << std::endl;
            return false;
        }

        std::string projectFilePath = ProjectManager::PROJECTS_DIR + projectName + "/" + projectName + ".project";
        std::ofstream prjectFile(projectFilePath);

        std::string sourcePath = ProjectManager::PROJECTS_DIR + projectName + "/Source";
        if (!fileHandler.CreateDirectory(sourcePath))
        {
            std::cout << "Failed to create source directory: " << sourcePath << std::endl;
            return false;
        }
        if (!CreateCMakeLists(projectName))
        {
            std::cout << "Failed to create CMakeLists.txt file.\n";
            return false;
        }
        if (!CreateEntryPointAndFill("EntryScene", projectName)) // TODO: get first from scenescriptsjson
        {
            std::cout << "Failed to create Entrypoint file.\n";
            return false;
        }
        std::cout << "Project '" << projectName << "' created successfully!\n";
        SetScriptRegistryInclude(projectName);
        CopyEngineFiles(projectName);
        CopyComponentScriptsList(projectName);
        CopyGameObjectsScriptsList(projectName);
        return true;
    }
    void DeleteProjectFolder(const std::string &projectName)
    {
        std::string projectPath = ProjectManager::PROJECTS_DIR + projectName;
        if (!std::filesystem::remove_all(projectPath))
        {
            std::cout << "Project not found! \n";
        }
        ClearScriptRegistryInclude(projectName);
    }
    void SetScriptRegistryInclude(const std::string &projectName) // TODO: carry this proper place
    {
        const std::string includeGameObjectsFilePath = "../Engine/Source/Core/Scripting/IncludeGameObjects.h";
        const std::string includeRegistryObjectListFilePath = "../Engine/Source/Core/Scripting/IncludeRegisterObjectList.h";
        std::ofstream includeGameObjectsFile(includeGameObjectsFilePath);
        if (includeGameObjectsFile.is_open())
        {
            std::string fileString = "#include  \"../../../../Projects/" + projectName + "/Source/IncludeGameObjects.h\"";
            includeGameObjectsFile << fileString;
        }
        includeGameObjectsFile.close();
        std::ofstream includeRegistryObjectListFile(includeRegistryObjectListFilePath);
        if (includeRegistryObjectListFile.is_open())
        {
            std::string fileString = "#include  \"../../../../Projects/" + projectName + "/Source/IncludeRegisterObjectList.h\"";
            includeRegistryObjectListFile << fileString;
        }
        includeGameObjectsFile.close();
        ///////////////////////////
        const std::string includeComponentsFilePath = "../Engine/Source/Core/Scripting/IncludeComponents.h";
        const std::string includeRegistryComponentListFilePath = "../Engine/Source/Core/Scripting/IncludeRegisterComponentList.h";
        std::ofstream includeComponentsFile(includeComponentsFilePath);
        if (includeComponentsFile.is_open())
        {
            std::string fileString = "#include  \"../../../../Projects/" + projectName + "/Source/IncludeComponents.h\"";
            includeComponentsFile << fileString;
        }
        includeComponentsFile.close();
        std::ofstream includeRegistryComponentListFile(includeRegistryComponentListFilePath);
        if (includeRegistryComponentListFile.is_open())
        {
            std::string fileString = "#include  \"../../../../Projects/" + projectName + "/Source/IncludeRegisterComponentList.h\"";
            includeRegistryComponentListFile << fileString;
        }
        includeComponentsFile.close();

        ///////////////////////////
    }

private:
    void ClearScriptRegistryInclude(const std::string &projectName) // TODO: call this everytime save a project or build a project
    {
        const std::string includeGameObjectsFilePath = "../Engine/Source/Core/Scripting/IncludeGameObjects.h";
        const std::string includeRegistryObjectListFilePath = "../Engine/Source/Core/Scripting/IncludeRegisterObjectList.h";
        std::ofstream includeGameObjectsFile(includeGameObjectsFilePath);
        if (includeGameObjectsFile.is_open())
        {
            std::string fileString = "\n";
            includeGameObjectsFile << fileString;
        }
        includeGameObjectsFile.close();
        std::ofstream includeRegistryObjectListFile(includeRegistryObjectListFilePath);
        if (includeRegistryObjectListFile.is_open())
        {
            std::string fileString = "\n";
            includeRegistryObjectListFile << fileString;
        }
        includeGameObjectsFile.close();
        /////////////////////
        const std::string includeComponentsFilePath = "../Engine/Source/Core/Scripting/IncludeComponents.h";
        const std::string includeRegistryComponentListFilePath = "../Engine/Source/Core/Scripting/IncludeRegisterComponentList.h";
        std::ofstream includeComponentsFile(includeComponentsFilePath);
        if (includeComponentsFile.is_open())
        {
            std::string fileString = "\n";
            includeComponentsFile << fileString;
        }
        includeComponentsFile.close();
        std::ofstream includeRegistryComponentListFile(includeRegistryComponentListFilePath);
        if (includeRegistryComponentListFile.is_open())
        {
            std::string fileString = "\n";
            includeRegistryComponentListFile << fileString;
        }
        includeComponentsFile.close();

        ///////////////
    }
    bool CreateCMakeLists(const std::string &projectName)
    {
        std::string cmakeFilePath = ProjectManager::PROJECTS_DIR + projectName + "/CMakeLists.txt";

        if (std::filesystem::exists(cmakeFilePath))
        {
            std::cout << "Header file already exists.\n";
            return false;
        }

        std::ifstream templateFile(CMAKE_TEMPLATE_DIR);
        if (!templateFile.is_open())
        {
            std::cout << "Failed to open template file.\n";
            return false;
        }

        std::ofstream cmakeFile(cmakeFilePath);
        if (!cmakeFile.is_open())
        {
            std::cout << "Failed to create header file.\n";
            templateFile.close();
            return false;
        }
        fileHandler.ReplacePlaceholders(templateFile, cmakeFile, "{ProjectName}", projectName);
        templateFile.close();
        cmakeFile.close();
        std::cout << "Successfully created header! \n";
        return true;
        return true;
    }
    void CopyEngineFiles(const std::string &projectName)
    {
        std::string sourceDir = "../Engine";
        std::string destinationDir = ProjectManager::PROJECTS_DIR + projectName + "/Engine";

        try
        {
            std::filesystem::copy(sourceDir, destinationDir, std::filesystem::copy_options::recursive);
            std::cout << "Engine files copied successfully!\n";
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Failed to copy engine files: " << e.what() << '\n';
        }
    }
    bool CreateEntryPointAndFill(const std::string &entrySceneName, const std::string &projectName)
    {
        std::string entryPointPath = ProjectManager::PROJECTS_DIR + projectName + "/Source/" + projectName + "EntryPoint.cpp";

        if (std::filesystem::exists(entryPointPath))
        {
            std::cout << "entrypoint file already exists.\n";
            return false;
        }

        std::ifstream templateFile(ENTRY_POINT_TEMPLATE_DIR);
        if (!templateFile.is_open())
        {
            std::cout << "Failed to open template file.\n";
            return false;
        }

        std::ofstream entryPoint(entryPointPath);
        if (!entryPoint.is_open())
        {
            std::cout << "Failed to create entrypoint file.\n";
            templateFile.close();
            return false;
        }
        const std::string placeholders[] = {"{ProjectName}", "{EntryScene}"};
        const std::string values[] = {projectName, entrySceneName};
        const size_t size = sizeof(placeholders) / sizeof(placeholders[0]);
        fileHandler.ReplacePlaceholders(templateFile, entryPoint, placeholders, values, size);
        templateFile.close();
        entryPoint.close();
        std::cout << "Successfully created entrypoint! \n";
        return true;
    }
    void CopyEngineComponents(const std::string &projectName)
    {
        std::string sourceDir = "../Engine/Source/Core/Component";
        std::string destinationDir = ProjectManager::PROJECTS_DIR + projectName + COMPONENTS_DIR;
        fileHandler.CreateDirectory(destinationDir);
        try
        {
            for (const auto &entry : std::filesystem::directory_iterator(sourceDir))
            {
                if (entry.path().extension() == ".cpp" || entry.path().extension() == ".h")
                {
                    std::filesystem::copy(entry.path(), destinationDir + "/" + entry.path().filename().string());
                }
            }
            std::cout << "Engine components copied successfully!\n";
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Failed to copy engine components: " << e.what() << '\n';
        }
    }
    void CopyComponentScriptsList(const std::string &projectName)
    {
        std::string sourceFile = "../ComponentScripts.json";
        std::string destinationFile = ProjectManager::PROJECTS_DIR + projectName + "/ComponentScripts.json";

        try
        {
            std::filesystem::copy(sourceFile, destinationFile, std::filesystem::copy_options::overwrite_existing);
            std::cout << "ComponentScripts.json copied successfully!\n";
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Failed to copy ComponentScripts.json: " << e.what() << '\n';
        }
    }
    void CopyGameObjectsScriptsList(const std::string &projectName)
    {
        std::string sourceFile = "../GameObjectScripts.json";
        std::string destinationFile = ProjectManager::PROJECTS_DIR + projectName + "/GameObjectScripts.json";

        try
        {
            std::filesystem::copy(sourceFile, destinationFile, std::filesystem::copy_options::overwrite_existing);
            std::cout << "GameObjectScripts.json copied successfully!\n";
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Failed to copy GameObjectScripts.json: " << e.what() << '\n';
        }
    }

private:
};
