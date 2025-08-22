#ifndef SOFTWARE_RENDERER_C_DRAW_H
#define SOFTWARE_RENDERER_C_DRAW_H

#include "matrix.h"
#include "vectors.h"
#include "mesh.h"
#include "sdl_gfx.h"

typedef enum { PERSPECTIVE, ORTHOGRAPHIC } projection_type;

void    draw_wireframe(const sdl_gfx* gfx, const vec3 *vertices, const triangle *tris, int tris_count, uint32_t color, const mat4x4* proj_mat, projection_type proj_type, bool cull_back_face);
void    draw_line(const sdl_gfx* gfx, vec2 a, vec2 b, uint32_t color);

vec3    project_to_screen(projection_type proj_type, const mat4x4* mat, vec3 v);
bool    is_back_face(projection_type proj_type, vec3 v1, vec3 v2, vec3 v3);
bool    is_outside_frustum(vec3 p1, vec3 p2, vec3 p3);

#endif //SOFTWARE_RENDERER_C_DRAW_H