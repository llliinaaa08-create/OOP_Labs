#ifndef SHAPE_H
#define SHAPE_H

#include <windows.h>
#include <cmath>

class Shape {
protected:
    long x1, y1, x2, y2;
public:
    virtual ~Shape() {}
    virtual void SetPoints(long startX, long startY, long endX, long endY) {
        x1 = startX; y1 = startY;
        x2 = endX;   y2 = endY;
    }
    virtual void Draw(HDC hdc) = 0;
};

class PointShape : public virtual Shape {
public:
    void Draw(HDC hdc) override {
        SetPixel(hdc, x1, y1, RGB(0, 0, 0));
    }
};

class LineShape : public virtual Shape {
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

class RectangleShape : public virtual Shape {
public:
    void Draw(HDC hdc) override {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH hBrush = CreateSolidBrush(RGB(144, 238, 144)); // Світло-зелене заповнення

        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        Rectangle(hdc, x1, y1, x2, y2);

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
        DeleteObject(hBrush);
    }
};

class EllipseShape : public virtual Shape {
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

class LineOOShape : public LineShape, public EllipseShape {
public:
    void Draw(HDC hdc) override {
        LineShape::Draw(hdc);

        long ox1 = x1, oy1 = y1, ox2 = x2, oy2 = y2;
        int r = 5;

        SetPoints(ox1 - r, oy1 - r, ox1 + r, oy1 + r);
        EllipseShape::Draw(hdc);

        SetPoints(ox2 - r, oy2 - r, ox2 + r, oy2 + r);
        EllipseShape::Draw(hdc);

        SetPoints(ox1, oy1, ox2, oy2);
    }
};

class CubeShape : public RectangleShape, public LineShape {
public:
    void Draw(HDC hdc) override {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // Прозора кисть

        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);

        int dx = (x2 - x1) / 3;
        int dy = (y2 - y1) / 3;

        // Задній прямокутник
        Rectangle(hdc, x1 + dx, y1 - dy, x2 + dx, y2 - dy);

        // Передній прямокутник
        Rectangle(hdc, x1, y1, x2, y2);

        // З'єднувальні лінії кутів
        MoveToEx(hdc, x1, y1, NULL); LineTo(hdc, x1 + dx, y1 - dy);
        MoveToEx(hdc, x2, y1, NULL); LineTo(hdc, x2 + dx, y1 - dy);
        MoveToEx(hdc, x1, y2, NULL); LineTo(hdc, x1 + dx, y2 - dy);
        MoveToEx(hdc, x2, y2, NULL); LineTo(hdc, x2 + dx, y2 - dy);

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
    }
};

#endif