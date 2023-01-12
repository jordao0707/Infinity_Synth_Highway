// Jordão Rodrigues Dantas - 403686
// Ciência da Computação

// LIB GRÁFICAS
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <algorithm>

// LIB UTILITÁRIAS
#include <time.h>

// LIB INTERNAS
#include "./highway.h"
#include "./objectLoader.h"
#include "./car.h"

// LUZ
vec3 light = vec3(0, 0, -20);
// VARIAVEIS
int total_objects_scenery = 12;
int total_cars_adversary = 10;
ushort is_dev_mode = 0;
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

// GLFW
GLFWwindow *window;

// ANGULOS DE CAMERA
ushort select_cam = 0;
std::vector<Cam> cameras = {
    Cam(vec3(0.2, 0.4, -2.0), vec3(0.2, 0.4, -5.0), vec3(0.0, 1.0, 0.0)),       // MAIN
    Cam(vec3(0.0, 0.0, 12.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0)),        // FAR NORMAL
    Cam(vec3(-1.8, 0.2, -0.2), vec3(-1.8, 0.8, -15.0), vec3(0.0, 1.0, 0.0)),    // LEFT NORMAL
    Cam(vec3(70.0, 3.0, -5.0), vec3(6.0, 1.0, -15.0), vec3(0.0, 1.0, 0.0)),     // FAR LEFT
    Cam(vec3(-70.0, 3.0, -5.0), vec3(-6.0, -1.0, -15.0), vec3(0.0, 1.0, 0.0)),  // FAR RIGHT
    Cam(vec3(-0.2, -0.8, -13.0), vec3(-0.2, -0.8, -11.0), vec3(0.0, 1.0, 0.0)), // FRONT

};

// FUNÇÕES
void controlCar(GLFWwindow *window, int key, int scancode, int action, int mods);
void resize(int w, int h);
void updateGameLoop();
// void updateObjects();
void initObjects();
void initWindow();
void controlCam();
void draw();
double getRandomNumber(int seed, float init, float end);

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
    objectLoader->drawObject(sun_id, vec3(0.0, 0.0, 0.0), 1, light, cameras[0].pos);

    // CARREGA PALMEIRAS
    objectLoader->loadObject("./objects/palm.obj");
    for (int i = 0; i < total_objects_scenery; i++)
    {
        objectsScene.push_back(scenary_id + i);
        objectLoader->drawObject(objectsScene[i], vec3(0, 0, 0), 2, light, cameras[0].pos);
    }

    // CARREGA CARROS(PRINCIPAL E OBSTACULOS)
    bestCar = new Car(best_car_id, vec3(0, -2, -10), 0, 3);
    objectLoader->loadObject("./objects/car.obj");
    for (int i = 0; i < total_cars_adversary; i++)
    {
        // id da lista - posição em vec3, velocidade - cor
        vec3 pos = vec3(getRandomNumber(i, -5, 5), -2, -50);
        cars.push_back(Car(total_objects_scenery + i, pos, ((int)getRandomNumber(i, 0, 50)), rand() % 3));
        objectLoader->drawObject(cars[i].getObjectId(), cars[i].getPosition(), cars[i].getColor(), light, cameras[0].pos);
    }

    // CARREGA CARRO PRINCIPAL
    objectLoader->drawObject(bestCar->getObjectId(), bestCar->getPosition(), bestCar->getColor(), light, cameras[0].pos);
}

