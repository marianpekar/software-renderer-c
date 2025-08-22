#include "camera.h"

camera_t make_camera(const vec3_t position) {
    camera_t camera;
    camera.position = position;
    camera.target = (vec3_t){0.0f, 0.0f, -1.0f};
    return camera;
}