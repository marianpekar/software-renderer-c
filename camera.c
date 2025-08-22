#include "camera.h"

camera make_camera(const vec3 position) {
    camera camera;
    camera.position = position;
    camera.target = (vec3){0.0f, 0.0f, -1.0f};
    return camera;
}