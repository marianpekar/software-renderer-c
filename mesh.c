#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mesh.h"

#define LINE_MAX 512

static void parse_face(const char** tokens, triangle_t* tri) {
    for (int i = 0; i < 3; i++) {
        int v = 0, vt = 0, vn = 0;
        sscanf(tokens[i+1], "%d/%d/%d", &v, &vt, &vn);
        tri->v[i] = v - 1;
        tri->uv[i] = vt - 1;
        tri->n[i] = vn - 1;
    }
}

static vec2_t parse_vec2(const char** tokens) {
    vec2_t v;
    v.x = strtof(tokens[1], NULL);
    v.y = strtof(tokens[2], NULL);
    return v;
}

static vec3_t parse_vec3(const char** tokens) {
    vec3_t v;
    v.x = strtof(tokens[1], NULL);
    v.y = strtof(tokens[2], NULL);
    v.z = strtof(tokens[3], NULL);
    return v;
}

mesh_t load_mesh_from_obj(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Failed to read file %s\n", filename);
        exit(1);
    }

    vec3_t* vertices = NULL;
    vec3_t* normals = NULL;
    vec2_t* uvs = NULL;
    triangle_t* triangles = NULL;
    int v_count = 0, n_count = 0, uv_count = 0, t_count = 0;

    char line[LINE_MAX];
    while (fgets(line, LINE_MAX, f)) {
        if (line[0] == '#') continue;
        const char* tokens[8];
        int tok_count = 0;

        const char* token = strtok(line, " \t\r\n");
        while (token && tok_count < 8) {
            tokens[tok_count++] = token;
            token = strtok(NULL, " \t\r\n");
        }
        if (tok_count == 0)
            continue;

        switch (tokens[0][0]) {
            case 'v':
                if (tokens[0][1] == '\0') {
                    vertices = realloc(vertices, (v_count+1) * sizeof(vec3_t));
                    vertices[v_count++] = parse_vec3(tokens);
                } else if (tokens[0][1] == 'n' && tokens[0][2] == '\0') {
                    normals = realloc(normals, (n_count+1) * sizeof(vec3_t));
                    normals[n_count++] = parse_vec3(tokens);
                } else if (tokens[0][1] == 't' && tokens[0][2] == '\0') {
                    uvs = realloc(uvs, (uv_count+1) * sizeof(vec2_t));
                    uvs[uv_count++] = parse_vec2(tokens);
                }
                break;

            case 'f':
                if (tokens[0][1] == '\0') {
                    triangles = realloc(triangles, (t_count+1) * sizeof(triangle_t));
                    parse_face(tokens, &triangles[t_count]);
                    t_count++;
                }
                break;

            default:
                break;
        }
    }
    fclose(f);

    mesh_t mesh = {0};
    mesh.vertexCount   = v_count;
    mesh.normalCount   = n_count;
    mesh.uvCount       = uv_count;
    mesh.triangleCount = t_count;

    mesh.vertices            = vertices;
    mesh.normals             = normals;
    mesh.uvs                 = uvs;
    mesh.triangles           = triangles;
    mesh.transformedVertices = calloc(v_count, sizeof(vec3_t));
    mesh.transformedNormals  = calloc(n_count, sizeof(vec3_t));

    return mesh;
}

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