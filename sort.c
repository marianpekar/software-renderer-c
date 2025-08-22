#include "sort.h"

void sort_points(vec3 *p1, vec3 *p2, vec3 *p3) {
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

void swap_points(vec3 *p1, vec3 *p2) {
    const vec3 temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}
