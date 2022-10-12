#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>
#include <GL/glu.h>
// #include "highway.h"
GLFWwindow *window;


// void controlCam(GLFWwindow *window, int key, int scancode, int action, int mods)
// {
//     if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
//         glfwSetWindowShouldClose(window, GLFW_TRUE);
//     }
//     if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_A) == GLFW_REPEAT){
//         glfwSetWindowShouldClose(window, GLFW_TRUE);
//     }
//     if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_D) == GLFW_REPEAT){
//         glfwSetWindowShouldClose(window, GLFW_TRUE);
//     }
//     if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_W) == GLFW_REPEAT){
//         glfwSetWindowShouldClose(window, GLFW_TRUE);
//     }
//     if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_S) == GLFW_REPEAT){
//         glfwSetWindowShouldClose(window, GLFW_TRUE);
//     }
//  }

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    float aspect = (float)w / (float)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 500.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(
        22.0,10.0,2.0,
        0, 2, 0,
        0, 1, 0);

    glLoadIdentity();
}

void initWindow()
{
    glfwInit();
    window = glfwCreateWindow(400, 400, "Infinity Synth Highway", NULL, NULL);
    glfwMakeContextCurrent(window);
    std::cout << "Company " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Model " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    // glfwSetKeyCallback(window, controlCam);
    glClearColor(0.2, 0.0, 0.3, 1);
    glEnable(GL_DEPTH_TEST);
}

GLuint list_id;
void initObjects(){
    list_id = glGenLists(1);
    // drawRoad(list_id);
}
int deslocation = 1;
int road_length = 500;
float road_height = 0.5;
void draw()
{
    glBegin(GL_LINES);
    for(int i = -road_length; i< road_length; i+= deslocation){
        glVertex3f(i, road_height, -road_length);
        glVertex3f(i, road_height, road_length);
        glVertex3f(-road_length, road_height, i);
        glVertex3f(road_length, road_height, i);
    }
    glEnd();
    //    glCallList(list_id); // terreno

}

int main()
{
    initWindow();
    initObjects();
    while (!glfwWindowShouldClose(window))
    {
 
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        resize(width, height);
        draw();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
     glDeleteLists(list_id,1);
    return 0;
}