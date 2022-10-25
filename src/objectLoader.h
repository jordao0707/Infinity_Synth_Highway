
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/gl.h>

#include "../hearders/consts.h"
#include "../hearders/structs.h"

#pragma once
class ObjectLoader
{
public:
    void loadObject(const char *file_path);
    void drawObject(unsigned &id, vec3 pos, int color_id);

private:
    vec3 getVertex(std::string s);
    vec3 getNormal(std::string s);
    face getFace(std::string s);
};
