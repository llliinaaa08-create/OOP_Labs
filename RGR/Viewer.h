#ifndef VIEWER_H
#define VIEWER_H

#include <windows.h>
#include <memory>
#include "Analysis.h"

class TreeViewer {
private:
    std::shared_ptr<FileNode> rootNode;
    int totalWidth = 0;
    int totalHeight = 0;
    int nodeWidth = 140;
    int nodeHeight = 40;
    int horizontalSpacing = 60;
    int verticalSpacing = 40;

    void CalculateLayout(std::shared_ptr<FileNode> node, int currentDepth, int& currentY);
    void DrawTree(HDC hdc, std::shared_ptr<FileNode> node, int scrollX, int scrollY);

public:
    TreeViewer() = default;
    void SetRootNode(std::shared_ptr<FileNode> root);
    void Render(HDC hdc, int scrollX, int scrollY);
    int GetTotalWidth() const { return totalWidth; }
    int GetTotalHeight() const { return totalHeight; }
};

#endif 