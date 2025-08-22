#include <stdlib.h>
#include "mesh.h"

mesh_t make_cube(void) {
    mesh_t mesh = {0};

    mesh.vertexCount   = 8;
    mesh.normalCount   = 6;
    mesh.uvCount       = 4;
    mesh.triangleCount = 12;

    mesh.vertices            = malloc(mesh.vertexCount   * sizeof(vec3_t));
    mesh.normals             = malloc(mesh.normalCount   * sizeof(vec3_t));
    mesh.uvs                 = malloc(mesh.uvCount       * sizeof(vec2_t));
    mesh.triangles           = malloc(mesh.triangleCount * sizeof(triangle_t));
    mesh.transformedVertices = malloc(mesh.vertexCount   * sizeof(vec3_t));
    mesh.transformedNormals  = malloc(mesh.normalCount   * sizeof(vec3_t));

    mesh.vertices[0] = (vec3_t){-1.0f, -1.0f, -1.0f};
    mesh.vertices[1] = (vec3_t){-1.0f,  1.0f, -1.0f};
    mesh.vertices[2] = (vec3_t){ 1.0f,  1.0f, -1.0f};
    mesh.vertices[3] = (vec3_t){ 1.0f, -1.0f, -1.0f};
    mesh.vertices[4] = (vec3_t){ 1.0f,  1.0f,  1.0f};
    mesh.vertices[5] = (vec3_t){ 1.0f, -1.0f,  1.0f};
    mesh.vertices[6] = (vec3_t){-1.0f,  1.0f,  1.0f};
    mesh.vertices[7] = (vec3_t){-1.0f, -1.0f,  1.0f};

    mesh.normals[0] = (vec3_t){ 0.0f,  0.0f, -1.0f};
    mesh.normals[1] = (vec3_t){ 1.0f,  0.0f,  0.0f};
    mesh.normals[2] = (vec3_t){ 0.0f,  0.0f,  1.0f};
    mesh.normals[3] = (vec3_t){-1.0f,  0.0f,  0.0f};
    mesh.normals[4] = (vec3_t){ 0.0f,  1.0f,  0.0f};
    mesh.normals[5] = (vec3_t){ 0.0f, -1.0f,  0.0f};

    mesh.uvs[0] = (vec2_t){0.0f, 0.0f};
    mesh.uvs[1] = (vec2_t){0.0f, 1.0f};
    mesh.uvs[2] = (vec2_t){1.0f, 1.0f};
    mesh.uvs[3] = (vec2_t){1.0f, 0.0f};

    mesh.triangles[0]  = (triangle_t){{0,1,2}, {0,1,2}, {0,0,0}};
    mesh.triangles[1]  = (triangle_t){{0,2,3}, {0,2,3}, {0,0,0}};
    mesh.triangles[2]  = (triangle_t){{3,2,4}, {0,1,2}, {1,1,1}};
    mesh.triangles[3]  = (triangle_t){{3,4,5}, {0,2,3}, {1,1,1}};
    mesh.triangles[4]  = (triangle_t){{5,4,6}, {0,1,2}, {2,2,2}};
    mesh.triangles[5]  = (triangle_t){{5,6,7}, {0,2,3}, {2,2,2}};
    mesh.triangles[6]  = (triangle_t){{7,6,1}, {0,1,2}, {3,3,3}};
    mesh.triangles[7]  = (triangle_t){{7,1,0}, {0,2,3}, {3,3,3}};
    mesh.triangles[8]  = (triangle_t){{1,6,4}, {0,1,2}, {4,4,4}};
    mesh.triangles[9]  = (triangle_t){{1,4,2}, {0,2,3}, {4,4,4}};
    mesh.triangles[10] = (triangle_t){{5,7,0}, {0,1,2}, {5,5,5}};
    mesh.triangles[11] = (triangle_t){{5,0,3}, {0,2,3}, {5,5,5}};

    return mesh;
}