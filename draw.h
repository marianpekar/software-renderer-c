#ifndef SOFTWARE_RENDERER_C_DRAW_H
#define SOFTWARE_RENDERER_C_DRAW_H

#include "matrix.h"
#include "vectors.h"
#include "mesh.h"
#include "sdl_gfx.h"
#include "z_buffer.h"
#include "light.h"

typedef enum { PERSPECTIVE, ORTHOGRAPHIC } projection_type;

void    draw_wireframe(const sdl_gfx* gfx, const vec3_t* vertices, const triangle_t* tris, int tris_count, uint32_t color, const mat4x4_t* proj_mat, projection_type proj_type, bool cull_back_face);
void    draw_line(const sdl_gfx* gfx, vec2_t a, vec2_t b, uint32_t color);
void    draw_unlit(const sdl_gfx* gfx, const vec3_t* vertices, const triangle_t* tris, int tris_count, uint32_t color, const mat4x4_t* proj_mat, projection_type proj_type, z_buffer_t* z_buffer);
void    draw_flat_shaded(const sdl_gfx* gfx, const vec3_t* vertices, const triangle_t* tris, int tris_count, uint32_t color, light_t light, const mat4x4_t* proj_mat, projection_type proj_type, z_buffer_t* z_buffer);
void    draw_filled_triangle(const sdl_gfx* gfx, vec3_t p1, vec3_t p2, vec3_t p3, uint32_t color, z_buffer_t* z_buffer);
void    draw_pixel(const sdl_gfx* gfx, int x, int y, vec3_t p1, vec3_t p2, vec3_t p3, uint32_t color, z_buffer_t* z_buffer);

#endif //SOFTWARE_RENDERER_C_DRAW_H