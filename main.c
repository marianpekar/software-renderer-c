#include <stdio.h>
#include <SDL3/SDL.h>

#include "camera.h"
#include "constants.h"
#include "draw.h"
#include "inputs.h"
#include "sdl_gfx.h"
#include "z_buffer.h"
#include "model.h"

int main(void) {
    const sdl_gfx* gfx = sdl_gfx_init("Software Renderer", SCREEN_WIDTH, SCREEN_HEIGHT);

    if (gfx == NULL) {
        return 1;
    }

    const model_t cube = load_model("./assets/cube.obj", "./assets/box.png", COLOR_WHITE, COLOR_GREEN);
    const model_t monkey = load_model("./assets/monkey.obj", "./assets/uv_checker.png", COLOR_WHITE, COLOR_RED);
    model_t models[] = { cube, monkey };

    const camera_t camera = make_camera((vec3_t){0.0f, 0.0f, -3.0f});

    const light_t light = make_light((vec3_t){-4.0f, 0.0f, -3.0f}, (vec3_t){ 1.0f, 1.0f, 0.0f}, (vec4_t){1.0f, 0.0f, 0.0f, 1.0f});
    const light_t light2 = make_light((vec3_t){4.0f, 0.0f, -3.0f}, (vec3_t){-1.0f, -1.0f, 0.0f}, (vec4_t){0.0f, 1.0f, 0.0f, 1.0f});
    const light_t lights[] = { light, light2 };
    const int lights_count = 2;

    const vec3_t ambient = { 0.2f, 0.2f, 0.2f };
    const vec3_t ambient2 = { 0.1f, 0.1f, 0.2f };

    const int rend_modes_count = 8;
    int render_mode = rend_modes_count - 1;
    projection_type proj_type = PERSPECTIVE;

    const mat4x4_t perspective_mat = make_perspective_matrix(FOV, SCREEN_WIDTH, SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
    const mat4x4_t ortho_mat = make_orthographic_matrix(SCREEN_WIDTH, SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

    z_buffer_t* z_buffer = make_z_buffer();

    int selected_model_idx = 0;
    const int model_count = sizeof(models) / sizeof(models[0]);
    model_t* selected_model;

    for (int i = 0; i < model_count; ++i) {
        model_t* model = &models[i];
        apply_transformations(model, &camera);
    }

    Uint64 last_time = SDL_GetPerformanceCounter();
    Uint64 fps_last_time = last_time;
    int frames = 0;

    bool is_running = true;
    while (is_running) {
        const Uint64 current_time = SDL_GetPerformanceCounter();
        const float delta_time = (float)(current_time - last_time)/(float)SDL_GetPerformanceFrequency();
        last_time = current_time;

        frames++;
        if ((current_time - fps_last_time) > SDL_GetPerformanceFrequency()) {
            float fps = (float)frames / ((float)(current_time - fps_last_time) / (float)SDL_GetPerformanceFrequency());
            printf("FPS: %.0f\n", fps);
            fps_last_time = current_time;
            frames = 0;
        }

        selected_model = &models[selected_model_idx];

        handle_inputs(&selected_model->translation, &selected_model->rotation, &selected_model->scale, &render_mode, rend_modes_count, &selected_model_idx, model_count, &proj_type, &is_running, delta_time);

        apply_transformations(selected_model, &camera);

        const mat4x4_t proj_mat = (proj_type == PERSPECTIVE) ? perspective_mat : ortho_mat;

        clear_z_buffer(z_buffer);

        sdl_gfx_clear(gfx, COLOR_BLACK);

        for (int i = 0; i < model_count; ++i) {
            const model_t* model = &models[i];

            switch (render_mode) {
                case 7:
                    draw_textured_phong_shaded(gfx,
                        model->mesh.transformed_vertices, model->mesh.transformed_normals,
                        model->mesh.triangles,model->mesh.triangle_count,
                        model->mesh.uvs,
                        &model->texture,
                        lights, lights_count,
                        &proj_mat, proj_type,
                        z_buffer,
                        ambient2);
                    break;
                case 6:
                    draw_textured_flat_shaded(gfx,
                        model->mesh.transformed_vertices,
                        model->mesh.triangles, model->mesh.triangle_count,
                        model->mesh.uvs, &model->texture,
                        lights, lights_count,
                        &proj_mat, proj_type,
                        z_buffer,
                        ambient);
                    break;
                case 5:
                    draw_textured_unlit(gfx,
                        model->mesh.transformed_vertices,
                        model->mesh.triangles, model->mesh.triangle_count,
                        model->mesh.uvs,
                        &model->texture,
                        &proj_mat, proj_type,
                        z_buffer);
                    break;
                case 4:
                    draw_phong_shaded(gfx,
                        model->mesh.transformed_vertices,
                        model->mesh.transformed_normals,
                        model->mesh.triangles,
                        model->mesh.triangle_count,
                        model->color,
                        lights, lights_count,
                        &proj_mat, proj_type,
                        z_buffer,
                        ambient2);
                    break;
                case 3:
                    draw_flat_shaded(gfx,
                        model->mesh.transformed_vertices,
                        model->mesh.triangles, model->mesh.triangle_count,
                        model->color,
                        lights, lights_count,
                        &proj_mat, proj_type,
                        z_buffer,
                        ambient);
                    break;
                case 2:
                    draw_unlit(gfx,
                        model->mesh.transformed_vertices,
                        model->mesh.triangles, model->mesh.triangle_count,
                        model->color,
                        &proj_mat, proj_type,
                        z_buffer);
                    break;
                case 1:
                    draw_wireframe(gfx,
                        model->mesh.transformed_vertices,
                        model->mesh.triangles, model->mesh.triangle_count,
                        model->wire_color,
                        &proj_mat, proj_type,
                        false);
                    break;
                case 0:
                    draw_wireframe(gfx,
                        model->mesh.transformed_vertices,
                        model->mesh.triangles, model->mesh.triangle_count,
                        model->wire_color,
                        &proj_mat, proj_type,
                        true);
                    break;
                default:
                    break;
            }
        }

        sdl_gfx_render(gfx);
    }

    sdl_gfx_dispose(gfx);
    return 0;
}
