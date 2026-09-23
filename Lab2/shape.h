#ifndef SHAPE_H
#define SHAPE_H

#include <windows.h>

class Shape {
protected:
    long xs1, ys1, xs2, ys2;
public:
    virtual ~Shape() {}
    virtual void Set(long x1, long y1, long x2, long y2) {
        xs1 = x1; ys1 = y1; xs2 = x2; ys2 = y2;
    }
    virtual void Show(HDC hdc) = 0;
};

#endif