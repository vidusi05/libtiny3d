#include "math3d.h"
#include <math.h>
#include <string.h>

// ============================================================================
// VECTOR 3D FUNCTIONS
// ============================================================================

vec3_t vec3_new(float x, float y, float z) {
    vec3_t v = {x, y, z};
    return v;
}

vec3_t vec3_from_cartesian(float x, float y, float z) {
    return vec3_new(x, y, z);
}

vec3_t vec3_zero(void) {
    vec3_t v = {0.0f, 0.0f, 0.0f};
    return v;
}

vec3_t vec3_one(void) {
    vec3_t v = {1.0f, 1.0f, 1.0f};
    return v;
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
    vec3_t result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
    vec3_t result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

vec3_t vec3_mul(vec3_t v, float s) {
    vec3_t result = {v.x * s, v.y * s, v.z * s};
    return result;
}

vec3_t vec3_div(vec3_t v, float s) {
    if (s == 0.0f) return vec3_zero();
    vec3_t result = {v.x / s, v.y / s, v.z / s};
    return result;
}

float vec3_dot(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    vec3_t result = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return result;
}

float vec3_length_squared(vec3_t v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float vec3_length(vec3_t v) {
    return sqrtf(vec3_length_squared(v));
}

vec3_t vec3_normalize(vec3_t v) {
    float len = vec3_length(v);
    if (len == 0.0f) return vec3_zero();
    return vec3_div(v, len);
}

vec3_t vec3_lerp(vec3_t a, vec3_t b, float t) {
    vec3_t diff = vec3_sub(b, a);
    vec3_t scaled = vec3_mul(diff, t);
    return vec3_add(a, scaled);
}

float vec3_distance(vec3_t a, vec3_t b) {
    vec3_t diff = vec3_sub(b, a);
    return vec3_length(diff);
}

// ============================================================================
// MATRIX 4x4 FUNCTIONS
// ============================================================================

mat4_t mat4_identity(void) {
    mat4_t m;
    memset(m.m, 0, sizeof(m.m));
    m.m[0] = 1.0f;
    m.m[5] = 1.0f;
    m.m[10] = 1.0f;
    m.m[15] = 1.0f;
    return m;
}

mat4_t mat4_translation(vec3_t translation) {
    mat4_t m = mat4_identity();
    m.m[12] = translation.x;
    m.m[13] = translation.y;
    m.m[14] = translation.z;
    return m;
}

// NEW: mat4_translate using x, y, z directly
mat4_t mat4_translate(float x, float y, float z) {
    mat4_t result = mat4_identity();
    result.m[12] = x;
    result.m[13] = y;
    result.m[14] = z;
    return result;
}

mat4_t mat4_rotation_x(float angle) {
    mat4_t m = mat4_identity();
    float c = cosf(angle);
    float s = sinf(angle);
    m.m[5] = c;
    m.m[6] = s;
    m.m[9] = -s;
    m.m[10] = c;
    return m;
}

mat4_t mat4_rotation_y(float angle) {
    mat4_t m = mat4_identity();
    float c = cosf(angle);
    float s = sinf(angle);
    m.m[0] = c;
    m.m[2] = -s;
    m.m[8] = s;
    m.m[10] = c;
    return m;
}

mat4_t mat4_rotation_z(float angle) {
    mat4_t m = mat4_identity();
    float c = cosf(angle);
    float s = sinf(angle);
    m.m[0] = c;
    m.m[1] = s;
    m.m[4] = -s;
    m.m[5] = c;
    return m;
}

mat4_t mat4_rotate_xyz(float x, float y, float z) {
    mat4_t rx = mat4_rotation_x(x);
    mat4_t ry = mat4_rotation_y(y);
    mat4_t rz = mat4_rotation_z(z);
    return mat4_mul(mat4_mul(rz, ry), rx);
}

mat4_t mat4_scale(float x, float y, float z) {
    mat4_t m = mat4_identity();
    m.m[0] = x;
    m.m[5] = y;
    m.m[10] = z;
    return m;
}

mat4_t mat4_scale_uniform(float s) {
    return mat4_scale(s, s, s);
}

mat4_t mat4_multiply(mat4_t a, mat4_t b) {
    mat4_t result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i * 4 + j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                result.m[i * 4 + j] += a.m[i * 4 + k] * b.m[k * 4 + j];
            }
        }
    }
    return result;
}

