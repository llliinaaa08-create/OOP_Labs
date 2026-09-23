#ifndef LINE_SHAPE_H
#define LINE_SHAPE_H

#include "shape.h"

class LineShape : public Shape {
public:
    virtual void Show(HDC hdc) override;
};

#endif