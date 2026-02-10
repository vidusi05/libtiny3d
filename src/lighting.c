// lighting.c
#include "lighting.h"
#include "math3d.h"  // For vec3_t operations
#include <math.h>

float dot_product(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t normalize(vec3_t v) {
    float mag = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (mag == 0) return vec3_zero();
    return vec3_div(v, mag);
}

float compute_lighting(vec3_t edge_dir, vec3_t *lights, int light_count) {
    float intensity = 0.0f;
    edge_dir = vec3_normalize(edge_dir);

    for (int i = 0; i < light_count; i++) {
        vec3_t light_dir = vec3_normalize(lights[i]);
        float dot = vec3_dot(edge_dir, light_dir);
        intensity += fmaxf(0.0f, dot);
    }

    return fminf(intensity, 1.0f);
}