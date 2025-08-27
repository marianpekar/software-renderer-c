#include "sort.h"

void sort_points(vec3_t *p1, vec3_t *p2, vec3_t *p3) {
    if (p1->y > p2->y) {
        swap_points(p1, p2);
    }
    if (p2->y > p3->y) {
        swap_points(p2, p3);
    }
    if (p1->y > p2->y) {
        swap_points(p1, p2);
    }
}

void sort_points_and_vertices(vec3_t *p1, vec3_t *p2, vec3_t *p3, vec3_t *v1, vec3_t *v2, vec3_t *v3) {
    if (p1->y > p2->y) {
        swap_points(p1, p2);
        swap_points(v1, v2);
    }
    if (p2->y > p3->y) {
        swap_points(p2, p3);
        swap_points(v2, v3);
    }
    if (p1->y > p2->y) {
        swap_points(p1, p2);
        swap_points(v1, v2);
    }
}

void swap_points(vec3_t *p1, vec3_t *p2) {
    const vec3_t temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}
