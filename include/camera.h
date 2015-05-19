#ifndef INCLUDE_CGFINAL_CAMERA_H
#define INCLUDE_CGFINAL_CAMERA_H

#include <cmath>

#include <gl.h>
#include <xyz.h>

class Camera {
public:
    explicit Camera();
    void Rotate(float degrees);
    void Update(GLuint program);

    void Frustrum(xyz<GLfloat, 6> proj);

private:
    xyz<GLfloat> eye_, lookat_, up_;
    xyz<GLfloat, 6> proj_;
};


Camera::Camera()
{
    eye_    = { 1.0f, 2.0f, 3.0f };
    lookat_ = { 0.0f, 0.0f, 0.0f };
    up_     = { 0.0f, 1.0f, 0.0f };
    proj_   = {
        -1.0f, // left
        1.0f , // right
        1.0f , // top
        -1.0f, // bottom
        3.0f , // near
        100.5f // far
    };
}


void Camera::Rotate(float degrees)
{
    GLfloat theta = degrees * (M_PI / 180);
    lookat_[0] += sin(theta);
}

void Camera::Update(GLuint program)
{
    glUseProgram(program);

    // set up the camera
    glUniform3fv(glGetUniformLocation(program, "cPosition"), 1, eye_.data());
    glUniform3fv(glGetUniformLocation(program, "cLookAt"  ), 1, lookat_.data());
    glUniform3fv(glGetUniformLocation(program, "cUp"      ), 1, up_.data());

    glUniform1f(glGetUniformLocation(program, "left")  , proj_[0]);
    glUniform1f(glGetUniformLocation(program, "right" ), proj_[1]);
    glUniform1f(glGetUniformLocation(program, "top")   , proj_[2]);
    glUniform1f(glGetUniformLocation(program, "bottom"), proj_[3]);
    glUniform1f(glGetUniformLocation(program, "near")  , proj_[4]);
    glUniform1f(glGetUniformLocation(program, "far")   , proj_[5]);
}


void Camera::Frustrum(xyz<GLfloat, 6> proj)
{
    proj_ = proj;
}


#endif