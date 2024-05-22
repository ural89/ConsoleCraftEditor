#include "ProjectManager.h"
std::string ProjectManager::PROJECTS_DIR  = std::filesystem::current_path().parent_path().string() + "/Projects/";
std::string ProjectManager::PROJECT_NAME  = "NewProject";
ProjectManager::ProjectManager()
{
  
}