// Jordão Rodrigues Dantas
// 403686
// CiÊncia da Computação
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>
#include <GL/glu.h>
#include "highway.h"

int road_length = 50;
float deslocation = 10;

void drawRoad(GLuint list_id)
{
    glNewList(list_id, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0);

    for (float i = -road_length; i < road_length; i += deslocation)
    {
        glVertex3f(-50, -2, i);
        glVertex3f(50, -2, i);
    }

    glEnd();
    glEndList();
}
float getDeslocation()
{
    return deslocation;
}
float getRoadLength()
{
    return road_length;
}
