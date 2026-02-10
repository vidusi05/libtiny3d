#ifndef RENDERER_H
#define RENDERER_H

#include "canvas.h"
#include "math3d.h"
#include <stdbool.h>

// Camera structure
typedef struct {
    vec3_t position;
    vec3_t target;
    vec3_t up;
    float fov;
    float aspect;
    float near;
    float far;
} camera_t;

// Viewport structure
typedef struct {
    float center_x, center_y;
    float radius;
    int screen_width, screen_height;
} viewport_t;

// Edge structure for wireframe rendering
typedef struct {
    int v0, v1;  // vertex indices
    float depth; // average depth for sorting
} edge_t;

// 3D mesh structure
typedef struct {
    vec3_t* vertices;
    edge_t* edges;
    int vertex_count;
    int edge_count;
} mesh_t;

// Rendering functions
vec3_t project_vertex(vec3_t vertex, mat4_t model_matrix, mat4_t view_matrix, 
                     mat4_t projection_matrix, viewport_t viewport);
bool clip_to_circular_viewport(canvas_t* canvas, float x, float y, viewport_t viewport);
void render_wireframe(canvas_t* canvas, mesh_t* mesh, mat4_t model_matrix, 
                     camera_t camera, viewport_t viewport);
mesh_t* generate_soccer_ball();
void free_mesh(mesh_t* mesh);

// Camera helper functions
mat4_t create_view_matrix(camera_t camera);
mat4_t create_projection_matrix(camera_t camera);

#endif // RENDERER_H

