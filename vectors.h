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
float fast_inverse_sqrt(float x);
vec3  vec3_cross(vec3 v1, vec3 v2);
float vec3_dot(vec3 v1, vec3 v2);
void  vec3_floor_xy(vec3* v);

vec2  vec2_diff(vec2 v1, vec2 v2);
vec2  vec2_from_vec3(vec3 v);

#endif //SOFTWARE_RENDERER_C_VECTORS_H