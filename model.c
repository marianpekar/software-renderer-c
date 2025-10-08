#include "model.h"

#include "matrix.h"

void transform_vertices(vec3_t* transformed, const vec3_t* original, const int count, const mat4x4_t* mat) {
    for (int i = 0; i < count; ++i) {
        transformed[i] = mat4_mul_vec3(mat, original[i]);
    }
}

void apply_transformations(const model_t* model, const camera_t* camera) {
    const mat4x4_t trans_mat = make_translation_matrix(model->translation.x, model->translation.y, model->translation.z);
    const mat4x4_t rot_mat   = make_rotation_matrix(model->rotation.x, model->rotation.y, model->rotation.z);
    const mat4x4_t scale_mat = make_scale_matrix(model->scale, model->scale, model->scale);

    const mat4x4_t rs_mat    = mat4_mul(&rot_mat, &scale_mat);
    const mat4x4_t model_mat = mat4_mul(&trans_mat, &rs_mat);
    const mat4x4_t view_mat  = make_view_matrix(camera->position, camera->target);
    const mat4x4_t mv_mat    = mat4_mul(&view_mat, &model_mat);

    transform_vertices(model->mesh.transformed_vertices, model->mesh.vertices, model->mesh.vertex_count, &mv_mat);
    transform_vertices(model->mesh.transformed_normals, model->mesh.normals, model->mesh.normals_count, &mv_mat);
}

model_t load_model(const char *mesh_path, const char *texture_path, const uint32_t color, const uint32_t wire_color) {
    const model_t model = {
        .mesh = load_mesh_from_obj(mesh_path),
        .texture = load_texture_from_file(texture_path),
        .color = color,
        .wire_color = wire_color,
        .translation = {0.0f, 0.0f, 0.0f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .scale = 1.0f
    };
    return model;
}