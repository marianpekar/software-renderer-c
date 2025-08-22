#ifndef SOFTWARE_RENDERER_C_Z_BUFFER_H
#define SOFTWARE_RENDERER_C_Z_BUFFER_H

#include "constants.h"

typedef float z_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

void        clear_z_buffer(z_buffer* z_buffer);
z_buffer*   make_z_buffer();

#endif //SOFTWARE_RENDERER_C_Z_BUFFER_H