#ifndef POINT_SHAPE_H
#define POINT_SHAPE_H

#include "shape.h"

class PointShape : public Shape {
public:
    virtual void Show(HDC hdc) override;
};

#endif