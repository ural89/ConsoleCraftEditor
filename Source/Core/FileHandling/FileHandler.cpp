#include "FileHandler.h"
#include "../ProjectManager.h"
void FileHandler::ReplacePlaceholders(std::ifstream &templateFile, std::ofstream &outFile, const std::string &placeholder, const std::string &replaceWord)
{
    std::string line;
    while (std::getline(templateFile, line))
    {
        size_t pos = 0;
        while ((pos = line.find(placeholder, pos)) != std::string::npos)
        {
            line.replace(pos, placeholder.length(), replaceWord);
            pos += replaceWord.length();
        }
        outFile << line << '\n';
    }
}
bool FileHandler::CreateGameObjectsIncludeFile(const std::string &projectName)
{
    const std::string includeGameObjectFilePath = ProjectManager::PROJECTS_DIR + projectName + "/Source/" + "IncludeGameObjects.h";
    if (!std::filesystem::exists(includeGameObjectFilePath))
    {
        std::ofstream includeFileGameObjectFile(includeGameObjectFilePath);
        std::cout << "creating include file" << '\n';
    }
    const std::string includeRegistryObjectListFilePath = ProjectManager::PROJECTS_DIR + projectName + "/Source/" + "IncludeRegisterObjectList.h";
    if (!std::filesystem::exists(includeRegistryObjectListFilePath))
    {
        std::ofstream includeRegistryObjectListFile(includeRegistryObjectListFilePath);
    }
    return true;
}
bool FileHandler::CreateComponentsIncludeFile(const std::string &projectName)
{
    const std::string includeGameObjectFilePath = ProjectManager::PROJECTS_DIR + projectName + "/Source/" + "IncludeComponents.h";
    if (!std::filesystem::exists(includeGameObjectFilePath))
    {
        std::ofstream includeFileGameObjectFile(includeGameObjectFilePath);
    }
    const std::string includeRegistryObjectListFilePath = ProjectManager::PROJECTS_DIR + projectName + "/Source/" + "IncludeRegisterComponentList.h";
    if (!std::filesystem::exists(includeRegistryObjectListFilePath))
    {
        std::ofstream includeRegistryObjectListFile(includeRegistryObjectListFilePath);
    }

    return true;
}

void FileHandler::ReplacePlaceholders(std::ifstream &templateFile, std::ofstream &outFile, const std::string placeholders[], const std::string values[], size_t size)
{
    std::string line;
    while (std::getline(templateFile, line))
    {
        for (size_t i = 0; i < size; ++i)
        {
            size_t pos = 0;
            const std::string &placeholder = placeholders[i];
            const std::string &value = values[i];
            while ((pos = line.find(placeholder, pos)) != std::string::npos)
            {
                line.replace(pos, placeholder.length(), value);
                pos += value.length();
            }
        }
        outFile << line << '\n';
    }
}

void FileHandler::AddLineToFile(const std::string &inFilePath, const std::string &line)
{
    std::ofstream outFile(inFilePath, std::ios::app); // app means append mode
    if (outFile.is_open())
    {
        outFile << line << '\n';
        outFile.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << inFilePath << std::endl;
    }
}

bool FileHandler::CreateDirectory(const std::string &path)
{

    if (std::filesystem::exists(path))
    {
        std::cout << "Directory already exists: " << path << std::endl;
        return false;
    }

    try
    {
        std::filesystem::create_directory(path);
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cout << "Failed to create directory: " << e.what() << std::endl;
        return false;
    }

    return true;
}
bool FileHandler::OpenVisualStudioCode(const std::string &path)
{
    // Command to open Visual Studio Code with the project folder
    std::string command = "code " + path;

    // Execute the command
    int result = system(command.c_str());
    if (result != 0)
    {
        return false;
    }

    return true;
}

void FileHandler::BuildAndRun(const std::string &path, const std::string &projectName)
{
    std::string command = "cd " + path + "&& cmake -G Ninja .. && ninja && " + "./" + projectName; //"cd " + path + " && cmake . && make && ./" + projectName;

    // Execute the command
    int result = system(command.c_str());
    if (result != 0)
    {
        std::cerr << "Failed to build and run the project." << std::endl;
    }
    else
    {
        std::cout << "Project built and run successfully." << std::endl;
    }
}

bool FileHandler::GetFiles(std::vector<std::string> &directories, std::vector<std::string> &files)
{
    std::filesystem::path projectsPath(ProjectManager::PROJECTS_DIR);
    if(!std::filesystem::exists(projectsPath))
    {
        return false;
    }
    for (const auto &entry : std::filesystem::recursive_directory_iterator(projectsPath))
    {
        if (std::filesystem::is_directory(entry.path()))
        {
            directories.push_back(entry.path().string());
        }
        else if (std::filesystem::is_regular_file(entry.path()))
        {
            files.push_back(entry.path().string());
        }
    }
    return true;
}
void FileHandler::WriteToFile(const std::string &filename, const std::string &projectName, const std::string &data)
{

    std::ofstream file(filename);
    if (file.is_open())
    {
        file << data;
        file.close();
        std::cout << "data written to file: " << filename << std::endl;
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}
