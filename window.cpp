#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>

#include "highway.h"
#include "objectLoader.h"

int total_objects_scenery = 12;
int total_obstacles = 8;

float road_deslocation = 0;
float sun_deslocation = 0;
float car_direction = 0;
float speed = 0;

GLuint road_id;
GLuint best_car_id;
GLuint sun_id;

std::vector<GLuint> objectsScene;
std::vector<GLuint> carsObjects;
std::vector<GLuint> palmsObjects;

GLFWwindow *window;

void controlCar(GLFWwindow *window, int key, int scancode, int action, int mods)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_REPEAT)
    {
        speed = speed < 30 ? speed + 0.09 : speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_REPEAT)
    {
        speed = speed > 0.5 ? speed - 0.1 : 0;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_REPEAT)
    {
        car_direction = car_direction > -4.5 ? car_direction - 0.1 * (speed * speed) / 100 : -4.5;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_REPEAT)
    {
        car_direction = car_direction < 4.5 ? car_direction + 0.1 * (speed * speed) / 100 : 4.5;
    }
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    float aspect = (float)w / (float)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0 + (speed * 0.5), aspect, 1.0, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        car_direction / 24, 0.0, 2,
        0, 0, -1,
        car_direction / 60, 1, 0);
}

void initWindow()
{
    glfwInit();
    window = glfwCreateWindow(1080, 400, "Infinity Synth Highway", NULL, NULL);
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
    ObjectLoader *obj = new ObjectLoader;
    vec3 pos;

    road_id = glGenLists(total_objects_scenery + total_obstacles + 3);
    best_car_id = road_id + 1;
    sun_id = road_id + 2;

    drawRoad(road_id);
    
    // CARREGA CENARIO 
    obj->loadObject("./objects/sphere.obj");
    obj->drawObject(sun_id, vec3(0.0, 0.0, 0.0), 1);

    obj->loadObject("./objects/palm.obj");
    for (int i = 0; i < total_objects_scenery; i++)
    {
        srand(time(0) + (i * 10));
        float pos_x = -0.1 + ((float)((rand() + rand()) % 200)) / 45;
        float pos_z = -0.1 + ((float)((rand() + rand()) % 20)) / 19;

        if (pos_x <= pos.x && (pos_x + 0.4) > pos.x)
        {
            pos_x = -2.0 + ((float)(rand() % 200)) / 45;
        }
        pos = vec3(pos_x, -0.21 + pos_z / 40, -4 + pos_z);
        GLuint temp_id = sun_id + 1 + i;
        objectsScene.push_back(temp_id);
        obj->drawObject(objectsScene[i], pos, 2);
    }

    // CARREGA CARROS(PRINCIPAL E OBSTACULOS)
    obj->loadObject("./objects/car.obj");
    obj->drawObject(best_car_id, vec3(0, 0, 0), 0);
    for (int i = 0; i < total_obstacles; i++)
    {
        srand(time(0) + (i * 10));
        float pos_x = -1 + ((float)((rand() + rand()) % 200)) / 25;
        float pos_z = 100 + ((float)((rand() + rand()) % 200)) / 40;

        pos = vec3(pos_x, -4, -pos_z);
        GLuint temp_id = total_objects_scenery + 1 + i;
        carsObjects.push_back(temp_id);
        obj->drawObject(carsObjects[i], pos, 1 + rand()%3);
    }
}

void draw(float frameTime)
{

    speed = speed > 0 ? speed - 0.0085 : 0;
    road_deslocation += frameTime * speed;
    sun_deslocation = sun_deslocation <= 360 ? sun_deslocation + (frameTime * 20) : 0;
    if (road_deslocation >= 1.1)
        road_deslocation = 0;

    glPushMatrix();
    glScaled(40, 40, 40);
    glTranslated(0, -0.1, -10.0);
    glRotated(sun_deslocation, 1, 0.2, 0.5);
    glCallList(sun_id); // sol
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0, road_deslocation);
    glCallList(road_id); // pista
    glPopMatrix();
    glPushMatrix();
    glScaled(0.1, 0.1, 0.1);
    glTranslated(car_direction, -6.0, 5.0);
    glCallList(best_car_id); // carro
    glPopMatrix();

    // CENÁRIO
    for (int i = 0; i < objectsScene.size(); i++)
    {
        GLuint palm_id = objectsScene[i];
        glPushMatrix();
        glScaled(5, 5, 5);

        glRotated((palm_id)*360, 0, 1, 0);
        glCallList(palm_id); // palms
        glPopMatrix();
    }

    // OBSTACULOS
    for (int i = 0; i < carsObjects.size(); i++)
    {
        GLuint car_id = carsObjects[i];
        glPushMatrix();
        glScaled(0.1, 0.1, 0.1);
        glCallList(car_id); // cars
        glPopMatrix();
    }
}

int main()
{

    // ISoundEngine *SoundEngine = createIrrKlangDevice();

    initWindow();
    initObjects();
    float currentTime, lastTime = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        currentTime = (float)glfwGetTime();
        float frameTime = currentTime - lastTime;
        lastTime = currentTime;

        glfwPostEmptyEvent();
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        draw(frameTime);
        resize(width, height);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    glDeleteLists(best_car_id, 1);
    return 0;
}