#ifndef SHAPE_H
#define SHAPE_H

#include <windows.h>
#include <cmath>

class Shape {
protected:
    long x1, y1, x2, y2;
public:
    virtual ~Shape() {}
    void SetPoints(long startX, long startY, long endX, long endY) {
        x1 = startX; y1 = startY;
        x2 = endX;   y2 = endY;
    }
    virtual void Draw(HDC hdc) = 0;
};

class PointShape : public Shape {
public:
    void Draw(HDC hdc) override {
        SetPixel(hdc, x1, y1, RGB(0, 0, 0));
    }
};

class LineShape : public Shape {
public:
    void Draw(HDC hdc) override {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
};

class RectangleShape : public Shape {
public:
    void Draw(HDC hdc) override {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH hBrush = CreateSolidBrush(RGB(144, 238, 144)); // Світло-зелене заповнення

        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        long dx = std::abs(x2 - x1);
        long dy = std::abs(y2 - y1);

        Rectangle(hdc, x1 - dx, y1 - dy, x1 + dx, y1 + dy);

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
        DeleteObject(hBrush);
    }
};

class EllipseShape : public Shape {
public:
    void Draw(HDC hdc) override {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);

        Ellipse(hdc, x1, y1, x2, y2);

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
    }
};

#endif