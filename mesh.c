#include <stdlib.h>
#include "mesh.h"

mesh make_cube(void) {
    mesh mesh = {0};

    mesh.vertexCount   = 8;
    mesh.normalCount   = 6;
    mesh.uvCount       = 4;
    mesh.triangleCount = 12;

    mesh.vertices            = malloc(mesh.vertexCount   * sizeof(vec3));
    mesh.normals             = malloc(mesh.normalCount   * sizeof(vec3));
    mesh.uvs                 = malloc(mesh.uvCount       * sizeof(vec2));
    mesh.triangles           = malloc(mesh.triangleCount * sizeof(triangle));
    mesh.transformedVertices = malloc(mesh.vertexCount   * sizeof(vec3));
    mesh.transformedNormals  = malloc(mesh.normalCount   * sizeof(vec3));

    mesh.vertices[0] = (vec3){-1.0f, -1.0f, -1.0f};
    mesh.vertices[1] = (vec3){-1.0f,  1.0f, -1.0f};
    mesh.vertices[2] = (vec3){ 1.0f,  1.0f, -1.0f};
    mesh.vertices[3] = (vec3){ 1.0f, -1.0f, -1.0f};
    mesh.vertices[4] = (vec3){ 1.0f,  1.0f,  1.0f};
    mesh.vertices[5] = (vec3){ 1.0f, -1.0f,  1.0f};
    mesh.vertices[6] = (vec3){-1.0f,  1.0f,  1.0f};
    mesh.vertices[7] = (vec3){-1.0f, -1.0f,  1.0f};

    mesh.normals[0] = (vec3){ 0.0f,  0.0f, -1.0f};
    mesh.normals[1] = (vec3){ 1.0f,  0.0f,  0.0f};
    mesh.normals[2] = (vec3){ 0.0f,  0.0f,  1.0f};
    mesh.normals[3] = (vec3){-1.0f,  0.0f,  0.0f};
    mesh.normals[4] = (vec3){ 0.0f,  1.0f,  0.0f};
    mesh.normals[5] = (vec3){ 0.0f, -1.0f,  0.0f};

    mesh.uvs[0] = (vec2){0.0f, 0.0f};
    mesh.uvs[1] = (vec2){0.0f, 1.0f};
    mesh.uvs[2] = (vec2){1.0f, 1.0f};
    mesh.uvs[3] = (vec2){1.0f, 0.0f};

    mesh.triangles[0]  = (triangle){{0,1,2}, {0,1,2}, {0,0,0}};
    mesh.triangles[1]  = (triangle){{0,2,3}, {0,2,3}, {0,0,0}};
    mesh.triangles[2]  = (triangle){{3,2,4}, {0,1,2}, {1,1,1}};
    mesh.triangles[3]  = (triangle){{3,4,5}, {0,2,3}, {1,1,1}};
    mesh.triangles[4]  = (triangle){{5,4,6}, {0,1,2}, {2,2,2}};
    mesh.triangles[5]  = (triangle){{5,6,7}, {0,2,3}, {2,2,2}};
    mesh.triangles[6]  = (triangle){{7,6,1}, {0,1,2}, {3,3,3}};
    mesh.triangles[7]  = (triangle){{7,1,0}, {0,2,3}, {3,3,3}};
    mesh.triangles[8]  = (triangle){{1,6,4}, {0,1,2}, {4,4,4}};
    mesh.triangles[9]  = (triangle){{1,4,2}, {0,2,3}, {4,4,4}};
    mesh.triangles[10] = (triangle){{5,7,0}, {0,1,2}, {5,5,5}};
    mesh.triangles[11] = (triangle){{5,0,3}, {0,2,3}, {5,5,5}};

    return mesh;
}