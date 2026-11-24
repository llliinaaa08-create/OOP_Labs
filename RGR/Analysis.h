#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>

struct FileNode {
    std::string fileName;                   
    std::string fullPath;                   // Повний шлях до файла
    std::vector<std::shared_ptr<FileNode>> includes; // Залежності (#include)
    int depth = 0;                          // Рівень в ієрархії
    int x = 0, y = 0;                       // Координати для візуалізації
};

class DependencyAnalyzer {
private:
    std::string projectDirPath;
    std::map<std::string, std::shared_ptr<FileNode>> visitedFiles;

    std::string ExtractFilename(const std::string& path);
    std::string GetDirectory(const std::string& filePath);
    void ParseFile(std::shared_ptr<FileNode> node, const std::string& currentDir);

public:
    DependencyAnalyzer() = default;
    
    // Аналіз починається з обраного головного файла або папки
    std::shared_ptr<FileNode> AnalyzeProject(const std::string& mainFilePath);
};

#endif 