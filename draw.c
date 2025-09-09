#include <math.h>
#include "draw.h"
#include "constants.h"
#include "sort.h"

static vec3_t project_to_screen(const projection_type proj_type, const mat4x4_t* mat, const vec3_t v) {
    const vec4_t clip = mat4x4_mul_vec4(mat, (vec4_t){v.x, v.y, v.z, 1.0f});
    const float inv_w = 1.0f / clip.w;

    const float ndc_x = clip.x * inv_w;
    const float ndc_y = clip.y * inv_w;

    const float screen_x = (ndc_x * 0.5f + 0.5f) * SCREEN_WIDTH;
    const float screen_y = (-ndc_y * 0.5f + 0.5f) * SCREEN_HEIGHT;

    if (proj_type == PERSPECTIVE) {
        return (vec3_t){screen_x, screen_y, inv_w};
    }

    return (vec3_t){screen_x, screen_y, -clip.z};
}

static vec3_t barycentric_weights(const vec2_t a, const vec2_t b, const vec2_t c, const vec2_t p) {
    const vec2_t ac = vec2_diff(c, a);
    const vec2_t ab = vec2_diff(b, a);
    const vec2_t ap = vec2_diff(p, a);
    const vec2_t pc = vec2_diff(c, p);
    const vec2_t pb = vec2_diff(b, p);

    const float area = ac.x * ab.y - ac.y * ab.x;

    const float alpha = (pc.x * pb.y - pc.y * pb.x) / area;
    const float beta  = (ac.x * ap.y - ac.y * ap.x) / area;
    const float gamma = 1.0f - alpha - beta;

    return (vec3_t){alpha, beta, gamma};
}

static bool is_back_face(const projection_type proj_type, const vec3_t v1, const vec3_t v2, const vec3_t v3) {
    const vec3_t edge1 = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    const vec3_t edge2 = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};

    const vec3_t cross = vec3_cross(edge1, edge2);
    const vec3_t cross_norm = vec3_normalize(cross);

    vec3_t to_camera;
    if (proj_type == PERSPECTIVE) {
        to_camera = vec3_normalize(v1);
    }
    else {
        to_camera = (vec3_t){0.0f, 0.0f, -1.0f};
    }

    return vec3_dot(cross_norm, to_camera) >= 0.0f;
}

