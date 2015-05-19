#version 120

// Vertex location (in model space)
attribute vec4 vPosition;

// Normal vector at vertex (in model space)
attribute vec3 vNormal;

// Camera parameters
uniform vec3 cPosition;
uniform vec3 cLookAt;
uniform vec3 cUp;

// View volume boundaries
uniform float left;
uniform float right;
uniform float top;
uniform float bottom;
uniform float near;
uniform float far;

void main()
{
    // Create view matrix
    vec3 nVec = normalize(cPosition - cLookAt);
    vec3 uVec = normalize(cross(normalize(cUp), nVec));
    vec3 vVec = normalize(cross(nVec, uVec));

    mat4 viewMat = mat4(uVec.x, vVec.x, nVec.x, 0.0,
                        uVec.y, vVec.y, nVec.y, 0.0,
                        uVec.z, vVec.z, nVec.z, 0.0,
                        -1.0*(dot(uVec, cPosition)),
                        -1.0*(dot(vVec, cPosition)),
                        -1.0*(dot(nVec, cPosition)), 1.0);


        // Create projection matrix
    mat4 projMat = mat4
    (
        (2.0*near)/(right-left)  , 0.0                      , 0.0                         , 0.0 ,
        0.0                      , (2.0*near)/(top-bottom)  , 0.0                         , 0.0 ,
        (right+left)/(right-left), (top+bottom)/(top-bottom), (-1.0*(far+near))/(far-near), -1.0,
        0.0                      , 0.0                      , (-2.0*far*near)/(far-near)  , 0.0
    );

    gl_Position = projMat * viewMat * vPosition;
}
