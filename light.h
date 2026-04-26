#ifndef SOFTWARE_RENDERER_C_LIGHT_H
#define SOFTWARE_RENDERER_C_LIGHT_H

#include "vectors.h"
#include "matrix.h"

typedef struct {
    vec3_t position;
    vec3_t direction;
    vec4_t color;
} light_t;

light_t make_light(vec3_t position, vec3_t direction, vec4_t color, mat4x4_t view_matrix);

#endif //SOFTWARE_RENDERER_C_LIGHT_H