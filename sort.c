#include "sort.h"

static void vec3_swap_points(vec3_t *p1, vec3_t *p2) {
    const vec3_t temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

static void vec2_swap_points(vec2_t *p1, vec2_t *p2) {
    const vec2_t temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

void sort_points(vec3_t *p1, vec3_t *p2, vec3_t *p3) {
    if (p1->y > p2->y) {
        vec3_swap_points(p1, p2);
    }
    if (p2->y > p3->y) {
        vec3_swap_points(p2, p3);
    }
    if (p1->y > p2->y) {
        vec3_swap_points(p1, p2);
    }
}

void sort_points_and_vertices(vec3_t *p1, vec3_t *p2, vec3_t *p3, vec3_t *v1, vec3_t *v2, vec3_t *v3) {
    if (p1->y > p2->y) {
        vec3_swap_points(p1, p2);
        vec3_swap_points(v1, v2);
    }
    if (p2->y > p3->y) {
        vec3_swap_points(p2, p3);
        vec3_swap_points(v2, v3);
    }
    if (p1->y > p2->y) {
        vec3_swap_points(p1, p2);
        vec3_swap_points(v1, v2);
    }
}

void sort_points_and_uvs(vec3_t *p1, vec3_t *p2, vec3_t *p3, vec2_t *uv1, vec2_t *uv2, vec2_t *uv3) {
    if (p1->y > p2->y) {
        vec3_swap_points(p1, p2);
        vec2_swap_points(uv1, uv2);
    }
    if (p2->y > p3->y) {
        vec3_swap_points(p2, p3);
        vec2_swap_points(uv2, uv3);
    }
    if (p1->y > p2->y) {
        vec3_swap_points(p1, p2);
        vec2_swap_points(uv1, uv2);
    }
}

void sort_points_vertices_and_uvs(vec3_t *p1, vec3_t *p2, vec3_t *p3, vec3_t *v1, vec3_t *v2, vec3_t *v3, vec2_t *uv1, vec2_t *uv2, vec2_t *uv3) {
    if (p1->y > p2->y) {
        vec3_swap_points(p1, p2);
        vec3_swap_points(v1, v2);
        vec2_swap_points(uv1, uv2);
    }
    if (p2->y > p3->y) {
        vec3_swap_points(p2, p3);
        vec3_swap_points(v2, v3);
        vec2_swap_points(uv2, uv3);
    }
    if (p1->y > p2->y) {
        vec3_swap_points(p1, p2);
        vec3_swap_points(v1, v2);
        vec2_swap_points(uv1, uv2);
    }
}