void draw()
{
    // PISTA
    // glPushMatrix();
    // glTranslated(0, 0, road_deslocation);

    // glCallList(road_id);
    // glPopMatrix();

    // SOL
    glPushMatrix();
    glScaled(10.0, 10.0, 10.0);
    glTranslated(0.0, 0.0, -10);
    glCallList(sun_id);
    glPopMatrix();

    // // PALMEIRAS
    // for (int i = 0; i < objectsScene.size(); i++)
    // {
    //     glPushMatrix();
    //     glCallList(objectsScene[i]);
    //     glPopMatrix();
    // }

    // CARROS

    // for (Car car : cars)
    // {
    //     glPushMatrix();
    //     glScaled(0.5, 0.5, 0.5);
    //     vec3 pos = car.getPosition();
    //     float adversary_speed = car.getSpeed();
    //     float best_speed = bestCar->getSpeed();
    //     car.setDeslocation(frameTime, best_speed);
    //     glTranslated(pos.x, pos.y, pos.z);
    //     // if (car.getPositionOnRoad() == ON)
    //     {
    //         glTranslated(pos.x, pos.y, pos.z + car.getDeslocation());
    //         glCallList(car.getObjectId());
    //     }
    //     glPopMatrix();
    // }

    // PRINCIPAL
    glPushMatrix();
    glScaled(0.01, 0.01, 0.01);
    glTranslated(bestCar->getCarDirection(), -2, -5);
    glCallList(bestCar->getObjectId());
    glPopMatrix();
}

double getRandomNumber(int seed, float init, float end)
{
    srand(time(0) + seed);
    double r = (double)rand() / RAND_MAX;
    return init + r * (end - init);
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
        // calcula tempo de frame
        frameTime = (float)glfwGetTime() - lastTime;
        lastTime += frameTime;
        countTimer = countTimer < 20 ? countTimer + frameTime : 0;
        // atualiza estados

        updateGameLoop();
        // updateObjects();
        // desenha e limpa tela
        draw();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    // DELETANDO ID'S DE LISTAS
    glDeleteLists(sun_id, total_objects_scenery + total_cars_adversary + 2);
    cars.clear();
    objectsScene.clear();
    free(bestCar);

    return 0;
}

void resize(int w, int h)
{

    glViewport(0, 0, w, h);
    float aspect = (float)w / (float)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    vec3 pos = cameras[select_cam].pos;
    vec3 look = cameras[select_cam].look;
    vec3 top = cameras[select_cam].top;
    gluLookAt(pos.x, pos.y, pos.z, look.x, look.y, look.z, top.x, top.y, top.z);
}

void controlCar(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    is_idle = 0;
    if (!!432)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // CLOSE WINDOW
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE) // ACELERA
            bestCar->carAcceleration();
        if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE) // PARA
            bestCar->carBreake();
        if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE) // ESQUERDA
            bestCar->carTurnsLeft(-5);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE) // DIREITA
            bestCar->carTurnsRight(5);
        // DEV MODE
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_RELEASE &&
            glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_RELEASE)
        {
            is_dev_mode = !is_dev_mode;
            std::cout << "\n\n\n DEV MODE " << (is_dev_mode ? "ON" : "OFF") << "\n\n\n";
        }
        if (is_dev_mode)
            controlCam();
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

// void updateObjects()
// {
//     // MOVIMENTO DA PISTA
//     road_deslocation += frameTime * bestCar->getSpeed();
//     road_deslocation = road_deslocation < getDeslocation() ? road_deslocation : 0;

//     if (!((int)countTimer % 10))
//     {
//         int r = (int)getRandomNumber((int)countTimer, 0, cars.size());
//         cars[r].setPositionOnRoad(ON);
//     }

//     // PARADA LENTA
//     if (is_idle && (int)countTimer % 2)
//         bestCar->carDeceleration();
// }

void initWindow()
{
    glfwInit();
    window = glfwCreateWindow(1080, 400, "Infinity Synth Highway", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controlCar);
    glClearColor(0.6, 0.7, 0.8, 1);
    glEnable(GL_DEPTH_TEST);
}

