#ifndef RECT_SHAPE_H
#define RECT_SHAPE_H

#include "shape.h"

class RectangleShape : public Shape {
public:
    virtual void Show(HDC hdc) override;
};

#endif