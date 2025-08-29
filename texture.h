#ifndef SOFTWARE_RENDERER_C_TEXTURE_H
#define SOFTWARE_RENDERER_C_TEXTURE_H

#include <SDL3/SDL.h>

typedef struct {
    int width;
    int height;
    uint32_t* pixels;
} texture_t;

texture_t load_texture_from_file(const char* path);

#endif //SOFTWARE_RENDERER_C_TEXTURE_H