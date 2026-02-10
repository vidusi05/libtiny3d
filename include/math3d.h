#ifndef MATH3D_H
#define MATH3D_H

#include <math.h>

// ============================================================================
// VECTOR 3D DEFINITION
// ============================================================================

typedef struct {
    float x, y, z;
} vec3_t;

// Keep Vec3 as an alias for compatibility
typedef vec3_t Vec3;

// ============================================================================
// MATRIX 4x4 DEFINITION (for transformations)
// ============================================================================

typedef struct {
    float m[16]; // Column-major order like OpenGL
} mat4_t;

// Keep Mat4 as an alias for compatibility
typedef mat4_t Mat4;

// ============================================================================
// VECTOR 3D FUNCTIONS
// ============================================================================

// Construction
vec3_t vec3_new(float x, float y, float z);
vec3_t vec3_zero(void);
vec3_t vec3_one(void);
vec3_t vec3_from_cartesian(float x, float y, float z); // Alias for vec3_new

// Basic operations
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_mul(vec3_t v, float s);
vec3_t vec3_div(vec3_t v, float s);

// Vector operations
float vec3_dot(vec3_t a, vec3_t b);
vec3_t vec3_cross(vec3_t a, vec3_t b);
float vec3_length(vec3_t v);
float vec3_length_squared(vec3_t v);
vec3_t vec3_normalize(vec3_t v);

// Utility
vec3_t vec3_lerp(vec3_t a, vec3_t b, float t);
float vec3_distance(vec3_t a, vec3_t b);

// ============================================================================
// MATRIX 4x4 FUNCTIONS
// ============================================================================

mat4_t mat4_identity(void);
mat4_t mat4_translation(vec3_t translation);
mat4_t mat4_rotation_x(float angle);
mat4_t mat4_rotation_y(float angle);
mat4_t mat4_rotation_z(float angle);
mat4_t mat4_rotate_xyz(float x, float y, float z); // Combined rotation
mat4_t mat4_scale(float x, float y, float z);
mat4_t mat4_scale_uniform(float s);
mat4_t mat4_multiply(mat4_t a, mat4_t b);
mat4_t mat4_mul(mat4_t a, mat4_t b); // Alias for mat4_multiply
vec3_t mat4_transform_point(mat4_t m, vec3_t p);
vec3_t mat4_transform_vector(mat4_t m, vec3_t v);

// Projection matrices
mat4_t mat4_perspective(float fov, float aspect, float near, float far);
mat4_t mat4_ortho(float left, float right, float bottom, float top, float near, float far);
mat4_t mat4_look_at(vec3_t eye, vec3_t center, vec3_t up);

// New added declarations
mat4_t mat4_translate(float x, float y, float z);
vec3_t mat4_mul_vec3(mat4_t m, vec3_t v);
mat4_t mat4_frustum_asymmetric(float left, float right, float bottom, float top, float near, float far);
mat4_t mat4_frustum_asymmetric(float left, float right, float bottom, float top, float near, float far);


#endif // MATH3D_H
