#ifndef SOFTWARE_RENDERER_C_CAMERA_H
#define SOFTWARE_RENDERER_C_CAMERA_H

#include "vectors.h"

typedef struct {
    vec3_t position;
    vec3_t target;
} camera_t;

camera_t make_camera(vec3_t position);

#endif //SOFTWARE_RENDERER_C_CAMERA_H