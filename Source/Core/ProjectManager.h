#pragma once
#include <string>
#include <filesystem>

class ProjectManager {
private:

public:
    static std::string PROJECTS_DIR; //TODO: refactor here
    static std::string PROJECT_NAME;
    ProjectManager();


    static std::string GetProjectDirectory()  {
        return PROJECTS_DIR;
    }

 
    static void SetProjectDirectory(const std::string& projectDir) {
        PROJECTS_DIR = projectDir;
    }
};
