#include "light.h"

light_t make_light(const vec3_t position, const vec3_t direction, const vec4_t color, const mat4x4_t view_matrix) {
    light_t light;
    light.position = mat4_mul_vec3(&view_matrix, position);
    light.direction = vec3_normalize(direction);
    light.color = color;
    return light;
}