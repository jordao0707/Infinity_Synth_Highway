#include "car.h"
float Car::getSpeed()
{
    return speed;
}
vec3 Car::getPosition()
{
    return position;
}
int Car::getPositionOnRoad()
{
    return position_on_road;
}
GLuint Car::getObjectId()
{
    return object_id;
}

int Car::getColor()
{
    return color;
}