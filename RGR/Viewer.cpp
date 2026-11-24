#include "Viewer.h"

void TreeViewer::SetRootNode(std::shared_ptr<FileNode> root) {
    rootNode = root;
    totalWidth = 0;
    totalHeight = 0;
    if (!rootNode) return;

    int currentY = 20;
    CalculateLayout(rootNode, 0, currentY);

    totalHeight = currentY + 60;
}

void TreeViewer::CalculateLayout(std::shared_ptr<FileNode> node, int currentDepth, int& currentY) {
    if (!node) return;

    node->depth = currentDepth;
    node->x = 30 + currentDepth * (nodeWidth + horizontalSpacing);
    node->y = currentY;

    if (node->includes.empty()) {
        currentY += nodeHeight + verticalSpacing;
    } else {
        int startY = currentY;
        for (auto& child : node->includes) {
            CalculateLayout(child, currentDepth + 1, currentY);
        }
        if (!node->includes.empty()) {
            int firstChildY = node->includes.front()->y;
            int lastChildY = node->includes.back()->y;
            node->y = (firstChildY + lastChildY) / 2;
        }
    }

    if (node->x + nodeWidth + 50 > totalWidth) {
        totalWidth = node->x + nodeWidth + 50;
    }
}

void TreeViewer::DrawTree(HDC hdc, std::shared_ptr<FileNode> node, int scrollX, int scrollY) {
    if (!node) return;

    int drawX = node->x - scrollX;
    int drawY = node->y - scrollY;

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(70, 130, 180));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    for (auto& child : node->includes) {
        int childDrawX = child->x - scrollX;
        int childDrawY = child->y - scrollY;

        MoveToEx(hdc, drawX + nodeWidth, drawY + nodeHeight / 2, NULL);
        LineTo(hdc, childDrawX, childDrawY + nodeHeight / 2);

        DrawTree(hdc, child, scrollX, scrollY);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    HBRUSH hBrush = CreateSolidBrush(RGB(230, 240, 255));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Rectangle(hdc, drawX, drawY, drawX + nodeWidth, drawY + nodeHeight);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));
    RECT textRect = { drawX + 5, drawY + 5, drawX + nodeWidth - 5, drawY + nodeHeight - 5 };
    
    std::wstring wName(node->fileName.begin(), node->fileName.end());
    DrawTextW(hdc, wName.c_str(), -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

void TreeViewer::Render(HDC hdc, int scrollX, int scrollY) {
    if (rootNode) {
        DrawTree(hdc, rootNode, scrollX, scrollY);
    }
}