#include "light.h"

light_t make_light(const vec3_t position, const vec3_t direction, const float strength) {
    light_t light;
    light.position = position;
    light.direction = vec3_normalize(direction);
    light.strength = strength;
    return light;
}