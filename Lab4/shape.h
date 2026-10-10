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
        // Зберігаємо початкові координати куба
        long ox1 = x1, oy1 = y1, ox2 = x2, oy2 = y2;

        int dx = (ox2 - ox1) / 3;
        int dy = (oy2 - oy1) / 3;

        // 1. Задня грань — через RectangleShape::Draw
        SetPoints(ox1 + dx, oy1 - dy, ox2 + dx, oy2 - dy);
        RectangleShape::Draw(hdc);

        // 2. Передня грань — через RectangleShape::Draw
        SetPoints(ox1, oy1, ox2, oy2);
        RectangleShape::Draw(hdc);

        // 3. Чотири ребра — через LineShape::Draw
        // Ліве верхнє ребро
        SetPoints(ox1, oy1, ox1 + dx, oy1 - dy);
        LineShape::Draw(hdc);

        // Праве верхнє ребро
        SetPoints(ox2, oy1, ox2 + dx, oy1 - dy);
        LineShape::Draw(hdc);

        // Ліве нижнє ребро
        SetPoints(ox1, oy2, ox1 + dx, oy2 - dy);
        LineShape::Draw(hdc);

        // Праве нижнє ребро
        SetPoints(ox2, oy2, ox2 + dx, oy2 - dy);
        LineShape::Draw(hdc);

        // Відновлюємо початкові координати куба
        SetPoints(ox1, oy1, ox2, oy2);
    }
};

#endif