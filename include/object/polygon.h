#ifndef INCLUDE_CGFINAL_OBJECT_POLYGON_H
#define INCLUDE_CGFINAL_OBJECT_POLYGON_H

#include <vector>

#include "gl.h"
#include "xyz.h"

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class Polygon {
public:
    void AddTriangle(xyz<GLfloat> a, xyz<GLfloat> b, xyz<GLfloat> c);
    void Create();
    void Bind(GLuint program);

    std::vector<GLfloat>  Verticies() const;
    std::vector<GLfloat>  Normals()   const;
    std::vector<GLushort> Elements()  const;

    void Draw(GLuint program);

private:
    std::vector<GLfloat> points_;
    std::vector<GLfloat> normals_;

    GLuint vbuffer_;
    GLuint ebuffer_;

    bool initialized_ = false;
};




void Polygon::AddTriangle(xyz<GLfloat> a, xyz<GLfloat> b, xyz<GLfloat> c) {

    // calculate the normal
    float ux = b[0] - a[0];
    float uy = b[1] - a[1];
    float uz = b[2] - a[2];

    float vx = c[0] - a[0];
    float vy = c[1] - a[1];
    float vz = c[2] - a[2];

    float nx = (uy * vz) - (uz * vy);
    float ny = (uz * vx) - (ux * vz);
    float nz = (ux * vy) - (uy * vx);

    float n[3] = { nx, ny, nz };


    for (int i = 0; i < 3; i++) points_.push_back(a[i]);
                                points_.push_back(1.0f);
    for (int i = 0; i < 3; i++) points_.push_back(b[i]);
                                points_.push_back(1.0f);
    for (int i = 0; i < 3; i++) points_.push_back(c[i]);
                                points_.push_back(1.0f);

    for (int i = 0; i < 3; i++) normals_.push_back(n[i]);
    for (int i = 0; i < 3; i++) normals_.push_back(n[i]);
    for (int i = 0; i < 3; i++) normals_.push_back(n[i]);
}


std::vector<GLfloat> Polygon::Verticies() const {
    return points_;
}

std::vector<GLfloat> Polygon::Normals() const {
    return normals_;
}

std::vector<GLushort> Polygon::Elements() const {
    std::vector<GLushort> elements(points_.size());

    for (int i = 0; i < points_.size(); i++) {
        elements[i] = i;
    }

    return elements;
}


void Polygon::Create()
{
    std::vector<float>    points   = Verticies();
    std::vector<float>    normals  = Normals();
    std::vector<GLushort> elements = Elements();


    if (initialized_)
    {
        glDeleteBuffers(1, &vbuffer_);
    }
    glGenBuffers(1, &vbuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(points) * sizeof(float),
        &points[0],
        GL_STATIC_DRAW
    );

    if (initialized_)
    {
        glDeleteBuffers(1, &ebuffer_);
    }
    glGenBuffers(1, &ebuffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(points) * sizeof (GLushort) / 4,
        &elements[0],
        GL_STATIC_DRAW
    );



    initialized_ = true;


}


void Polygon::Bind(GLuint program)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer_);

    // calculate the number of bytes of vertex data
    int size_data = points_.size() * sizeof(float);

    // set up the vertex attribute variables
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    GLuint vNormal   = glGetAttribLocation(program, "vNormal");

    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(size_data));
}

void Polygon::Draw(GLuint program)
{
    int num_verticies = points_.size() / 4;

    // calculate the number of bytes of vertex data
    std::vector<float>  points     = Verticies();
    std::vector<float>  normals    = Normals();
    std::vector<GLushort> elements = Elements();

    int size_data = points.size() * sizeof(float);

    // bind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer_);

    glUseProgram(program);

    // set up the vertex attribute variables
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    GLuint vNormal   = glGetAttribLocation(program, "vNormal");

    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(
        vPosition,  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        4,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        BUFFER_OFFSET(0)
    );

    glEnableVertexAttribArray(vNormal);
        glVertexAttribPointer(
        vNormal,    // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        BUFFER_OFFSET(size_data)
    );

    glDrawElements(GL_TRIANGLES, num_verticies,
                   GL_UNSIGNED_SHORT, nullptr);
}

#endif