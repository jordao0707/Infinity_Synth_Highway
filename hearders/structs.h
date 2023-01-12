#ifndef STRUCT_H_
#define STRUCT_H_
#include <GL/gl.h>

#include <glm/glm.hpp>

using namespace glm;
// typedef struct vec3
// {
//     float x, y, z;
//     vec3() : x(0.0), y(0.0), z(0.0) {}
//     vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
//     vec3 operator+(const vec3 &v) const
//     {
//         return vec3(x + v.x, y + v.y, z + v.z);
//     }
//     vec3 operator*(float k)
//     {
//         return vec3(k * x, k * y, k * z);
//     }
// } vec3;

typedef struct material
{
    vec3 ambient, diffuse, specular;
    float shine;
    material(vec3 a_, vec3 d_, vec3 s_, float sh_) : ambient(a_), diffuse(d_), specular(s_) { shine = sh_; }
} material;

typedef struct Cam
{
    vec3 pos, look, top;
    Cam() : pos(vec3(0.0, 0.0, 0.0)), look(vec3(0.0, 0.0, 0.0)), top(vec3(0.0, 0.0, 0.0)) {}
    Cam(vec3 pos_, vec3 look_, vec3 top_) : pos(pos_), look(look_), top(top_) {}
} Cam;

typedef struct Object
{
    GLuint id;
    vec3 pos;
    Object() : id(0), pos(vec3(0.0, 0.0, 0.0)) {}
} Object;

typedef struct face
{
    unsigned index;
    int vertex[4];
    face(int v1, int v2, int v3, int v4, int i)
    {
        vertex[0] = v1;
        vertex[1] = v2;
        vertex[2] = v3;
        vertex[3] = v4;
        index = i;
    }
} face;

#endif