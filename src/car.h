
#include <GL/gl.h>
#include "../hearders/structs.h"

enum position_road
{
    BEFORE = -1,
    ON = 0,
    AFTER = 1
};

class Car
{

public:
    Car(GLuint obj_id, vec3 pst, float spd, int c)
    {
        object_id = obj_id;
        position = pst;
        position_on_road = BEFORE;
        speed = spd;
        color = c;
    }
    vec3 getPosition();
    GLuint getObjectId();
    void setPositionOnRoad(position_road p);
    int getPositionOnRoad();
    int getColor();
    float getSpeed();
    float getCarDirection();
    float carBreake();
    float carAcceleration();
    float carDeceleration();
    float carTurnsLeft(float left_limit);
    float carTurnsRight(float right_limit);

private:
    float car_direction = 0;
    float speed = 0;
    position_road position_on_road = BEFORE;
    int color = 0;
    vec3 position;
    GLuint object_id;
};
