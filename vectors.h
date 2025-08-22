#ifndef SOFTWARE_RENDERER_C_VECTORS_H
#define SOFTWARE_RENDERER_C_VECTORS_H

typedef struct {
    float x, y;
} vec2;

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float x, y, z, w;
} vec4;

vec3  vec3_normalize(vec3 v);
vec3  vec3_cross(vec3 v1, vec3 v2);
float vec3_dot(vec3 v1, vec3 v2);
float fast_inverse_sqrt(float x);

#endif //SOFTWARE_RENDERER_C_VECTORS_H