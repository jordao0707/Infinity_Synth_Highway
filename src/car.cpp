#include "car.h"
float Car::getSpeed()
{
    return speed;
}

float Car::getCarDirection()
{
    return car_direction;
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

void Car::setPositionOnRoad(position_road p){
    position_on_road = p;
}

int Car::getColor()
{
    return color;
}

float Car::carBreake()
{
    return speed = speed > 0 ? speed - 2.0 : 0;
}

float Car::carAcceleration()
{
    return speed = speed <= 200 ? 0.5 + speed * (6 / 5) : speed;
}

float Car::carDeceleration()
{
    return speed = speed > 0 ? speed - 0.08 : 0;
}

float Car::carTurnsLeft(float left_limit)
{
    return car_direction = car_direction > left_limit ? car_direction - 0.05 * (speed * speed) / 1000 : left_limit;
}

float Car::carTurnsRight(float right_limit)
{
    return car_direction = car_direction < right_limit ? car_direction + 0.05 * (speed * speed) / 1000 : right_limit;
}