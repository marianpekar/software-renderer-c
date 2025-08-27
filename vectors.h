#ifndef SOFTWARE_RENDERER_C_VECTORS_H
#define SOFTWARE_RENDERER_C_VECTORS_H

typedef struct {
    float x, y;
} vec2_t;

typedef struct {
    float x, y, z;
} vec3_t;

typedef struct {
    float x, y, z, w;
} vec4_t;

vec3_t vec3_normalize(vec3_t v);
float  fast_inverse_sqrt(float x);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
float  vec3_dot(vec3_t v1, vec3_t v2);
void   vec3_floor_xy(vec3_t* v);
vec3_t vec3_diff(vec3_t v1, vec3_t v2);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_mul(vec3_t v, float s);

vec2_t vec2_diff(vec2_t v1, vec2_t v2);
vec2_t vec2_from_vec3(vec3_t v);

#endif //SOFTWARE_RENDERER_C_VECTORS_H