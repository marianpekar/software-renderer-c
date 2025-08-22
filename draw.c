#include <math.h>
#include "draw.h"
#include "constants.h"
#include "sort.h"

void draw_wireframe(
    const sdl_gfx* gfx,
    const vec3 *vertices,
    const triangle *tris,
    const int tris_count,
    const uint32_t color,
    const mat4x4* proj_mat,
    const projection_type proj_type,
    const bool cull_back_face
    ) {
    for (int i = 0; i < tris_count; ++i) {
        const triangle tri = tris[i];

        const vec3 v1 = vertices[tri.v[0]];
        const vec3 v2 = vertices[tri.v[1]];
        const vec3 v3 = vertices[tri.v[2]];

        if (cull_back_face && is_back_face(proj_type, v1, v2, v3))
            continue;

        const vec3 p1 = project_to_screen(proj_type, proj_mat, v1);
        const vec3 p2 = project_to_screen(proj_type, proj_mat, v2);
        const vec3 p3 = project_to_screen(proj_type, proj_mat, v3);

        if (is_outside_frustum(p1, p2, p3))
            continue;

        draw_line(gfx, (vec2){p1.x, p1.y}, (vec2){p2.x, p2.y}, color);
        draw_line(gfx, (vec2){p2.x, p2.y}, (vec2){p3.x, p3.y}, color);
        draw_line(gfx, (vec2){p3.x, p3.y}, (vec2){p1.x, p1.y}, color);
    }
}

void draw_line(const sdl_gfx* gfx, const vec2 a, const vec2 b, const uint32_t color) {
    const float d_x = b.x - a.x;
    const float d_y = b.y - a.y;

    const float longer_delta = fabsf(d_x) >= fabsf(d_y) ? fabsf(d_x) : fabsf(d_y);

    const float inc_x = d_x / longer_delta;
    const float inc_y = d_y / longer_delta;

    float x = a.x;
    float y = a.y;

    for (int i = 0; i <= (int)longer_delta; ++i) {
        sdl_gfx_draw_pixel(gfx, (int)x, (int)y, color);
        x += inc_x;
        y += inc_y;
    }
}

void draw_unlit(const sdl_gfx *gfx, const vec3 *vertices, const triangle *tris, int tris_count, uint32_t color, const mat4x4 *proj_mat, projection_type proj_type, z_buffer *z_buffer) {
    for (int i = 0; i < tris_count; ++i) {
        const triangle tri = tris[i];

        const vec3 v1 = vertices[tri.v[0]];
        const vec3 v2 = vertices[tri.v[1]];
        const vec3 v3 = vertices[tri.v[2]];

        if (is_back_face(proj_type, v1, v2, v3))
            continue;

        const vec3 p1 = project_to_screen(proj_type, proj_mat, v1);
        const vec3 p2 = project_to_screen(proj_type, proj_mat, v2);
        const vec3 p3 = project_to_screen(proj_type, proj_mat, v3);

        if (is_outside_frustum(p1, p2, p3))
            continue;

        draw_filled_triangle(gfx, p1, p2, p3, color, z_buffer);
    }
}

void draw_filled_triangle(const sdl_gfx *gfx, vec3 p1, vec3 p2, vec3 p3, const uint32_t color, z_buffer *z_buffer) {
    sort_points(&p1, &p2, &p3);
    vec3_floor_xy(&p1);
    vec3_floor_xy(&p2);
    vec3_floor_xy(&p3);

    // Draw flat-bottom triangle
    if ((int)p2.y != (int)p1.y) {
        const float inv_slope1 = (p2.x - p1.x) / (p2.y - p1.y);
        const float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

        for (int y = (int)p1.y; y <= (int)p2.y; y++) {
            float x_start = p1.x + ((float)y - p1.y) * inv_slope1;
            float x_end   = p1.x + ((float)y - p1.y) * inv_slope2;

            if (x_start > x_end) {
                const float temp = x_start;
                x_start = x_end;
                x_end = temp;
            }

            for (int x = (int)x_start; x <= (int)x_end; x++) {
                draw_pixel(gfx, x, y, p1, p2, p3, color, z_buffer);
            }
        }
    }

    // Draw flat-top triangle
    if ((int)p3.y != (int)p2.y) {
        const float inv_slope1 = (p3.x - p2.x) / (p3.y - p2.y);
        const float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

        for (int y = (int)p2.y; y <= (int)p3.y; y++) {
            float x_start = p2.x + ((float)y - p2.y) * inv_slope1;
            float x_end   = p1.x + ((float)y - p1.y) * inv_slope2;

            if (x_start > x_end) {
                const float temp = x_start;
                x_start = x_end;
                x_end = temp;
            }

            for (int x = (int)x_start; x <= (int)x_end; x++) {
                draw_pixel(gfx, x, y, p1, p2, p3, color, z_buffer);
            }
        }
    }
}

