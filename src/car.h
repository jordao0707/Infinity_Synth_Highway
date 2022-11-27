
#include <GL/gl.h>
#include "../hearders/structs.h"

class Car
{

public:
    Car(GLuint obj_id, vec3 pst, int pst_on_rd, float spd, int c)
    {
        object_id = obj_id;
        position = pst;
        position_on_road = pst_on_rd;
        speed = spd;
        color = c;
    }
    float getSpeed();
    vec3 getPosition();
    int getPositionOnRoad();
    GLuint getObjectId();
    int getColor();

private:
    float speed;
    vec3 position;
    int position_on_road;
    GLuint object_id;
    int color;
};
