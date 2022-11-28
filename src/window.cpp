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

// ANGULOS DE CAMERA
ushort select_cam = 0;
std::vector<Cam> cameras = {
    Cam(vec3(0.2, 0.4, -2.0), vec3(0.2, 0.4, -5.0), vec3(0.0, 1.0, 0.0)),
    Cam(vec3(0.0, 0.0, 12.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0)),
    Cam(vec3(70.0, 3.0, -5.0), vec3(6.0, 1.0, -15.0), vec3(0.0, 1.0, 0.0)),
    Cam(vec3(-70.0, 3.0, 5.0), vec3(6.0, 1.0, 15.0), vec3(0.0, 1.0, 0.0)),
    Cam(vec3(-70.0, 1.6, -10.0), vec3(-70, 1.0, -11.0), vec3(0.0, 1.0, 0.0)),
};

// FUNÇÕES
void controlCar(GLFWwindow *window, int key, int scancode, int action, int mods);
void resize(int w, int h);
void updateGameLoop();
void updateObjects();
void initObjects();
void initWindow();
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
    objectLoader->drawObject(sun_id, vec3(0.0, 0.0, 0.0), 1);

    // CARREGA PALMEIRAS
    objectLoader->loadObject("./objects/palm.obj");
    for (int i = 0; i < total_objects_scenery; i++)
    {
        objectsScene.push_back(scenary_id + i);
        glPushMatrix();
        glScaled(10.0, 10.0, 10.0);
        double rx = getRandomNumber(i, 0, 20);
        double ry = getRandomNumber(i, -1, 0);
        glTranslated(rx, 0, ry);
        objectLoader->drawObject(objectsScene[i], vec3(0, 0, 0), 2);
        glPopMatrix();
    }

    // CARREGA CARROS(PRINCIPAL E OBSTACULOS)
    bestCar = new Car(best_car_id, vec3(0, -2, -10), 0, 3);
    objectLoader->loadObject("./objects/car.obj");
    for (int i = 0; i < total_cars_adversary; i++)
    {
        // id da lista - posição em vec3, velocidade - cor
        vec3 pos = vec3(getRandomNumber(i, -5, 5), -2, -50);
        cars.push_back(Car(total_objects_scenery + i, pos, ((int)getRandomNumber(i, 0, 150)), rand() % 3));
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
    for (Car car : cars)
    {
        glPushMatrix();
        glScaled(0.5, 0.5, 0.5);
        vec3 pos = car.getPosition();
        float adversary_speed = car.getSpeed();
        float best_speed = bestCar->getSpeed();
        glTranslated(pos.x, pos.y, pos.z + (best_speed - adversary_speed));
        if (car.getPositionOnRoad() == ON)
            glCallList(car.getObjectId());
        glPopMatrix();
    }

    // PRINCIPAL
    glPushMatrix();
    glScaled(0.5, 0.5, 0.5);
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
        updateObjects();
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
    gluPerspective(45.0 + bestCar->getSpeed() / 20, aspect, 1.0, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    vec3 pos = cameras[select_cam].pos;
    vec3 look = cameras[select_cam].look;
    vec3 top = cameras[select_cam].top;
    gluLookAt(pos.x, pos.y, pos.z, look.x, look.y, look.z, bestCar->getCarDirection() / 100, top.y, top.z);
}

void controlCar(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    is_idle = 0;
    if (!is_idle)
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
        if (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_RELEASE) // TROCA DE CAMERA
            select_cam = select_cam < cameras.size() ? select_cam + 1 : 0;

        // DEBUG
        {
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

    if (!((int)countTimer % 10))
    {
        int r = (int)getRandomNumber((int)countTimer, 0, cars.size());
        cars[r].setPositionOnRoad(ON);
    }

    // PARADA LENTA
    if (is_idle && (int)countTimer % 2)
        bestCar->carDeceleration();
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
