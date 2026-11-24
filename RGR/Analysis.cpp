#include "Analysis.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

std::string DependencyAnalyzer::ExtractFilename(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return path;
}

std::string DependencyAnalyzer::GetDirectory(const std::string& filePath) {
    size_t pos = filePath.find_last_of("/\\");
    if (pos != std::string::npos) {
        return filePath.substr(0, pos);
    }
    return ".";
}

void DependencyAnalyzer::ParseFile(std::shared_ptr<FileNode> node, const std::string& currentDir) {
    std::ifstream file(node->fullPath);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        // Видаляємо пробіли на початку
        size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos) continue;
        line = line.substr(start);

        // Шукаємо директиву #include
        if (line.rfind("#include", 0) == 0) {
            size_t openQuote = line.find('"');
            size_t closeQuote = line.find('"', openQuote + 1);

            if (openQuote != std::string::npos && closeQuote != std::string::npos) {
                std::string incName = line.substr(openQuote + 1, closeQuote - openQuote - 1);
                std::string fullIncPath = currentDir + "\\" + incName;

                if (visitedFiles.find(incName) != visitedFiles.end()) {
                    node->includes.push_back(visitedFiles[incName]);
                } else {
                    auto childNode = std::make_shared<FileNode>();
                    childNode->fileName = incName;
                    childNode->fullPath = fullIncPath;
                    childNode->depth = node->depth + 1;
                    
                    visitedFiles[incName] = childNode;
                    node->includes.push_back(childNode);

                    ParseFile(childNode, currentDir);
                }
            }
        }
    }
}

std::shared_ptr<FileNode> DependencyAnalyzer::AnalyzeProject(const std::string& mainFilePath) {
    visitedFiles.clear();
    projectDirPath = GetDirectory(mainFilePath);

    auto rootNode = std::make_shared<FileNode>();
    rootNode->fileName = ExtractFilename(mainFilePath);
    rootNode->fullPath = mainFilePath;
    rootNode->depth = 0;

    visitedFiles[rootNode->fileName] = rootNode;
    ParseFile(rootNode, projectDirPath);

    return rootNode;
}