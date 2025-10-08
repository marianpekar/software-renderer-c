#ifndef SOFTWARE_RENDERER_C_MODEL_H
#define SOFTWARE_RENDERER_C_MODEL_H

#include "camera.h"
#include "mesh.h"
#include "texture.h"

typedef struct {
    mesh_t mesh;
    texture_t texture;
    uint32_t color;
    uint32_t wire_color;
    vec3_t translation;
    vec3_t rotation;
    float scale;
} model_t;

model_t load_model(const char* mesh_path, const char* texture_path, uint32_t color, uint32_t wire_color);
void apply_transformations(const model_t* model, const camera_t* camera);

#endif //SOFTWARE_RENDERER_C_MODEL_H