#include <math.h>
#include "matrix.h"

vec3_t mat4_mul_vec3(const mat4x4_t* mat, const vec3_t vec) {
    return (vec3_t){
        mat->m[0][0] * vec.x + mat->m[0][1] * vec.y + mat->m[0][2] * vec.z + mat->m[0][3],
        mat->m[1][0] * vec.x + mat->m[1][1] * vec.y + mat->m[1][2] * vec.z + mat->m[1][3],
        mat->m[2][0] * vec.x + mat->m[2][1] * vec.y + mat->m[2][2] * vec.z + mat->m[2][3]
    };
}

vec4_t mat4x4_mul_vec4(const mat4x4_t* mat, const vec4_t vec) {
    return (vec4_t){
        mat->m[0][0] * vec.x + mat->m[0][1] * vec.y + mat->m[0][2] * vec.z + mat->m[0][3] * vec.w,
        mat->m[1][0] * vec.x + mat->m[1][1] * vec.y + mat->m[1][2] * vec.z + mat->m[1][3] * vec.w,
        mat->m[2][0] * vec.x + mat->m[2][1] * vec.y + mat->m[2][2] * vec.z + mat->m[2][3] * vec.w,
        mat->m[3][0] * vec.x + mat->m[3][1] * vec.y + mat->m[3][2] * vec.z + mat->m[3][3] * vec.w
    };
}

mat4x4_t mat4_mul(const mat4x4_t* a, const mat4x4_t* b) {
    mat4x4_t result = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = a->m[i][0] * b->m[0][j] +
                             a->m[i][1] * b->m[1][j] +
                             a->m[i][2] * b->m[2][j] +
                             a->m[i][3] * b->m[3][j];
        }
    }
    return result;
}

mat4x4_t make_translation_matrix(const float x, const float y, const float z) {
    return (mat4x4_t){{
        {1.0f, 0.0f, 0.0f,   x},
        {0.0f, 1.0f, 0.0f,   y},
        {0.0f, 0.0f, 1.0f,   z},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
}

mat4x4_t make_scale_matrix(const float sx, const float sy, const float sz) {
    return (mat4x4_t){{
        { sx, 0.0f, 0.0f, 0.0f},
        {0.0f,  sy, 0.0f, 0.0f},
        {0.0f, 0.0f,  sz, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
}

mat4x4_t make_rotation_matrix(const float pitch, const float yaw, const float roll) {
    const float alpha = yaw   * DEG_TO_RAD;
    const float beta  = pitch * DEG_TO_RAD;
    const float gamma = roll  * DEG_TO_RAD;

    const float ca = cosf(alpha);
    const float sa = sinf(alpha);
    const float cb = cosf(beta);
    const float sb = sinf(beta);
    const float cg = cosf(gamma);
    const float sg = sinf(gamma);

    return (mat4x4_t){{
        {ca*cb,  ca*sb*sg - sa*cg,  ca*sb*cg + sa*sg, 0.0f},
        {sa*cb,  sa*sb*sg + ca*cg,  sa*sb*cg - ca*sg, 0.0f},
        {  -sb,             cb*sg,             cb*cg, 0.0f},
        {0.0f,               0.0f,              0.0f, 1.0f}
    }};
}

mat4x4_t make_view_matrix(const vec3_t eye, const vec3_t target) {
    const vec3_t forward = vec3_normalize((vec3_t){eye.x - target.x, eye.y - target.y, eye.z - target.z});
    const vec3_t right   = vec3_cross((vec3_t){0.0f, 1.0f, 0.0f}, forward);
    const vec3_t up      = vec3_cross(forward, right);

    return (mat4x4_t){{
        {  right.x,   right.y,   right.z,  -vec3_dot(right, eye)},
        {     up.x,      up.y,      up.z,  -vec3_dot(up, eye)},
        {forward.x, forward.y, forward.z,  -vec3_dot(forward, eye)},
        {     0.0f,      0.0f,      0.0f,   1.0f}
    }};
}

mat4x4_t make_perspective_matrix(const float fov, const int screen_width, const int screen_height, const float near, const float far) {
    const float f = 1.0f / tanf(fov * 0.5f * DEG_TO_RAD);
    const float aspect = (float)screen_width / (float)screen_height;

    return (mat4x4_t){{
        {f / aspect, 0.0f,                       0.0f,  0.0f},
        {0.0f,          f,                       0.0f,  0.0f},
        {0.0f,       0.0f,        -far / (far - near), -1.0f},
        {0.0f,       0.0f, -far * near / (far - near),  0.0f}
    }};
}

mat4x4_t make_orthographic_matrix(const int screen_width, const int screen_height, const float near, const float far) {
    const float aspect = (float)screen_width / (float)screen_height;
    const float right  =  aspect;
    const float left   = -aspect;
    const float top    =  1.0f;
    const float bottom = -1.0f;

    return (mat4x4_t){{
        {2.0f / (right - left),                  0.0f,                 0.0f, -(right + left) / (right - left)},
        {0.0f,                  2.0f / (top - bottom),                 0.0f, -(top + bottom) / (top - bottom)},
        {0.0f,                                   0.0f, -2.0f / (far - near),     -(far + near) / (far - near)},
        {0.0f,                                   0.0f,                 0.0f,                             1.0f}
    }};
}
