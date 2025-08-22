#ifndef SOFTWARE_RENDERER_C_MATRIX_H
#define SOFTWARE_RENDERER_C_MATRIX_H

#include "vectors.h"

#define DEG_TO_RAD (3.14159265358979323846f / 180.0f)

typedef struct {
    float m[4][4];
} mat4x4_t;

vec3_t   mat4_mul_vec3(const mat4x4_t* mat, vec3_t vec);
vec4_t   mat4x4_mul_vec4(const mat4x4_t* mat, vec4_t vec);
mat4x4_t mat4_mul(const mat4x4_t* a, const mat4x4_t* b);

mat4x4_t make_translation_matrix(float x, float y, float z);
mat4x4_t make_scale_matrix(float sx, float sy, float sz);
mat4x4_t make_rotation_matrix(float pitch, float yaw, float roll);
mat4x4_t make_view_matrix(vec3_t eye, vec3_t target);
mat4x4_t make_perspective_matrix(float fov, int screen_width, int screen_height, float near, float far);
mat4x4_t make_orthographic_matrix(int screen_width, int screen_height, float near, float far);

#endif //SOFTWARE_RENDERER_C_MATRIX_H