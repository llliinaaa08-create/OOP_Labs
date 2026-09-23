#include "ellipse_shape.h"
#include <cmath>

void EllipseShape::Show(HDC hdc) {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); 
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    long rx = std::abs(xs2 - xs1);
    long ry = std::abs(ys2 - ys1);

    Ellipse(hdc, xs1 - rx, ys1 - ry, xs1 + rx, ys1 + ry);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}