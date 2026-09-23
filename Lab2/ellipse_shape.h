#ifndef ELLIPSE_SHAPE_H
#define ELLIPSE_SHAPE_H

#include "shape.h"

class EllipseShape : public Shape {
public:
    virtual ~EllipseShape() {}
    virtual void Show(HDC hdc) override;
};

#endif