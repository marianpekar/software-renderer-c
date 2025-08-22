#include <SDL3/SDL.h>
#include "inputs.h"

void handle_inputs(vec3* translation, vec3* rotation, float* scale, int* render_mode, const int rend_modes_count, projection_type *proj_type, bool* is_running, const float delta_time) {
    const bool* key_state = SDL_GetKeyboardState(NULL);

    const float linear_step = (key_state[SDL_SCANCODE_LSHIFT] != 0) ? 0.25f : 1.0f * delta_time;
    const float angular_step = (key_state[SDL_SCANCODE_LSHIFT] != 0) ? 12.0f : 48.0f * delta_time;

    // Translation
    if (key_state[SDL_SCANCODE_W]) translation->z += linear_step;
    if (key_state[SDL_SCANCODE_S]) translation->z -= linear_step;
    if (key_state[SDL_SCANCODE_A]) translation->x += linear_step;
    if (key_state[SDL_SCANCODE_D]) translation->x -= linear_step;
    if (key_state[SDL_SCANCODE_E]) translation->y += linear_step;
    if (key_state[SDL_SCANCODE_Q]) translation->y -= linear_step;

    // Rotation
    if (key_state[SDL_SCANCODE_J]) rotation->x += angular_step;
    if (key_state[SDL_SCANCODE_L]) rotation->x -= angular_step;
    if (key_state[SDL_SCANCODE_O]) rotation->y += angular_step;
    if (key_state[SDL_SCANCODE_U]) rotation->y -= angular_step;
    if (key_state[SDL_SCANCODE_I]) rotation->z -= angular_step;
    if (key_state[SDL_SCANCODE_K]) rotation->z += angular_step;

    // Scale
    if (key_state[SDL_SCANCODE_KP_PLUS]) *scale += linear_step;
    if (key_state[SDL_SCANCODE_KP_MINUS]) *scale -= linear_step;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            *is_running = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN) {
            switch (event.key.scancode) {
                // Render modes
                case SDL_SCANCODE_LEFT:
                    *render_mode = (*render_mode + rend_modes_count - 1) % rend_modes_count;
                    break;
                case SDL_SCANCODE_RIGHT:
                    *render_mode = (*render_mode + 1) % rend_modes_count;
                    break;

                    // Projection modes
                case SDL_SCANCODE_KP_0:
                    *proj_type = PERSPECTIVE;
                    break;
                case SDL_SCANCODE_KP_1:
                    *proj_type = ORTHOGRAPHIC;
                    break;
                default:
                    break;
            }
        }
    }
}