void draw_pixel(const sdl_gfx *gfx, const int x, const int y, const vec3 p1, const vec3 p2, const vec3 p3, const uint32_t color, z_buffer *z_buffer) {
    if (is_point_outside_viewport(x, y))
        return;

    const vec2 p = (vec2){(float)x, (float)y};
    const vec2 p1_xy = vec2_from_vec3(p1);
    const vec2 p2_xy = vec2_from_vec3(p2);
    const vec2 p3_xy = vec2_from_vec3(p3);

    const vec3 weights = barycentric_weights(p1_xy, p2_xy, p3_xy, p);

    const float alpha = weights.x;
    const float beta  = weights.y;
    const float gamma = weights.z;

    const float depth  = alpha*p1.z + beta*p2.z + gamma*p3.z;

    const int z_index = SCREEN_WIDTH * y + x;
    if (depth <= (*z_buffer)[z_index]) {
        sdl_gfx_draw_pixel(gfx, x, y, color);
        (*z_buffer)[z_index] = depth;
    }
}

vec3 project_to_screen(const projection_type proj_type, const mat4x4* mat, const vec3 v) {
    const vec4 clip = mat4x4_mul_vec4(mat, (vec4){v.x, v.y, v.z, 1.0f});
    const float inv_w = 1.0f / clip.w;

    const float ndc_x = clip.x * inv_w;
    const float ndc_y = clip.y * inv_w;

    const float screen_x = (ndc_x * 0.5f + 0.5f) * SCREEN_WIDTH;
    const float screen_y = (-ndc_y * 0.5f + 0.5f) * SCREEN_HEIGHT;

    if (proj_type == PERSPECTIVE) {
        return (vec3){screen_x, screen_y, inv_w};
    }

    return (vec3){screen_x, screen_y, -clip.z};
}

vec3 barycentric_weights(const vec2 a, const vec2 b, const vec2 c, const vec2 p) {
    const vec2 ac = vec2_diff(c, a);
    const vec2 ab = vec2_diff(b, a);
    const vec2 ap = vec2_diff(p, a);
    const vec2 pc = vec2_diff(c, p);
    const vec2 pb = vec2_diff(b, p);

    const float area = ac.x * ab.y - ac.y * ab.x;

    if (area <= 0.0f) {
        return (vec3){0.0f, 0.0f, 0.0f};
    }

    const float alpha = (pc.x * pb.y - pc.y * pb.x) / area;
    const float beta  = (ac.x * ap.y - ac.y * ap.x) / area;
    const float gamma = 1.0f - alpha - beta;

    return (vec3){alpha, beta, gamma};
}

bool is_back_face(const projection_type proj_type, const vec3 v1, const vec3 v2, const vec3 v3) {
    const vec3 edge1 = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    const vec3 edge2 = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};

    const vec3 cross = vec3_cross(edge1, edge2);
    const vec3 cross_norm = vec3_normalize(cross);

    vec3 to_camera;
    if (proj_type == PERSPECTIVE) {
        to_camera = vec3_normalize(v1);
    }
    else {
        to_camera = (vec3){0.0f, 0.0f, -1.0f};
    }

    return vec3_dot(cross_norm, to_camera) >= 0.0f;
}

bool is_outside_frustum(const vec3 p1, const vec3 p2, const vec3 p3) {
    if (p1.z > 1.0f || p2.z > 1.0f || p3.z > 1.0f || p1.z < -1.0f || p2.z < -1.0f || p3.z < -1.0f)
        return true;

    const float min_x = fminf(p1.x, fminf(p2.x, p3.x));
    const float max_x = fmaxf(p1.x, fmaxf(p2.x, p3.x));
    const float min_y = fminf(p1.y, fminf(p2.y, p3.y));
    const float max_y = fmaxf(p1.y, fmaxf(p2.y, p3.y));

    if (max_x < 0.0f || min_x > SCREEN_WIDTH || max_y < 0.0f || min_y > SCREEN_HEIGHT)
        return true;

    return false;
}

bool is_point_outside_viewport(const int x, const int y) {
    return x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT;
}
