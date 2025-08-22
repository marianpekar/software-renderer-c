#ifndef SOFTWARE_RENDERER_C_DRAW_H
#define SOFTWARE_RENDERER_C_DRAW_H

#include "matrix.h"
#include "vectors.h"
#include "mesh.h"
#include "sdl_gfx.h"
#include "z_buffer.h"
#include "light.h"

typedef enum { PERSPECTIVE, ORTHOGRAPHIC } projection_type;

void    draw_wireframe(const sdl_gfx* gfx, const vec3 *vertices, const triangle *tris, int tris_count, uint32_t color, const mat4x4* proj_mat, projection_type proj_type, bool cull_back_face);
void    draw_line(const sdl_gfx* gfx, vec2 a, vec2 b, uint32_t color);
void    draw_unlit(const sdl_gfx *gfx, const vec3 *vertices, const triangle *tris, int tris_count, uint32_t color, const mat4x4 *proj_mat, projection_type proj_type, z_buffer* z_buffer);
void    draw_flat_shaded(const sdl_gfx *gfx, const vec3 *vertices, const triangle *tris, int tris_count, uint32_t color, light light, const mat4x4 *proj_mat, projection_type proj_type, z_buffer* z_buffer);
void    draw_filled_triangle(const sdl_gfx* gfx, vec3 p1, vec3 p2, vec3 p3, uint32_t color, z_buffer* z_buffer);
void    draw_pixel(const sdl_gfx *gfx, int x, int y, vec3 p1, vec3 p2, vec3 p3, uint32_t color, z_buffer* z_buffer);

vec3    project_to_screen(projection_type proj_type, const mat4x4* mat, vec3 v);
vec3    barycentric_weights(vec2 a, vec2 b, vec2 c, vec2 p);
bool    is_back_face(projection_type proj_type, vec3 v1, vec3 v2, vec3 v3);
bool    is_outside_frustum(vec3 p1, vec3 p2, vec3 p3);
bool    is_point_outside_viewport(int x, int y);
float   clamp(float value, float min, float max);

#endif //SOFTWARE_RENDERER_C_DRAW_H