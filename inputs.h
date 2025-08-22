#ifndef SOFTWARE_RENDERER_C_INPUTS_H
#define SOFTWARE_RENDERER_C_INPUTS_H

#include "draw.h"
#include "vectors.h"

void handle_inputs(vec3_t* translation, vec3_t* rotation, float* scale, int* render_mode, int rend_modes_count, projection_type *proj_type, bool* is_running, float delta_time);

#endif //SOFTWARE_RENDERER_C_INPUTS_H