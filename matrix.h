#ifndef SOFTWARE_RENDERER_C_MATRIX_H
#define SOFTWARE_RENDERER_C_MATRIX_H

#include "vectors.h"

#define DEG_TO_RAD (3.14159265358979323846f / 180.0f)

typedef struct {
    float m[4][4];
} mat4x4;

vec3   mat4_mul_vec3(const mat4x4* mat, vec3 vec);
vec4   mat4x4_mul_vec4(const mat4x4* mat, vec4 vec);
mat4x4 mat4_mul(const mat4x4* a, const mat4x4* b);

mat4x4 make_translation_matrix(float x, float y, float z);
mat4x4 make_scale_matrix(float sx, float sy, float sz);
mat4x4 make_rotation_matrix(float pitch, float yaw, float roll);
mat4x4 make_view_matrix(vec3 eye, vec3 target);
mat4x4 make_perspective_matrix(float fov, int screen_width, int screen_height, float near, float far);
mat4x4 make_orthographic_matrix(int screen_width, int screen_height, float near, float far);

#endif //SOFTWARE_RENDERER_C_MATRIX_H