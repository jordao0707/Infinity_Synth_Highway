// #include <GLFW/glfw3.h>
// #include <GL/gl.h>
// #include <iostream>
// #include <GL/glu.h>

// int deslocation = 1;
// int road_length = 500;
// float road_height = 0.5;
// void drawRoad(GLuint list_id){
//     glNewList(list_id, GL_COMPILE);
//     glColor3f(1.0,1.0,1.0);
//     glBegin(GL_LINES);
//     for(int i = -road_length; i< road_length; i+= deslocation){
//         glVertex3f(i, road_height, -road_length);
//         glVertex3f(i, road_height, road_length);
//         glVertex3f(-road_length, road_height, i);
//         glVertex3f(road_length, road_height, i);
//     }
//     glEnd();
//     glEndList();
// }