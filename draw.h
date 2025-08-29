#ifndef SOFTWARE_RENDERER_C_DRAW_H
#define SOFTWARE_RENDERER_C_DRAW_H

#include "matrix.h"
#include "vectors.h"
#include "mesh.h"
#include "sdl_gfx.h"
#include "z_buffer.h"
#include "light.h"
#include "texture.h"

typedef enum { PERSPECTIVE, ORTHOGRAPHIC } projection_type;

void draw_wireframe(const sdl_gfx* gfx, const vec3_t* vertices, const triangle_t* tris, int tris_count, uint32_t color, const mat4x4_t* proj_mat, projection_type proj_type, bool cull_back_face);
void draw_unlit(const sdl_gfx* gfx, const vec3_t* vertices, const triangle_t* tris, int tris_count, uint32_t color, const mat4x4_t* proj_mat, projection_type proj_type, z_buffer_t* z_buffer);
void draw_flat_shaded(const sdl_gfx* gfx, const vec3_t* vertices, const triangle_t* tris, int tris_count, uint32_t color, light_t light, const mat4x4_t* proj_mat, projection_type proj_type, z_buffer_t* z_buffer);
void draw_phong_shaded(const sdl_gfx* gfx, const vec3_t* vertices, const vec3_t* normals, const triangle_t* tris, int tris_count, uint32_t color, light_t light, const mat4x4_t* proj_mat, projection_type proj_type, z_buffer_t* z_buffer);
void draw_textured_unlit(const sdl_gfx* gfx, const vec3_t* vertices, const triangle_t* tris, int tris_count, const vec2_t* uvs, const texture_t* texture, const mat4x4_t* proj_mat, projection_type proj_type, z_buffer_t* z_buffer);

#endif //SOFTWARE_RENDERER_C_DRAW_H