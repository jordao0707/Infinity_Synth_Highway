// Jordão Rodrigues Dantas - 403686
// Ciência da Computação

// LIB GRÁFICAS
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <GL/gl.h>

// LIB UTILITÁRIAS
#include <time.h>

// LIB INTERNAS
#include "./highway.h"
#include "./objectLoader.h"
#include "./car.h"

int total_objects_scenery = 12;
int total_cars_adversary = 10;

// VARIÁVEIS DE CONTROLE
ushort is_idle = 1;
float countTimer = 0;
float frameTime = 0;

// VARIÁVEIS INCREMENTAIS
float road_deslocation = 0;

// PRINCIPAIS OBJETOS PRINCIPAIS
Car *bestCar;
GLuint road_id;
GLuint sun_id;

// GRUPOS DE OBJETOS (ADVERSÁRIOS E CENÁRIO)
std::vector<Car> cars;
std::vector<GLuint> objectsScene;
GLFWwindow *window;

// FUNÇÕES
void controlCar(GLFWwindow *window, int key, int scancode, int action, int mods);
void resize(int w, int h);
void updateGameLoop();
void updateObjects();
void initObjects();
void initWindow();
void draw();
int getRandomNumber(int seed);

void initObjects()
{
    ObjectLoader *objectLoader = new ObjectLoader;

    // DEFINE OS ID INICIAL DE CADA GRUPO DE OBJETOS
    sun_id = glGenLists(total_objects_scenery + total_cars_adversary + 3);
    road_id = sun_id + 1;
    GLuint best_car_id = sun_id + 2;
    GLuint scenary_id = sun_id + 3;
    GLuint adversary_id = total_objects_scenery + 1;

    // CARREGA PISTA
    drawRoad(road_id);

    // CARREGA SOL
    objectLoader->loadObject("./objects/sphere.obj");
    objectLoader->drawObject(sun_id, vec3(0.0, 0.0, 0.0), 1);

    // CARREGA PALMEIRAS
    objectLoader->loadObject("./objects/palm.obj");
    for (int i = 0; i < total_objects_scenery; i++)
    {
        objectsScene.push_back(scenary_id + i);
        objectLoader->drawObject(objectsScene[i], vec3(0, 0, 0), 2);
    }

    // CARREGA CARROS(PRINCIPAL E OBSTACULOS)
    bestCar = new Car(best_car_id, vec3(0, 0, 0), 0, 0, 0);
    objectLoader->loadObject("./objects/car.obj");
    for (int i = 0; i < total_cars_adversary; i++)
    {
        // id da lista - posição em vec3 - posição na pista (antes, pista, depois) - cor
        cars.push_back(Car(total_objects_scenery + i, vec3(0, 0, 0), -1, (getRandomNumber(i) % 20) * 10, rand() % 4));
        objectLoader->drawObject(cars[i].getObjectId(), cars[i].getPosition(), cars[i].getColor());
    }

    // CARREGA CARRO PRINCIPAL
    objectLoader->drawObject(bestCar->getObjectId(), bestCar->getPosition(), bestCar->getColor());
}

void draw()
{
    // PISTA
    glPushMatrix();
    glTranslated(0, 0, road_deslocation);
    glCallList(road_id);
    glPopMatrix();

    // // SOL
    // glPushMatrix();
    // glCallList(sun_id);
    // glPopMatrix();

    // // PALMEIRAS
    // for (int i = 0; i < objectsScene.size(); i++)
    // {
    //     glPushMatrix();
    //     glCallList(objectsScene[i]);
    //     glPopMatrix();
    // }

    // // CARROS
    // for (int i = 0; i < cars.size(); i++)
    // {
    //     glPushMatrix();
    //     glCallList(cars[i].getObjectId());
    //     glPopMatrix();
    // }

    // // PRINCIPAL
    // glPushMatrix();
    // glCallList(best_car_id);
    // glPopMatrix();
}

int main()
{
    // INICIANDO OBJETOS
    initWindow();
    initObjects();
    float lastTime = 0.0;

    // GAME LOOP
    while (!glfwWindowShouldClose(window))
    {
        frameTime = (float)glfwGetTime() - lastTime;
        lastTime += frameTime;
        countTimer = countTimer < 10 ? countTimer + frameTime : 0;

        updateGameLoop();
        updateObjects();

        draw();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    // DELETANDO ID'S DE LISTAS
    glDeleteLists(sun_id, total_objects_scenery + total_cars_adversary + 3);
    return 0;
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    float aspect = (float)w / (float)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0 , aspect, 1.0, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(bestCar->getCarDirection() / 75, 0.0, 2.0, 0, 0, -1, bestCar->getCarDirection() / 150, 1.0, 0.0);
}

void controlCar(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    is_idle = 0;
    if (!is_idle)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // CLOSE WINDOW
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE) // ACELERA
            bestCar->carAcceleration();
        else if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE) // PARA
            bestCar->carBreake();
        else if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE) // ESQUERDA
            bestCar->carTurnsLeft(-5);
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE) // DIREITA
            bestCar->carTurnsRight(5);
        is_idle = 1;
    }
}

void updateGameLoop()
{
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    resize(width, height);
}

void updateObjects()
{
    // MOVIMENTO DA PISTA
    road_deslocation += frameTime * bestCar->getSpeed();
    road_deslocation = road_deslocation < getDeslocation() ? road_deslocation : 0;

    // PARADA LENTA
    if (is_idle && (int)countTimer % 2)
        bestCar->carDeceleration();
}

int getRandomNumber(int seed)
{
    srand(time(0) + seed);
    return (rand() + rand()) / 2;
}

void initWindow()
{
    glfwInit();
    window = glfwCreateWindow(1080, 400, "Infinity Synth Highway", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controlCar);
    glClearColor(0.6, 0.7, 0.8, 1);
    glEnable(GL_DEPTH_TEST);
}
