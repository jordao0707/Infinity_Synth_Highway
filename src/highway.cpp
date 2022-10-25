// Jordão Rodrigues Dantas
// 403686
// CiÊncia da Computação
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>
#include <GL/glu.h>
#include "highway.h"

float deslocation = 0.5;
int road_length = 35;
float road_height = 1;
void drawRoad(GLuint list_id)
{
    glNewList(list_id, GL_COMPILE);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for (float i = -road_length; i < road_length; i += deslocation)
    {
        glColor3f(0.2, 0.8, 0.7);
        if (i == 0)
            continue;
        if (i == 1.5 )
        {
            glColor3f(1.0, 0.5, 0.0);
            glVertex3f(1.6, -road_height, -road_length);
            glVertex3f(1.6, -road_height, road_length);
        }
        if (i == -1.5 )
        {
            glColor3f(1.0, 0.5, 0.0);
            glVertex3f(-1.6, -road_height, -road_length);
            glVertex3f(-1.6, -road_height, road_length);
        }
        if (i >= -1.5 && i <= 1.5)
        {
            glVertex3f(i, -road_height, -road_length);
            glVertex3f(i, -road_height, road_length);
        }
    }
    for (float i = -road_length; i < road_length; i += 2 * deslocation)
    {
        glVertex3f(-road_length, -road_height, i);
        glVertex3f(road_length, -road_height, i);
    }
    glEnd();
    glEndList();
}