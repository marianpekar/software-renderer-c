#include <stdint.h>
#include "vectors.h"

#include <math.h>

vec3_t vec3_normalize(const vec3_t v) {
    const float length_sq = v.x * v.x + v.y * v.y + v.z * v.z;

    if (length_sq == 0.0f) {
        return (vec3_t){0.0f, 0.0f, 0.0f};
    }

    const float inv_length = fast_inverse_sqrt(length_sq);

    return (vec3_t) {
        v.x * inv_length,
        v.y * inv_length,
        v.z * inv_length
    };
}

float fast_inverse_sqrt(const float x) {
    union {
        float f;
        int32_t i;
    } conv;

    conv.f = x;
    conv.i = 0x5f3759df - (conv.i >> 1);
    float y = conv.f;

    return y * (1.5f - 0.5f * x * y * y);
}

vec3_t vec3_cross(const vec3_t v1, const vec3_t v2) {
    return (vec3_t) {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

float vec3_dot(const vec3_t v1, const vec3_t v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void vec3_floor_xy(vec3_t *v) {
    v->x = floorf(v->x);
    v->y = floorf(v->y);
}

vec3_t vec3_diff(const vec3_t v1, const vec3_t v2) {
    return (vec3_t){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

vec2_t vec2_diff(const vec2_t v1, const vec2_t v2) {
    return (vec2_t){v1.x - v2.x, v1.y - v2.y};
}

vec2_t vec2_from_vec3(const vec3_t v) {
    return (vec2_t){v.x, v.y};
}
