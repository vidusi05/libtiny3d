#include "canvas.h"
#include "math3d.h"
#include "renderer.h"
#include "lighting.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600
#define FPS 30
#define NUM_LIGHTS 2

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    canvas_t* canvas = create_canvas(WIDTH, HEIGHT);
    float aspect = (float)WIDTH / (float)HEIGHT;

    // Create meshes
    mesh_t* cube = malloc(sizeof(mesh_t));
    mesh_t* soccer_ball = generate_soccer_ball();

    // Initialize cube mesh (same as before)
    cube->vertex_count = 8;
    cube->vertices = malloc(cube->vertex_count * sizeof(vec3_t));
    cube->edge_count = 12;
    cube->edges = malloc(cube->edge_count * sizeof(edge_t));

    // Cube vertices
    cube->vertices[0] = vec3_from_cartesian(-0.5f, -0.5f,  0.5f);
    cube->vertices[1] = vec3_from_cartesian( 0.5f, -0.5f,  0.5f);
    cube->vertices[2] = vec3_from_cartesian( 0.5f,  0.5f,  0.5f);
    cube->vertices[3] = vec3_from_cartesian(-0.5f,  0.5f,  0.5f);
    cube->vertices[4] = vec3_from_cartesian(-0.5f, -0.5f, -0.5f);
    cube->vertices[5] = vec3_from_cartesian( 0.5f, -0.5f, -0.5f);
    cube->vertices[6] = vec3_from_cartesian( 0.5f,  0.5f, -0.5f);
    cube->vertices[7] = vec3_from_cartesian(-0.5f,  0.5f, -0.5f);

    // Cube edges
    int cube_edges[12][2] = {
        {0,1}, {1,2}, {2,3}, {3,0}, // front face
        {4,5}, {5,6}, {6,7}, {7,4}, // back face
        {0,4}, {1,5}, {2,6}, {3,7}  // connecting edges
    };

    for (int i = 0; i < 12; i++) {
        cube->edges[i].v0 = cube_edges[i][0];
        cube->edges[i].v1 = cube_edges[i][1];
        cube->edges[i].depth = 0.0f;
    }

    // Camera setup
    camera_t camera = {
        .position = {0, 0, -5},
        .target = {0, 0, 0},
        .up = {0, 1, 0},
        .fov = 60.0f * (M_PI / 180.0f),
        .aspect = aspect,
        .near = 0.1f,
        .far = 100.0f
    };

    // Viewport setup
    viewport_t viewport = {
        .center_x = WIDTH / 2,
        .center_y = HEIGHT / 2,
        .radius = HEIGHT / 2
    };

    // Lighting setup
    vec3_t lights[NUM_LIGHTS] = {
        {0.5f, 1.0f, -1.0f},  // Light from top-right
        {-0.5f, 0.5f, 0.5f}   // Light from top-left
    };

    float angle = 0.0f;
    int frame_count = 0;

    while (frame_count < 240) {  // 8 seconds at 30 FPS
        canvas_clear(canvas, 0.0f);
        angle += 0.01f;

        // Cube transformation (left side)
        mat4_t cube_model = mat4_identity();
        cube_model = mat4_multiply(cube_model, mat4_translation(vec3_from_cartesian(-1.5f, 0, 0)));
        cube_model = mat4_multiply(cube_model, mat4_rotate_xyz(angle, angle * 0.5f, angle * 0.3f));
        cube_model = mat4_multiply(cube_model, mat4_scale_uniform(1.2f));

        // Soccer ball transformation (right side)
        mat4_t ball_model = mat4_identity();
        ball_model = mat4_multiply(ball_model, mat4_translation(vec3_from_cartesian(1.5f, 0, 0)));
        ball_model = mat4_multiply(ball_model, mat4_rotate_xyz(angle * 0.7f, angle * 0.9f, angle * 0.5f));
        ball_model = mat4_multiply(ball_model, mat4_scale_uniform(1.0f));

        // Render cube with lighting
        for (int i = 0; i < cube->edge_count; i++) {
            vec3_t v0 = cube->vertices[cube->edges[i].v0];
            vec3_t v1 = cube->vertices[cube->edges[i].v1];

            vec3_t world_v0 = mat4_transform_point(cube_model, v0);
            vec3_t world_v1 = mat4_transform_point(cube_model, v1);
            vec3_t edge_dir = vec3_sub(world_v1, world_v0);

            float intensity = compute_lighting(edge_dir, lights, NUM_LIGHTS);

            vec3_t p0 = project_vertex(v0, cube_model,
                                     create_view_matrix(camera),
                                     create_projection_matrix(camera),
                                     viewport);
            vec3_t p1 = project_vertex(v1, cube_model,
                                     create_view_matrix(camera),
                                     create_projection_matrix(camera),
                                     viewport);

            draw_line_f(canvas, p0.x, p0.y, p1.x, p1.y, intensity * 2.0f);
        }

        // Render soccer ball with lighting
        for (int i = 0; i < soccer_ball->edge_count; i++) {
            vec3_t v0 = soccer_ball->vertices[soccer_ball->edges[i].v0];
            vec3_t v1 = soccer_ball->vertices[soccer_ball->edges[i].v1];

            vec3_t world_v0 = mat4_transform_point(ball_model, v0);
            vec3_t world_v1 = mat4_transform_point(ball_model, v1);
            vec3_t edge_dir = vec3_sub(world_v1, world_v0);

            float intensity = compute_lighting(edge_dir, lights, NUM_LIGHTS);

            vec3_t p0 = project_vertex(v0, ball_model,
                                     create_view_matrix(camera),
                                     create_projection_matrix(camera),
                                     viewport);
            vec3_t p1 = project_vertex(v1, ball_model,
                                     create_view_matrix(camera),
                                     create_projection_matrix(camera),
                                     viewport);

            draw_line_f(canvas, p0.x, p0.y, p1.x, p1.y, intensity * 2.0f);
        }

        // Save frame
        char filename[64];
        snprintf(filename, sizeof(filename), "visual_%04d.pgm", frame_count++);
        save_canvas_as_pgm(canvas, filename);
    }

    // Cleanup
    free_mesh(cube);
    free_mesh(soccer_ball);
    free_canvas(canvas);

    return 0;
}