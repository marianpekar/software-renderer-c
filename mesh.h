#ifndef SOFTWARE_RENDERER_C_MESH_H
#define SOFTWARE_RENDERER_C_MESH_H

#include "vectors.h"

typedef struct {
    int v[3];
    int uv[3];
    int n[3];
} triangle;

typedef struct {
    vec3* transformedVertices;
    vec3* transformedNormals;
    vec3* vertices;
    vec3* normals;
    vec2* uvs;
    triangle* triangles;

    int vertexCount;
    int normalCount;
    int uvCount;
    int triangleCount;
} mesh;

mesh make_cube(void);

#endif //SOFTWARE_RENDERER_C_MESH_H