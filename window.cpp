#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>
#include <GL/glu.h>
#include "highway.h"

GLFWwindow *window;
GLuint list_id;
float speed = 2;
float road_deslocation = 0;

void controlCar(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    
    // speed = speed > 0 ? speed - 0.005 : 0 ;
 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_REPEAT)
    {
        speed = speed < 20 ? speed + 0.09 : speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_REPEAT)
    {
        speed = speed > 0 ? speed - 1 : 0;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_REPEAT)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_REPEAT)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    float aspect = (float)w / (float)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0 + (speed * 0.5), aspect, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0, 0.0, 2,
        0, 0, -1,
        0, 1, 0);
    
}

void initWindow()
{
    glfwInit();
    window = glfwCreateWindow(400, 400, "Infinity Synth Highway", NULL, NULL);
    glfwMakeContextCurrent(window);
    std::cout << "Company " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Model " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    glfwSetKeyCallback(window, controlCar);
    glClearColor(0.2, 0.0, 0.3, 1);
    glEnable(GL_DEPTH_TEST);
}

void initObjects()
{
    list_id = glGenLists(1);
    drawRoad(list_id);
}

void draw(float frameTime)
{
    road_deslocation += frameTime * speed;
    
    if (road_deslocation >= 1)
        road_deslocation = 0;
    glTranslated(0, 0, road_deslocation);
    glCallList(list_id); // terreno
}

int main()
{
    initWindow();
    initObjects();
    float currentTime, lastTime = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        currentTime = (float)glfwGetTime();
        float frameTime = currentTime - lastTime;
        lastTime = currentTime;
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        resize(width, height);
        draw(frameTime);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    glDeleteLists(list_id, 1);
    return 0;
}