#ifndef SOFTWARE_RENDERER_C_CAMERA_H
#define SOFTWARE_RENDERER_C_CAMERA_H

#include "vectors.h"

typedef struct {
    vec3 position;
    vec3 target;
} camera;

camera make_camera(vec3 position);

#endif //SOFTWARE_RENDERER_C_CAMERA_H