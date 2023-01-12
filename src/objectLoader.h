#ifndef OBJECT_LOADER_H_
#define OBJECT_LOADER_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/gl.h>

#include "../hearders/structs.h"

#pragma once
class ObjectLoader
{
public:
    void loadObject(const char *file_path);
    void drawObject(GLuint id, vec3 pos, int color_id, vec3 ligth, vec3 cam);

private:
    // PROPRIEDADES DO OBJ
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<face> faces;

    // METODOS PRE-PROCESSAMENTO
    vec3 getVertex(std::string s);
    vec3 getNormal(std::string s);
    face getFace(std::string s);
};
#endif