#include <stdio.h>
#include <SDL3/SDL.h>

#include "camera.h"
#include "constants.h"
#include "draw.h"
#include "inputs.h"
#include "mesh.h"
#include "sdl_gfx.h"
#include "texture.h"
#include "z_buffer.h"

void apply_transformations(vec3_t* transformed, const vec3_t* original, const int count, const mat4x4_t* mat) {
    for (int i = 0; i < count; i++) {
        transformed[i] = mat4_mul_vec3(mat, original[i]);
    }
}

int main(void) {
    const sdl_gfx* gfx = sdl_gfx_init("Software Renderer", SCREEN_WIDTH, SCREEN_HEIGHT);

    if (gfx == NULL) {
        return 1;
    }

    //const mesh_t mesh = make_cube();
    const mesh_t mesh = load_mesh_from_obj("./assets/monkey.obj");
    const texture_t texture = load_texture_from_file("./assets/uv_checker.png");
    const camera_t camera = make_camera((vec3_t){0.0f, 0.0f, -3.0f});
    const light_t light = make_light((vec3_t){0.0f, 0.0f, -3.0f}, (vec3_t){0.0f, 1.0f, 0.0f}, 1.0f);

    vec3_t rotation = {0.0f, 180.0f, 0.0f};
    vec3_t translation = {0.0f, 0.0f, 0.0f};
    float scale = 1.0f;

    const int rend_modes_count = 8;
    int render_mode = rend_modes_count - 1;
    projection_type proj_type = PERSPECTIVE;

    const mat4x4_t perspective_mat = make_perspective_matrix(FOV, SCREEN_WIDTH, SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
    const mat4x4_t ortho_mat = make_orthographic_matrix(SCREEN_WIDTH, SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

    z_buffer_t* z_buffer = make_z_buffer();

    Uint64 last_time = SDL_GetPerformanceCounter();

    bool is_running = true;
    while (is_running) {
        const Uint64 current_time = SDL_GetPerformanceCounter();
        const float delta_time = (float)(current_time - last_time)/(float)SDL_GetPerformanceFrequency();
        last_time = current_time;

        handle_inputs(&translation, &rotation, &scale, &render_mode, rend_modes_count, &proj_type, &is_running, delta_time);

        const mat4x4_t trans_mat = make_translation_matrix(translation.x, translation.y, translation.z);
        const mat4x4_t rot_mat   = make_rotation_matrix(rotation.x, rotation.y, rotation.z);
        const mat4x4_t scale_mat = make_scale_matrix(scale, scale, scale);

        const mat4x4_t rs_mat    = mat4_mul(&rot_mat, &scale_mat);
        const mat4x4_t model_mat = mat4_mul(&trans_mat, &rs_mat);
        const mat4x4_t view_mat  = make_view_matrix(camera.position, camera.target);
        const mat4x4_t mv_mat    = mat4_mul(&view_mat, &model_mat);

        apply_transformations(mesh.transformedVertices, mesh.vertices, mesh.vertexCount, &mv_mat);
        apply_transformations(mesh.transformedNormals, mesh.normals, mesh.normalCount, &mv_mat);

        const mat4x4_t proj_mat = (proj_type == PERSPECTIVE) ? perspective_mat : ortho_mat;

        clear_z_buffer(z_buffer);

        sdl_gfx_clear(gfx, COLOR_BLACK);

        switch (render_mode) {
            case 7:
                draw_textured_phong_shaded(gfx, mesh.transformedVertices, mesh.transformedNormals, mesh.triangles, mesh.triangleCount, mesh.uvs, &texture, light, &proj_mat, proj_type, z_buffer);
                break;
            case 6:
                 draw_textured_flat_shaded(gfx, mesh.transformedVertices, mesh.triangles, mesh.triangleCount, mesh.uvs, &texture, light, &proj_mat, proj_type, z_buffer);
                break;
            case 5:
                       draw_textured_unlit(gfx, mesh.transformedVertices, mesh.triangles, mesh.triangleCount, mesh.uvs, &texture, &proj_mat, proj_type, z_buffer);
                break;
            case 4:
                         draw_phong_shaded(gfx, mesh.transformedVertices, mesh.transformedNormals, mesh.triangles, mesh.triangleCount, COLOR_WHITE, light, &proj_mat, proj_type, z_buffer);
                break;
            case 3:
                          draw_flat_shaded(gfx, mesh.transformedVertices, mesh.triangles, mesh.triangleCount, COLOR_WHITE, light, &proj_mat, proj_type, z_buffer);
                break;
            case 2:
                                draw_unlit(gfx, mesh.transformedVertices, mesh.triangles, mesh.triangleCount, COLOR_WHITE, &proj_mat, proj_type, z_buffer);
                break;
            case 1:
                            draw_wireframe(gfx, mesh.transformedVertices, mesh.triangles, mesh.triangleCount, COLOR_GREEN, &proj_mat, proj_type, false);
                break;
            case 0:
                            draw_wireframe(gfx, mesh.transformedVertices, mesh.triangles, mesh.triangleCount, COLOR_GREEN, &proj_mat, proj_type, true);
                break;
            default:
                break;
        }

        sdl_gfx_render(gfx);
    }

    sdl_gfx_dispose(gfx);
    return 0;
}