mat4_t mat4_mul(mat4_t a, mat4_t b) {
    return mat4_multiply(a, b);
}

vec3_t mat4_transform_point(mat4_t m, vec3_t p) {
    float x = m.m[0] * p.x + m.m[4] * p.y + m.m[8]  * p.z + m.m[12];
    float y = m.m[1] * p.x + m.m[5] * p.y + m.m[9]  * p.z + m.m[13];
    float z = m.m[2] * p.x + m.m[6] * p.y + m.m[10] * p.z + m.m[14];
    float w = m.m[3] * p.x + m.m[7] * p.y + m.m[11] * p.z + m.m[15];

    if (w != 0.0f) {
        x /= w;
        y /= w;
        z /= w;
    }

    return vec3_new(x, y, z);
}

vec3_t mat4_transform_vector(mat4_t m, vec3_t v) {
    float x = m.m[0] * v.x + m.m[4] * v.y + m.m[8]  * v.z;
    float y = m.m[1] * v.x + m.m[5] * v.y + m.m[9]  * v.z;
    float z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z;
    return vec3_new(x, y, z);
}

// NEW: multiply mat4 with vec3 (no perspective divide)
vec3_t mat4_mul_vec3(mat4_t m, vec3_t v) {
    vec3_t result;
    result.x = m.m[0]*v.x + m.m[4]*v.y + m.m[8]*v.z  + m.m[12];
    result.y = m.m[1]*v.x + m.m[5]*v.y + m.m[9]*v.z  + m.m[13];
    result.z = m.m[2]*v.x + m.m[6]*v.y + m.m[10]*v.z + m.m[14];
    return result;
}

mat4_t mat4_perspective(float fov, float aspect, float near, float far) {
    mat4_t m = {0};
    float f = 1.0f / tanf(fov * 0.5f);
    m.m[0] = f / aspect;
    m.m[5] = f;
    m.m[10] = (far + near) / (near - far);
    m.m[11] = -1.0f;
    m.m[14] = (2.0f * far * near) / (near - far);
    return m;
}

mat4_t mat4_ortho(float left, float right, float bottom, float top, float near, float far) {
    mat4_t m = {0};
    m.m[0] = 2.0f / (right - left);
    m.m[5] = 2.0f / (top - bottom);
    m.m[10] = -2.0f / (far - near);
    m.m[12] = -(right + left) / (right - left);
    m.m[13] = -(top + bottom) / (top - bottom);
    m.m[14] = -(far + near) / (far - near);
    m.m[15] = 1.0f;
    return m;
}

// NEW: asymmetric frustum matrix
mat4_t mat4_frustum_asymmetric(float left, float right, float bottom, float top, float near, float far) {
    mat4_t m = {0};
    m.m[0] = (2.0f * near) / (right - left);
    m.m[5] = (2.0f * near) / (top - bottom);
    m.m[8] = (right + left) / (right - left);
    m.m[9] = (top + bottom) / (top - bottom);
    m.m[10] = -(far + near) / (far - near);
    m.m[11] = -1.0f;
    m.m[14] = -(2.0f * far * near) / (far - near);
    return m;
}

mat4_t mat4_look_at(vec3_t eye, vec3_t center, vec3_t up) {
    vec3_t f = vec3_normalize(vec3_sub(center, eye));
    vec3_t s = vec3_normalize(vec3_cross(f, up));
    vec3_t u = vec3_cross(s, f);

    mat4_t m = mat4_identity();
    m.m[0] = s.x;  m.m[4] = s.y;  m.m[8]  = s.z;
    m.m[1] = u.x;  m.m[5] = u.y;  m.m[9]  = u.z;
    m.m[2] = -f.x; m.m[6] = -f.y; m.m[10] = -f.z;
    m.m[12] = -vec3_dot(s, eye);
    m.m[13] = -vec3_dot(u, eye);
    m.m[14] =  vec3_dot(f, eye);
    return m;
}
