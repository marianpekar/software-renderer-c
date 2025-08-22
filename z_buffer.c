#include <stdlib.h>
#include <float.h>
#include "z_buffer.h"

void clear_z_buffer(z_buffer_t *z_buffer) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
        (*z_buffer)[i] = FLT_MAX;
    }
}

z_buffer_t* make_z_buffer() {
    z_buffer_t* z_buffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(float));
    return z_buffer;
}
