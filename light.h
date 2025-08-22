#ifndef SOFTWARE_RENDERER_C_LIGHT_H
#define SOFTWARE_RENDERER_C_LIGHT_H

#include "vectors.h"

typedef struct {
    vec3 position;
    vec3 direction;
    float strength;
} light;

light make_light(vec3 position, vec3 direction, float strength);

#endif //SOFTWARE_RENDERER_C_LIGHT_H