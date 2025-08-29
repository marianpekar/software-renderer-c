#ifndef SOFTWARE_RENDERER_C_SORT_H
#define SOFTWARE_RENDERER_C_SORT_H

#include "vectors.h"

void sort_points(vec3_t* p1, vec3_t* p2, vec3_t* p3);
void sort_points_and_vertices(vec3_t* p1, vec3_t* p2, vec3_t* p3, vec3_t* v1, vec3_t* v2, vec3_t* v3);
void sort_points_and_uvs(vec3_t* p1, vec3_t* p2, vec3_t* p3, vec2_t* uv1, vec2_t* uv2, vec2_t* uv3);

#endif //SOFTWARE_RENDERER_C_SORT_H