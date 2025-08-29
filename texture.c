#include "texture.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

texture_t load_texture_from_file(const char* path) {
    texture_t texture = {0, 0, NULL};

    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "Failed to load image %s: %s\n", path, SDL_GetError());
        return texture;
    }

    SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_BGRA32);
    SDL_DestroySurface(surface);
    if (!converted) {
        fprintf(stderr, "Failed to convert surface format: %s\n", SDL_GetError());
        return texture;
    }

    texture.width = converted->w;
    texture.height = converted->h;

    const size_t size = (size_t)texture.width * (size_t)texture.height;
    texture.pixels = malloc(size * sizeof(uint32_t));
    if (!texture.pixels) {
        fprintf(stderr, "Failed to allocate memory for pixels\n");
        SDL_DestroySurface(converted);
        texture.width = texture.height = 0;
        return texture;
    }

    memcpy(texture.pixels, converted->pixels, size * sizeof(uint32_t));

    SDL_DestroySurface(converted);

    return texture;
}