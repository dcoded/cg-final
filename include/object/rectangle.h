#ifndef INCLUDE_CGFINAL_OBJECT_TRIANGLE_H
#define INCLUDE_CGFINAL_OBJECT_TRIANGLE_H

#include "polygon.h"

class Rectangle : public Polygon {
public:
    Rectangle();
    Rectangle(xyz<GLfloat> const a, xyz<GLfloat> const b,
              xyz<GLfloat> const c, xyz<GLfloat> const d);
};

Rectangle::Rectangle() {}

Rectangle::Rectangle(xyz<GLfloat> const a, xyz<GLfloat> const b,
                     xyz<GLfloat> const c, xyz<GLfloat> const d)
{
    AddTriangle(a,b,d);
    AddTriangle(b,c,d);
    Create();
}
#endif