void controlCam()
{
    // DEBUG
    if (glfwGetKey(window, GLFW_KEY_TAB) != GLFW_RELEASE) // TROCA DE CAMERA
        select_cam = select_cam < cameras.size() - 1 ? select_cam + 1 : 0;

    // CONTROLE DE CAMERA POSIÇÂO
    vec3 pos = cameras[select_cam].pos;
    vec3 look = cameras[select_cam].look;
    vec3 top = cameras[select_cam].top;

    if (glfwGetKey(window, GLFW_KEY_A) != GLFW_RELEASE)
        pos.x += 0.2;
    if (glfwGetKey(window, GLFW_KEY_D) != GLFW_RELEASE)
        pos.x -= 0.2;
    if (glfwGetKey(window, GLFW_KEY_W) != GLFW_RELEASE)
        pos.y += 0.2;
    if (glfwGetKey(window, GLFW_KEY_S) != GLFW_RELEASE)
        pos.y -= 0.2;
    if (glfwGetKey(window, GLFW_KEY_Q) != GLFW_RELEASE)
        pos.z += 0.2;
    if (glfwGetKey(window, GLFW_KEY_E) != GLFW_RELEASE)
        pos.z -= 0.2;

    // CONTROLE DE CAMERA LOOK
    if (glfwGetKey(window, GLFW_KEY_I) != GLFW_RELEASE)
        look.x += 0.2;
    if (glfwGetKey(window, GLFW_KEY_K) != GLFW_RELEASE)
        look.x -= 0.2;
    if (glfwGetKey(window, GLFW_KEY_J) != GLFW_RELEASE)
        look.y += 0.2;
    if (glfwGetKey(window, GLFW_KEY_L) != GLFW_RELEASE)
        look.y -= 0.2;
    if (glfwGetKey(window, GLFW_KEY_U) != GLFW_RELEASE)
        look.z += 2;
    if (glfwGetKey(window, GLFW_KEY_O) != GLFW_RELEASE)
        look.z -= 2;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        pos = vec3(0.0, 0.0, 0.0);
        look = vec3(0.0, 0.0, 0.0);
    }
    cameras[select_cam].pos = pos;
    cameras[select_cam].look = look;
    cameras[select_cam].top = top;
    std ::cout << "posi x:" << pos.x << " y:" << pos.y << " z:" << pos.z << "\n";
    std ::cout << "look x:" << look.x << " y:" << look.y << " z:" << look.z << "\n \n";
}

// void resize(int w, int h)
// {
//     glViewport(0, 0, w, h);
//     float aspect = (float)w / (float)h;
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     gluPerspective(60.0, aspect, 1.0, 400.0);
//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();
//     gluLookAt(4, 4, 4, 0.0, 0.0, 0.0, 0, 1, 0);
// }

// glm::vec3 p_light = glm::vec3(4, 2, 5);
// glm::vec3 p_cam = glm::vec3(7, 7, 5);

// glm::vec3 n1 = glm::vec3(1, 0, 0);
// glm::vec3 n2 = glm::vec3(0, 1, 0);
// glm::vec3 n3 = glm::vec3(0, 0, 1);

// glm::vec3 Ka = glm::vec3(0.2, 0.2, 0.1);
// glm::vec3 Kd = glm::vec3(0.4, 0.9, 0.1);
// glm::vec3 Ks = glm::vec3(0.2, 1.0, 1.0);

// glm::vec3 Ia = glm::vec3(0.1, 0.1, 0.1);
// glm::vec3 Id = glm::vec3(0.8, 0.8, 0.8);
// glm::vec3 Is = glm::vec3(0.3, 0.2, 1.0);

// glm::vec3 a = Ka * Ia;
// glm::vec3 d = Kd * Id;
// glm::vec3 s = Ks * Is;

// float max(float a, float b)
// {
//     if (a > b)
//         return a;
//     else
//         return b;
// }

// void draw()
// {

//     glBegin(GL_LINES);
//     glColor3d(1, 0, 0);
//     glVertex3d(0.0, 0.0, 0.0);
//     glVertex3d(p_light.x, p_light.y, p_light.z);
//     glEnd();
//     glm::vec3 l = glm::normalize(p_light - n2);
//     glm::vec3 v = glm::normalize(p_cam - n2);
//     glm::vec3 r = (2 * glm::dot(l, n2) * n2) - l;
//     glm::vec3 ambiente = a;
//     glm::vec3 difusa = max(glm::dot(l, n2), 0) * d;
//     glm::vec3 especular = (float)pow(max(glm::dot(r, v), 0), 2) * s;