static bool is_outside_frustum(const vec3_t p1, const vec3_t p2, const vec3_t p3) {
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

static bool is_point_outside_viewport(const int x, const int y) {
    return x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT;
}

static float clamp(const float value, const float min, const float max) {
    const float t = value < min ? min : value;
    return t > max ? max : t;
}

static void draw_pixel(const sdl_gfx* gfx, const float x, const float y, const vec3_t p1, const vec3_t p2, const vec3_t p3, const uint32_t color, z_buffer_t* z_buffer) {
    if (is_point_outside_viewport((int)x, (int)y))
        return;

    const vec2_t p = (vec2_t){(float)x, (float)y};
    const vec2_t p1_xy = vec2_from_vec3(p1);
    const vec2_t p2_xy = vec2_from_vec3(p2);
    const vec2_t p3_xy = vec2_from_vec3(p3);

    const vec3_t weights = barycentric_weights(p1_xy, p2_xy, p3_xy, p);

    const float alpha = weights.x;
    const float beta  = weights.y;
    const float gamma = weights.z;

    const float depth = 1.0f / (alpha*p1.z + beta*p2.z + gamma*p3.z);

    const int z_index = SCREEN_WIDTH * y + x;
    if (depth <= (*z_buffer)[z_index]) {
        sdl_gfx_draw_pixel(gfx, (int)x, (int)y, color);
        (*z_buffer)[z_index] = depth;
    }
}

static void draw_pixel_phong(const sdl_gfx* gfx, float x, float y, vec3_t v1, vec3_t v2, vec3_t v3, vec3_t p1, vec3_t p2, vec3_t p3, vec3_t n1, vec3_t n2, vec3_t n3, uint32_t color, light_t light, z_buffer_t* z_buffer) {
    if (is_point_outside_viewport((int)x, (int)y))
        return;

    const vec2_t p = (vec2_t){x, y};
    const vec2_t p1_xy = vec2_from_vec3(p1);
    const vec2_t p2_xy = vec2_from_vec3(p2);
    const vec2_t p3_xy = vec2_from_vec3(p3);

    const vec3_t weights = barycentric_weights(p1_xy, p2_xy, p3_xy, p);

    const float alpha = weights.x;
    const float beta  = weights.y;
    const float gamma = weights.z;

    const float depth = 1.0f / (alpha*p1.z + beta*p2.z + gamma*p3.z);

    const int z_index = SCREEN_WIDTH * y + x;
    if (depth <= (*z_buffer)[z_index]) {
        const vec3_t va = vec3_mul(vec3_mul(v1, p1.z), alpha);
        const vec3_t vb = vec3_mul(vec3_mul(v2, p2.z), beta);
        const vec3_t vc = vec3_mul(vec3_mul(v3, p3.z), gamma);
        vec3_t interp_pos = vec3_mul(vec3_add(vec3_add(va, vb), vc), depth);

        const vec3_t na = vec3_mul(n1, alpha);
        const vec3_t nb = vec3_mul(n2, beta);
        const vec3_t nc = vec3_mul(n3, gamma);
        vec3_t interp_normal = vec3_normalize(vec3_add(vec3_add(na, nb), nc));

        vec3_t light_vec = vec3_normalize(vec3_diff(light.position, interp_pos));
        float diffuse = vec3_dot(interp_normal, light_vec);
        float intensity = clamp(AMBIENT_LIGHT_INTENSITY_PHONG + diffuse * light.strength, 0.0f, 1.0f);

        const uint32_t r = RED(color) * intensity;
        const uint32_t g = GREEN(color) * intensity;
        const uint32_t b = BLUE(color) * intensity;

        sdl_gfx_draw_pixel(gfx, (int)x, (int)y, RGB(r,g,b));
        (*z_buffer)[z_index] = depth;
    }
}

static void draw_line(const sdl_gfx* gfx, const vec2_t a, const vec2_t b, const uint32_t color) {
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

static void draw_filled_triangle(const sdl_gfx* gfx, vec3_t p1, vec3_t p2, vec3_t p3, const uint32_t color, z_buffer_t* z_buffer) {
    sort_points(&p1, &p2, &p3);
    vec3_floor_xy(&p1);
    vec3_floor_xy(&p2);
    vec3_floor_xy(&p3);

    // Draw flat-bottom triangle
    if (p2.y != p1.y) {
        const float inv_slope1 = (p2.x - p1.x) / (p2.y - p1.y);
        const float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

        float y = p1.y;
        while (y <= p2.y) {
            float x_start = p1.x + (y - p1.y) * inv_slope1;
            float x_end   = p1.x + (y - p1.y) * inv_slope2;

            if (x_start > x_end) {
                const float temp = x_start;
                x_start = x_end;
                x_end = temp;
            }

            float x = x_start;
            while (x <= x_end) {
                draw_pixel(gfx, x, y, p1, p2, p3, color, z_buffer);
                x += 1.0f;
            }
            y += 1.0f;
        }
    }

    // Draw flat-top triangle
    if (p3.y != p2.y) {
        const float inv_slope1 = (p3.x - p2.x) / (p3.y - p2.y);
        const float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

        float y = p2.y;
        while (y <= p3.y) {
            float x_start = p2.x + (y - p2.y) * inv_slope1;
            float x_end   = p1.x + (y - p1.y) * inv_slope2;

            if (x_start > x_end) {
                const float temp = x_start;
                x_start = x_end;
                x_end = temp;
            }

            float x = x_start;
            while (x <= x_end) {
                draw_pixel(gfx, x, y, p1, p2, p3, color, z_buffer);
                x += 1.0f;
            }
            y += 1.0f;
        }
    }
}

static void draw_texel_flat_shaded(const sdl_gfx* gfx, const float x, const float y, const vec3_t p1, const vec3_t p2, const vec3_t p3, const vec2_t uv1, const vec2_t uv2, const  vec2_t uv3, const texture_t* texture, float intensity, z_buffer_t* z_buffer) {
    if (is_point_outside_viewport((int)x, (int)y))
        return;

    const vec2_t p = (vec2_t){(float)x, (float)y};
    const vec2_t p1_xy = vec2_from_vec3(p1);
    const vec2_t p2_xy = vec2_from_vec3(p2);
    const vec2_t p3_xy = vec2_from_vec3(p3);

    const vec3_t weights = barycentric_weights(p1_xy, p2_xy, p3_xy, p);

    const float alpha = weights.x;
    const float beta  = weights.y;
    const float gamma = weights.z;

    const float depth = 1.0f / (alpha*p1.z + beta*p2.z + gamma*p3.z);

    const int z_index = SCREEN_WIDTH * y + x;
    if (depth <= (*z_buffer)[z_index]) {
        const float interp_u = ((uv1.x * p1.z) * alpha + (uv2.x * p2.z) * beta + (uv3.x * p3.z) * gamma) * depth;
        const float interp_v = ((uv1.y * p1.z) * alpha + (uv2.y * p2.z) * beta + (uv3.y * p3.z) * gamma) * depth;
        const int tex_x = (int)(interp_u * (float)texture->width) & texture->width - 1;
        const int tex_y = (int)(interp_v * (float)texture->height) & texture->height - 1;
        const uint32_t tex = texture->pixels[tex_y * texture->width + tex_x];

        const uint32_t r = RED(tex) * intensity;
        const uint32_t g = GREEN(tex) * intensity;
        const uint32_t b = BLUE(tex) * intensity;

        sdl_gfx_draw_pixel(gfx, (int)x, (int)y, RGB(r,g,b));
        (*z_buffer)[z_index] = depth;
    }
}

static void draw_textured_triangle_flat_shaded(const sdl_gfx* gfx, vec3_t p1, vec3_t p2, vec3_t p3, vec2_t uv1, vec2_t uv2, vec2_t uv3, const texture_t* texture, const float intensity, z_buffer_t* z_buffer) {
    sort_points_and_uvs(&p1, &p2, &p3, &uv1, &uv2, &uv3);
    vec3_floor_xy(&p1);
    vec3_floor_xy(&p2);
    vec3_floor_xy(&p3);

    // Draw flat-bottom triangle
    if (p2.y != p1.y) {
        const float inv_slope1 = (p2.x - p1.x) / (p2.y - p1.y);
        const float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

        float y = p1.y;
        while (y <= p2.y) {
            float x_start = p1.x + (y - p1.y) * inv_slope1;
            float x_end   = p1.x + (y - p1.y) * inv_slope2;

            if (x_start > x_end) {
                const float temp = x_start;
                x_start = x_end;
                x_end = temp;
            }

            float x = x_start;
            while (x <= x_end) {
                draw_texel_flat_shaded(gfx, x, y, p1, p2, p3, uv1, uv2, uv3, texture, intensity, z_buffer);
                x += 1.0f;
            }
            y += 1.0f;
        }
    }

    // Draw flat-top triangle
    if (p3.y != p2.y) {
        const float inv_slope1 = (p3.x - p2.x) / (p3.y - p2.y);
        const float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

        float y = p2.y;
        while (y <= p3.y) {
            float x_start = p2.x + (y - p2.y) * inv_slope1;
            float x_end   = p1.x + (y - p1.y) * inv_slope2;

            if (x_start > x_end) {
                const float temp = x_start;
                x_start = x_end;
                x_end = temp;
            }

            float x = x_start;
            while (x <= x_end) {
                draw_texel_flat_shaded(gfx, x, y, p1, p2, p3, uv1, uv2, uv3, texture, intensity, z_buffer);
                x += 1.0f;
            }
            y += 1.0f;
        }
    }
}

static void draw_filled_triangle_phong(const sdl_gfx* gfx, vec3_t v1, vec3_t v2, vec3_t v3, vec3_t p1, vec3_t p2, vec3_t p3, const vec3_t n1, const vec3_t n2, const vec3_t n3, const uint32_t color, const light_t light, z_buffer_t* z_buffer) {
    sort_points_and_vertices(&p1, &p2, &p3, &v1, &v2, &v3);
    vec3_floor_xy(&p1);
    vec3_floor_xy(&p2);
    vec3_floor_xy(&p3);

    // Draw flat-bottom triangle
    if (p2.y != p1.y) {
        const float inv_slope1 = (p2.x - p1.x) / (p2.y - p1.y);
        const float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

        float y = p1.y;
        while (y <= p2.y) {
            float x_start = p1.x + (y - p1.y) * inv_slope1;
            float x_end   = p1.x + (y - p1.y) * inv_slope2;

            if (x_start > x_end) {
                const float temp = x_start;
                x_start = x_end;
                x_end = temp;
            }

            float x = x_start;
            while (x <= x_end) {
                draw_pixel_phong(gfx, x, y, v1, v2, v3, p1, p2, p3, n1, n2, n3, color, light, z_buffer);
                x += 1.0f;
            }
            y += 1.0f;
        }
    }

    // Draw flat-top triangle
    if (p3.y != p2.y) {
        const float inv_slope1 = (p3.x - p2.x) / (p3.y - p2.y);
        const float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

        float y = p2.y;
        while (y <= p3.y) {
            float x_start = p2.x + (y - p2.y) * inv_slope1;
            float x_end   = p1.x + (y - p1.y) * inv_slope2;

            if (x_start > x_end) {
                const float temp = x_start;
                x_start = x_end;
                x_end = temp;
            }

            float x = x_start;
            while (x <= x_end) {
                draw_pixel_phong(gfx, x, y, v1, v2, v3, p1, p2, p3, n1, n2, n3, color, light, z_buffer);
                x += 1.0f;
            }
            y += 1.0f;
        }
    }
}

static void draw_texel_phong(const sdl_gfx* gfx, const float x, const float y, vec3_t v1, vec3_t v2, vec3_t v3, vec3_t p1, vec3_t p2, vec3_t p3, vec3_t n1, vec3_t n2, vec3_t n3, const vec2_t uv1, const vec2_t uv2, const  vec2_t uv3, const texture_t* texture, light_t light, z_buffer_t* z_buffer) {
    if (is_point_outside_viewport((int)x, (int)y))
        return;

    const vec2_t p = (vec2_t){x, y};
    const vec2_t p1_xy = vec2_from_vec3(p1);
    const vec2_t p2_xy = vec2_from_vec3(p2);
    const vec2_t p3_xy = vec2_from_vec3(p3);

    const vec3_t weights = barycentric_weights(p1_xy, p2_xy, p3_xy, p);

    const float alpha = weights.x;
    const float beta  = weights.y;
    const float gamma = weights.z;

    const float depth = 1.0f / (alpha*p1.z + beta*p2.z + gamma*p3.z);

    const int z_index = SCREEN_WIDTH * y + x;
    if (depth <= (*z_buffer)[z_index]) {
        const float interp_u = ((uv1.x * p1.z) * alpha + (uv2.x * p2.z) * beta + (uv3.x * p3.z) * gamma) * depth;
        const float interp_v = ((uv1.y * p1.z) * alpha + (uv2.y * p2.z) * beta + (uv3.y * p3.z) * gamma) * depth;
        const int tex_x = (int)(interp_u * (float)texture->width) & texture->width - 1;
        const int tex_y = (int)(interp_v * (float)texture->height) & texture->height - 1;
        const uint32_t tex = texture->pixels[tex_y * texture->width + tex_x];

        const vec3_t va = vec3_mul(vec3_mul(v1, p1.z), alpha);
        const vec3_t vb = vec3_mul(vec3_mul(v2, p2.z), beta);
        const vec3_t vc = vec3_mul(vec3_mul(v3, p3.z), gamma);
        vec3_t interp_pos = vec3_mul(vec3_add(vec3_add(va, vb), vc), depth);

        const vec3_t na = vec3_mul(n1, alpha);
        const vec3_t nb = vec3_mul(n2, beta);
        const vec3_t nc = vec3_mul(n3, gamma);
        vec3_t interp_normal = vec3_normalize(vec3_add(vec3_add(na, nb), nc));

        vec3_t light_vec = vec3_normalize(vec3_diff(light.position, interp_pos));
        float diffuse = vec3_dot(interp_normal, light_vec);
        float intensity = clamp(AMBIENT_LIGHT_INTENSITY_PHONG + diffuse * light.strength, 0.0f, 1.0f);

        const uint32_t r = RED(tex) * intensity;
        const uint32_t g = GREEN(tex) * intensity;
        const uint32_t b = BLUE(tex) * intensity;

        sdl_gfx_draw_pixel(gfx, (int)x, (int)y, RGB(r,g,b));
        (*z_buffer)[z_index] = depth;
    }
}

static void draw_textured_triangle_phong_shaded(const sdl_gfx* gfx, vec3_t v1, vec3_t v2, vec3_t v3, vec3_t p1, vec3_t p2, vec3_t p3, vec2_t uv1, vec2_t uv2, vec2_t uv3, const vec3_t n1, const vec3_t n2, const vec3_t n3, const texture_t* texture, const light_t light, z_buffer_t* z_buffer) {
    sort_points_vertices_and_uvs(&p1, &p2, &p3, &v1, &v2, &v3, &uv1, &uv2, &uv3);
    vec3_floor_xy(&p1);
    vec3_floor_xy(&p2);
    vec3_floor_xy(&p3);

    // Draw flat-bottom triangle
    if (p2.y != p1.y) {
        const float inv_slope1 = (p2.x - p1.x) / (p2.y - p1.y);
        const float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

        float y = p1.y;
        while (y <= p2.y) {
            float x_start = p1.x + (y - p1.y) * inv_slope1;
            float x_end   = p1.x + (y - p1.y) * inv_slope2;

            if (x_start > x_end) {
                const float temp = x_start;
                x_start = x_end;
                x_end = temp;
            }

            float x = x_start;
            while (x <= x_end) {
                draw_texel_phong(gfx, x, y, v1, v2, v3, p1, p2, p3, n1, n2, n3, uv1, uv2, uv3, texture, light, z_buffer);
                x += 1.0f;
            }
            y += 1.0f;
        }
    }

    // Draw flat-top triangle
    if (p3.y != p2.y) {
        const float inv_slope1 = (p3.x - p2.x) / (p3.y - p2.y);
        const float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

        float y = p2.y;
        while (y <= p3.y) {
            float x_start = p2.x + (y - p2.y) * inv_slope1;
            float x_end   = p1.x + (y - p1.y) * inv_slope2;

            if (x_start > x_end) {
                const float temp = x_start;
                x_start = x_end;
                x_end = temp;
            }

            float x = x_start;
            while (x <= x_end) {
                draw_texel_phong(gfx, x, y, v1, v2, v3, p1, p2, p3, n1, n2, n3, uv1, uv2, uv3, texture, light, z_buffer);
                x += 1.0f;
            }
            y += 1.0f;
        }
    }
}


void draw_wireframe(
    const sdl_gfx* gfx,
    const vec3_t* vertices,
    const triangle_t* tris,
    const int tris_count,
    const uint32_t color,
    const mat4x4_t* proj_mat,
    const projection_type proj_type,
    const bool cull_back_face
    ) {
    for (int i = 0; i < tris_count; ++i) {
        const triangle_t tri = tris[i];

        const vec3_t v1 = vertices[tri.v[0]];
        const vec3_t v2 = vertices[tri.v[1]];
        const vec3_t v3 = vertices[tri.v[2]];

        if (cull_back_face && is_back_face(proj_type, v1, v2, v3))
            continue;

        const vec3_t p1 = project_to_screen(proj_type, proj_mat, v1);
        const vec3_t p2 = project_to_screen(proj_type, proj_mat, v2);
        const vec3_t p3 = project_to_screen(proj_type, proj_mat, v3);

        if (is_outside_frustum(p1, p2, p3))
            continue;

        draw_line(gfx, (vec2_t){p1.x, p1.y}, (vec2_t){p2.x, p2.y}, color);
        draw_line(gfx, (vec2_t){p2.x, p2.y}, (vec2_t){p3.x, p3.y}, color);
        draw_line(gfx, (vec2_t){p3.x, p3.y}, (vec2_t){p1.x, p1.y}, color);
    }
}

void draw_unlit(const sdl_gfx* gfx, const vec3_t* vertices, const triangle_t* tris, const int tris_count, const uint32_t color, const mat4x4_t* proj_mat, const projection_type proj_type, z_buffer_t* z_buffer) {
    for (int i = 0; i < tris_count; ++i) {
        const triangle_t tri = tris[i];

        const vec3_t v1 = vertices[tri.v[0]];
        const vec3_t v2 = vertices[tri.v[1]];
        const vec3_t v3 = vertices[tri.v[2]];

        if (is_back_face(proj_type, v1, v2, v3))
            continue;

        const vec3_t p1 = project_to_screen(proj_type, proj_mat, v1);
        const vec3_t p2 = project_to_screen(proj_type, proj_mat, v2);
        const vec3_t p3 = project_to_screen(proj_type, proj_mat, v3);

        if (is_outside_frustum(p1, p2, p3))
            continue;

        draw_filled_triangle(gfx, p1, p2, p3, color, z_buffer);
    }
}

void draw_flat_shaded(const sdl_gfx* gfx, const vec3_t* vertices, const triangle_t* tris, const int tris_count, const uint32_t color, const light_t light, const mat4x4_t* proj_mat, const projection_type proj_type, z_buffer_t* z_buffer) {
    for (int i = 0; i < tris_count; ++i) {
        const triangle_t tri = tris[i];

        const vec3_t v1 = vertices[tri.v[0]];
        const vec3_t v2 = vertices[tri.v[1]];
        const vec3_t v3 = vertices[tri.v[2]];

        const vec3_t cross = vec3_cross(vec3_diff(v2, v1), vec3_diff(v3, v1));
        const vec3_t cross_norm = vec3_normalize(cross);
        const vec3_t to_camera = proj_type == PERSPECTIVE ? vec3_normalize(v1) : (vec3_t){0,0,-1};

        // Backface culling
        if (vec3_dot(cross_norm, to_camera) > 0.0f)
            continue;

        const vec3_t p1 = project_to_screen(proj_type, proj_mat, v1);
        const vec3_t p2 = project_to_screen(proj_type, proj_mat, v2);
        const vec3_t p3 = project_to_screen(proj_type, proj_mat, v3);

        if (is_outside_frustum(p1, p2, p3))
            continue;

        float intensity = vec3_dot(cross_norm, light.direction);
        intensity = clamp(intensity, 0.0f, 1.0f);
        intensity = clamp(AMBIENT_LIGHT_INTENSITY + intensity * light.strength, 0.0f, 1.0f);
        const uint32_t r = RED(color) * intensity;
        const uint32_t g = GREEN(color) * intensity;
        const uint32_t b = BLUE(color) * intensity;

        draw_filled_triangle(gfx, p1, p2, p3, RGB(r,g,b), z_buffer);
    }
}

void draw_phong_shaded(const sdl_gfx* gfx, const vec3_t* vertices, const vec3_t* normals, const triangle_t* tris, const int tris_count, const uint32_t color, const light_t light, const mat4x4_t* proj_mat, const projection_type proj_type, z_buffer_t* z_buffer) {
    for (int i = 0; i < tris_count; ++i) {
        const triangle_t tri = tris[i];

        const vec3_t v1 = vertices[tri.v[0]];
        const vec3_t v2 = vertices[tri.v[1]];
        const vec3_t v3 = vertices[tri.v[2]];

        const vec3_t n1 = normals[tri.n[0]];
        const vec3_t n2 = normals[tri.n[1]];
        const vec3_t n3 = normals[tri.n[2]];

        if (is_back_face(proj_type, v1, v2, v3))
            continue;

        const vec3_t p1 = project_to_screen(proj_type, proj_mat, v1);
        const vec3_t p2 = project_to_screen(proj_type, proj_mat, v2);
        const vec3_t p3 = project_to_screen(proj_type, proj_mat, v3);

        if (is_outside_frustum(p1, p2, p3))
            continue;

        draw_filled_triangle_phong(gfx, v1, v2, v3, p1, p2, p3, n1, n2, n3, color, light, z_buffer);
    }
}

void draw_textured_unlit(const sdl_gfx* gfx, const vec3_t* vertices, const triangle_t* tris, const int tris_count, const vec2_t* uvs, const texture_t* texture, const mat4x4_t* proj_mat, const projection_type proj_type, z_buffer_t* z_buffer) {
    for (int i = 0; i < tris_count; ++i) {
        const triangle_t tri = tris[i];

        const vec3_t v1 = vertices[tri.v[0]];
        const vec3_t v2 = vertices[tri.v[1]];
        const vec3_t v3 = vertices[tri.v[2]];

        const vec2_t uv1 = uvs[tri.uv[0]];
        const vec2_t uv2 = uvs[tri.uv[1]];
        const vec2_t uv3 = uvs[tri.uv[2]];

        if (is_back_face(proj_type, v1, v2, v3))
            continue;

        const vec3_t p1 = project_to_screen(proj_type, proj_mat, v1);
        const vec3_t p2 = project_to_screen(proj_type, proj_mat, v2);
        const vec3_t p3 = project_to_screen(proj_type, proj_mat, v3);

        if (is_outside_frustum(p1, p2, p3))
            continue;

        draw_textured_triangle_flat_shaded(gfx, p1, p2, p3, uv1, uv2, uv3, texture, 1.0f /* Unlit */, z_buffer);
    }
}

void draw_textured_flat_shaded(const sdl_gfx *gfx, const vec3_t *vertices, const triangle_t *tris, const int tris_count, const vec2_t *uvs, const texture_t *texture, const light_t light, const mat4x4_t *proj_mat, const projection_type proj_type, z_buffer_t *z_buffer) {
    for (int i = 0; i < tris_count; ++i) {
        const triangle_t tri = tris[i];

        const vec3_t v1 = vertices[tri.v[0]];
        const vec3_t v2 = vertices[tri.v[1]];
        const vec3_t v3 = vertices[tri.v[2]];

        const vec2_t uv1 = uvs[tri.uv[0]];
        const vec2_t uv2 = uvs[tri.uv[1]];
        const vec2_t uv3 = uvs[tri.uv[2]];

        const vec3_t cross = vec3_cross(vec3_diff(v2, v1), vec3_diff(v3, v1));
        const vec3_t cross_norm = vec3_normalize(cross);
        const vec3_t to_camera = proj_type == PERSPECTIVE ? vec3_normalize(v1) : (vec3_t){0,0,-1};

        // Backface culling
        if (vec3_dot(cross_norm, to_camera) > 0.0f)
            continue;

        const vec3_t p1 = project_to_screen(proj_type, proj_mat, v1);
        const vec3_t p2 = project_to_screen(proj_type, proj_mat, v2);
        const vec3_t p3 = project_to_screen(proj_type, proj_mat, v3);

        if (is_outside_frustum(p1, p2, p3))
            continue;

        float intensity = vec3_dot(cross_norm, light.direction);
        intensity = clamp(intensity, 0.0f, 1.0f);
        intensity = clamp(AMBIENT_LIGHT_INTENSITY + intensity * light.strength, 0.0f, 1.0f);

        draw_textured_triangle_flat_shaded(gfx, p1, p2, p3, uv1, uv2, uv3, texture, intensity, z_buffer);
    }
}

void draw_textured_phong_shaded(const sdl_gfx *gfx, const vec3_t *vertices, const vec3_t *normals, const triangle_t *tris, const int tris_count, const vec2_t *uvs, const texture_t *texture, const light_t light, const mat4x4_t *proj_mat, const projection_type proj_type, z_buffer_t *z_buffer) {
    for (int i = 0; i < tris_count; ++i) {
        const triangle_t tri = tris[i];

        const vec3_t v1 = vertices[tri.v[0]];
        const vec3_t v2 = vertices[tri.v[1]];
        const vec3_t v3 = vertices[tri.v[2]];

        const vec2_t uv1 = uvs[tri.uv[0]];
        const vec2_t uv2 = uvs[tri.uv[1]];
        const vec2_t uv3 = uvs[tri.uv[2]];

        const vec3_t n1 = normals[tri.n[0]];
        const vec3_t n2 = normals[tri.n[1]];
        const vec3_t n3 = normals[tri.n[2]];

        if (is_back_face(proj_type, v1, v2, v3))
            continue;

        const vec3_t p1 = project_to_screen(proj_type, proj_mat, v1);
        const vec3_t p2 = project_to_screen(proj_type, proj_mat, v2);
        const vec3_t p3 = project_to_screen(proj_type, proj_mat, v3);

        if (is_outside_frustum(p1, p2, p3))
            continue;

        draw_textured_triangle_phong_shaded(gfx, v1, v2, v3, p1, p2, p3, uv1, uv2, uv3, n1, n2, n3, texture, light, z_buffer);
    }
}
