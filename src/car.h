
#include <GL/gl.h>
#include "../hearders/structs.h"

class Car
{

public:
    float car_direction;
    float speed;
    Car(GLuint obj_id, vec3 pst, int pst_on_rd, float spd, int c)
    {
        object_id = obj_id;
        position = pst;
        position_on_road = pst_on_rd;
        speed = spd;
        color = c;
    }
    float getSpeed();
    float getCarDirection();
    vec3 getPosition();
    GLuint getObjectId();
    int getPositionOnRoad();
    int getColor();
    float carBreake();
    float carAcceleration();
    float carDeceleration();
    float carTurnsLeft(float left_limit);
    float carTurnsRight(float right_limit);
   

private:
    vec3 position;
    int position_on_road;
    GLuint object_id;
    int color;
};
