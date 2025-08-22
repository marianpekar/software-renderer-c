#include "light.h"

light make_light(const vec3 position, const vec3 direction, const float strength) {
    light light;
    light.position = position;
    light.direction = vec3_normalize(direction);
    light.strength = strength;
    return light;
}