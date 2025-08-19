#include "sdl_gfx.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

sdl_gfx * sdl_gfx_init(char window_title[], const int width, const int height) {
    sdl_gfx* gfx = malloc(sizeof(sdl_gfx));

    gfx->width = width;
    gfx->height = height;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == false) {
        fprintf(stderr, "Failed to initialize video: %s\n", SDL_GetError());
        return NULL;
    }

    gfx->window = SDL_CreateWindow(window_title, width, height, 0);
    if (gfx->window == NULL) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        return NULL;
    }

    gfx->renderer = SDL_CreateRenderer(gfx->window, NULL);
    if (gfx->renderer == NULL) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        return NULL;
    }

    gfx->texture = SDL_CreateTexture(gfx->renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
    if (gfx->texture == NULL) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        return NULL;
    }

    gfx->buffer = malloc(width * height * 4);
    if (gfx->buffer == NULL) {
        fprintf(stderr, "Failed to allocate buffer.\n");
        return NULL;
    }

    return gfx;
}

void sdl_gfx_render(const sdl_gfx *gfx) {
    if (SDL_UpdateTexture(gfx->texture, NULL, gfx->buffer, gfx->width * 4) == false) {
        printf("Error updating texture: %s", SDL_GetError());
        return;
    }

    SDL_RenderClear(gfx->renderer);
    SDL_RenderTexture(gfx->renderer, gfx->texture, NULL, NULL);
    SDL_RenderPresent(gfx->renderer);
}

void sdl_gfx_draw_pixel(const sdl_gfx *r, const int x, const int y, const uint32_t color) {
    r->buffer[y * r->width + x] = color;
}

void sdl_gfx_clear(const sdl_gfx *gfx, const uint32_t color) {
    for (int i = 0; i < gfx->width*gfx->height; ++i) {
        gfx->buffer[i] = color;
    }
}

void sdl_gfx_dispose(const sdl_gfx *gfx) {
    free(gfx->buffer);
    SDL_DestroyTexture(gfx->texture);
    SDL_DestroyRenderer(gfx->renderer);
    SDL_DestroyWindow(gfx->window);
    SDL_Quit();
}
