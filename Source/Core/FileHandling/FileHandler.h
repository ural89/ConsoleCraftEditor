#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Directories.h"
class FileHandler
{
public:
    void ReplacePlaceholders(std::ifstream &templateFile, std::ofstream &headerFile, const std::string &placeholder, const std::string &replaceWord);
    bool CreateGameObjectsIncludeFile(const std::string &projectName);
    bool CreateComponentsIncludeFile(const std::string &projectName);
    void ReplacePlaceholders(std::ifstream &templateFile, std::ofstream &outFile, const std::string placeholders[], const std::string values[], size_t size);

    void AddLineToFile(const std::string &inFile, const std::string &line);
    bool CreateDirectory(const std::string &path);
    bool OpenVisualStudioCode(const std::string &path);
    void BuildAndRun(const std::string &path, const std::string &projectName);
    bool GetFiles(std::vector<std::string> &directories, std::vector<std::string> &files);
    void WriteToFile(const std::string &filename, const std::string &projectName, const std::string &json);

private:
};