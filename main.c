#include <SDL3/SDL.h>

#include "sdl_gfx.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    const sdl_gfx* gfx = sdl_gfx_init("Software Renderer", screenWidth, screenHeight);

    if (gfx == NULL) {
        return 1;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        sdl_gfx_clear(gfx, COLOR_BLACK);
        sdl_gfx_render(gfx);
    }

    return 0;
}
