#ifndef LIGHTING_H
#define LIGHTING_H

#include "math3d.h"

float compute_lighting(vec3_t edge_dir, vec3_t *lights, int light_count);

#endif