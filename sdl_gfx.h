#ifndef SOFTWARE_RENDERER_C_WINDOW_H
#define SOFTWARE_RENDERER_C_WINDOW_H

#include <SDL3/SDL.h>

typedef struct {
    int width;
    int height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t* buffer;
} sdl_gfx;

#define RED(color) ((color >> 16) & 0xFF)
#define GREEN(color) ((color >> 8) & 0xFF)
#define BLUE(color) (color & 0xFF)
#define RGB(r, g, b) ((r << 16) | (g << 8) | b)
#define COLOR_BLACK RGB(0, 0, 0)
#define COLOR_WHITE RGB(255, 255, 255)
#define COLOR_GREEN RGB(0, 255, 0)

sdl_gfx* sdl_gfx_init(char window_title[], int width, int height);
void sdl_gfx_render(const sdl_gfx* gfx);
void sdl_gfx_draw_pixel(const sdl_gfx* r, int x, int y, uint32_t color);
void sdl_gfx_clear(const sdl_gfx* gfx, uint32_t color);
void sdl_gfx_dispose(const sdl_gfx *gfx);

#endif //SOFTWARE_RENDERER_C_WINDOW_H