//     glm::vec3 cor = ambiente + difusa + especular;

//     // 1
//     glColor3d(cor.r, cor.g, cor.b);
//     glBegin(GL_POLYGON);
//     glVertex3d(1.0, 1.0, -1.0);
//     glVertex3d(-1.0, 1.0, -1.0);
//     glVertex3d(-1.0, 1.0, 1.0);
//     glVertex3d(1.0, 1.0, 1.0);
//     glEnd();

//     l = glm::normalize(p_light - n1);
//     v = glm::normalize(p_cam - n1);
//     r = (2 * glm::dot(l, n1) * n1) - l;
//     difusa = max(glm::dot(l, n1), 0) * d;
//     especular = (float)pow(max(glm::dot(r, v), 0), 2) * s;
//     cor = ambiente + difusa + especular;

//     // 2
//     glColor3d(cor.r, cor.g, cor.b);
//     glBegin(GL_POLYGON);
//     glVertex3d(1.0, -1.0, -1.0);
//     glVertex3d(1.0, 1.0, -1.0);
//     glVertex3d(1.0, 1.0, 1.0);
//     glVertex3d(1.0, -1.0, 1.0);
//     glEnd();

//     l = glm::normalize(p_light - n3);
//     v = glm::normalize(p_cam - n3);
//     r = (2 * glm::dot(l, n3) * n3) - l;
//     difusa = max(glm::dot(l, n3), 0) * d;
//     especular = (float)pow(max(glm::dot(r, v), 0), 2) * s;
//     cor = ambiente + difusa + especular;

//     // 3
//     glColor3d(cor.r, cor.g, cor.b);
//     glBegin(GL_POLYGON);
//     glVertex3d(1.0, 1.0, 1.0);
//     glVertex3d(-1.0, 1.0, 1.0);
//     glVertex3d(-1.0, -1.0, 1.0);
//     glVertex3d(1.0, -1.0, 1.0);
//     glEnd();
// }

void controlLight(GLFWwindow *window, int key, int scancode, int action, int mods)
{

    if (glfwGetKey(window, GLFW_KEY_A) != GLFW_RELEASE)
        light.x += light.x > 0 ? 0.2 : 0;
    if (glfwGetKey(window, GLFW_KEY_D) != GLFW_RELEASE)
        light.x -= light.x > 0 ? 0.2 : 0;
    if (glfwGetKey(window, GLFW_KEY_W) != GLFW_RELEASE)
        light.y += light.y > 0 ? 0.2 : 0;
    if (glfwGetKey(window, GLFW_KEY_S) != GLFW_RELEASE)
        light.y -= light.y > 0 ? 0.2 : 0;
    if (glfwGetKey(window, GLFW_KEY_Q) != GLFW_RELEASE)
        light.z += light.z > 0 ? 0.2 : 0;
    if (glfwGetKey(window, GLFW_KEY_E) != GLFW_RELEASE)
        light.z -= light.z > 0 ? 0.2 : 0;
}
// int main()
// {
//     glfwInit();
//     GLFWwindow *window = glfwCreateWindow(1080, 400, "Infinity Synth Highway", NULL, NULL);
//     glfwMakeContextCurrent(window);
//     glClearColor(0.6, 0.7, 0.8, 1);
//     glEnable(GL_DEPTH_TEST);
//     glPointSize(5);
//     // std::cout << glGetPointerv() << std::endl;
//     glfwSetKeyCallback(window, controlLight);
//     while (!glfwWindowShouldClose(window))
//     {
//         glfwPollEvents();
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         int width, height;
//         glfwGetFramebufferSize(window, &width, &height);
//         resize(width, height);

//         draw();
//         glfwSwapBuffers(window);
//     }
// }