#ifndef SHAPE_H
#define SHAPE_H

#include <windows.h>
#include <cmath>
#include <cwchar>
#include <wchar.h>

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

    
    virtual void GetName(wchar_t* buf, int bufSize) const = 0;
    virtual void GetPoints(int& rx1, int& ry1, int& rx2, int& ry2) const {
        rx1 = (int)x1; ry1 = (int)y1;
        rx2 = (int)x2; ry2 = (int)y2;
    }
};

class PointShape : public virtual Shape {
public:
    void Draw(HDC hdc) override {
        SetPixel(hdc, x1, y1, RGB(0, 0, 0));
    }
    void GetName(wchar_t* buf, int bufSize) const override {
        wcsncpy(buf, L"Крапка", bufSize);
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
    void GetName(wchar_t* buf, int bufSize) const override {
        wcsncpy(buf, L"Лінія", bufSize);
    }
};

class RectangleShape : public virtual Shape {
protected:
    virtual HBRUSH GetBrush() const {
        return CreateSolidBrush(RGB(144, 238, 144)); 
    }
public:
    void Draw(HDC hdc) override {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH hBrush = GetBrush();

        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        Rectangle(hdc, x1, y1, x2, y2);

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
        if (hBrush != GetStockObject(NULL_BRUSH)) {
            DeleteObject(hBrush);
        }
    }
    void GetName(wchar_t* buf, int bufSize) const override {
        wcsncpy(buf, L"Прямокутник", bufSize);
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
    void GetName(wchar_t* buf, int bufSize) const override {
        wcsncpy(buf, L"Еліпс", bufSize);
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
    void GetName(wchar_t* buf, int bufSize) const override {
        wcsncpy(buf, L"Лінія з кружечками", bufSize);
    }
};

class CubeShape : public RectangleShape, public LineShape {
protected:
    HBRUSH GetBrush() const override {
        return (HBRUSH)GetStockObject(NULL_BRUSH); 
    }
public:
    void Draw(HDC hdc) override {
        long ox1 = x1, oy1 = y1, ox2 = x2, oy2 = y2;

        int dx = (ox2 - ox1) / 3;
        int dy = (oy2 - oy1) / 3;

        // 1. Задня грань
        SetPoints(ox1 + dx, oy1 - dy, ox2 + dx, oy2 - dy);
        RectangleShape::Draw(hdc);

        // 2. Передня грань
        SetPoints(ox1, oy1, ox2, oy2);
        RectangleShape::Draw(hdc);

        // 3. Чотири ребра
        SetPoints(ox1, oy1, ox1 + dx, oy1 - dy);
        LineShape::Draw(hdc);

        SetPoints(ox2, oy1, ox2 + dx, oy1 - dy);
        LineShape::Draw(hdc);

        SetPoints(ox1, oy2, ox1 + dx, oy2 - dy);
        LineShape::Draw(hdc);

        SetPoints(ox2, oy2, ox2 + dx, oy2 - dy);
        LineShape::Draw(hdc);

        SetPoints(ox1, oy1, ox2, oy2);
    }
    void GetName(wchar_t* buf, int bufSize) const override {
        wcsncpy(buf, L"Куб", bufSize);
    }
};

#endif