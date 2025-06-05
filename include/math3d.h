#ifndef MATH3D_H
#define MATH3D_H

#include <stdbool.h>

typedef struct {
    // Cartesian coordinates
    float x, y, z;
    
    // Spherical coordinates (automatically updated)
    float r, theta, phi;
    
    // Dirty flags to avoid redundant conversions
    bool cartesian_dirty;
    bool spherical_dirty;
} vec3_t;

typedef struct {
    float m[16]; // Column-major 4x4 matrix
} mat4_t;

// Helper functions (add these declarations)
void update_cartesian(vec3_t* v);
void update_spherical(vec3_t* v);

// Vector operations
vec3_t vec3_from_cartesian(float x, float y, float z);
vec3_t vec3_from_spherical(float r, float theta, float phi);
vec3_t vec3_normalize_fast(vec3_t v);
vec3_t vec3_slerp(vec3_t a, vec3_t b, float t);
// Add this to math3d.h

// Matrix operations
mat4_t mat4_identity();
mat4_t mat4_translate(float tx, float ty, float tz);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_rotate_xyz(float rx, float ry, float rz);
mat4_t mat4_frustum_asymmetric(float left, float right, float bottom, float top, float near, float far);
// Add this to math3d.h
mat4_t mat4_mul(mat4_t a, mat4_t b);

// Matrix-vector multiplication
vec3_t mat4_mul_vec3(mat4_t m, vec3_t v);

#endif // MATH3D_H