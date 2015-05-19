//
//  CG Final Project
//  Denis Coady, May 18, 2015
//

#include <cstdlib>
#include <iostream>

#include <gl.h>
#include <object/rectangle.h>
#include <scene.h>

#include <shader.h>

#include <camera.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// buffer information
GLuint program;

// shader arguments...rotation of object.
GLuint theta; // theta uniform location
float angles[3] = {30.0, 30.0, 0.0};
float angleInc = 5.0;

Camera camera;
Scene scene;
Rectangle r1, r2;



// OpenGL initialization
void init() {

    program = glCreateProgram();
    // Load shaders and use the resulting shader program
    Shader shading(program, "glsl/shader.vert", "glsl/shader.frag");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // initally create a cube
    r1 = Rectangle(xyz<GLfloat>{-0.5, +0.5, +0.0}, xyz<GLfloat>{-0.5, -0.5, +0.0},
                   xyz<GLfloat>{+0.5, -0.5, +0.0}, xyz<GLfloat>{+0.5, +0.5, +0.0});

    r2 = Rectangle(xyz<GLfloat>{-0.25, +0.25, +0.0}, xyz<GLfloat>{-0.25, -0.25, +0.0},
                   xyz<GLfloat>{+0.25, -0.25, +0.0}, xyz<GLfloat>{+0.25, +0.25, +0.0});
}


void display( void ) {

    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    scene.Draw(program);
    camera.Update(program);
    // swap the framebuffers
    glutSwapBuffers();
}


















// Causes gimbal lock which also happened on Apollo 11
// http://en.wikipedia.org/wiki/Gimbal_lock#Gimbal_lock_on_Apollo_11
// Solution? Use Quaternions (Taught in Comp. Animation: Algorithms)
//
// TIDBIT:
// Quaternion plaque on Brougham (Broom) Bridge, Dublin, which says:
// 
//    "Here as he walked by
//    on the 16th of October 1843
// 
//    Sir William Rowan Hamilton 
//    
//    in a flash of genius discovered
//    the fundamental formula for
//    quaternion multiplication
//    i^2 = j^2 = k^2 = ijk = -1
//    & cut it on a stone of this bridge"

void animate() {
    int i;

    for( i = 0; i < 150; i++ ) {
        angles[0] -= angleInc / 3;
        display();
    }
    for( i = 0; i < 150; i++ ) {
        angles[1] -= angleInc / 3;
        display();
    }
    for( i = 0; i < 150; i++ ) {
        angles[2] -= angleInc / 3;
        display();
    }

}


void keyboard( unsigned char key, int x, int y ) {

    switch( key ) {

	// automated animation
        case 'a': animate(); break;

	// incremental rotation along the axes
        case 'x': angles[0] -= angleInc; break;
        case 'y': angles[1] -= angleInc; break;
        case 'z': angles[2] -= angleInc; break;
        case 'X': angles[0] += angleInc; break;
        case 'Y': angles[1] += angleInc; break;
        case 'Z': angles[2] += angleInc; break;

    // add/remove
        case '1': scene.Add(&r1); break;
        case '2': scene.Add(&r2); break;
        case '3': scene.Del(&r1); break;
        case '4': scene.Del(&r2); break;

	// termination
        case 033:  // Escape key
        case 'q': case 'Q':
            exit( 0 );
    }

    glutPostRedisplay();
}


int main( int argc, char **argv ) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("CG - Final Assignment");

#ifndef __APPLE__
    glewInit();
#endif

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
