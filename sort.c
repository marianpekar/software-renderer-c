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

void swap_points(vec3_t *p1, vec3_t *p2) {
    const vec3_t temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}
