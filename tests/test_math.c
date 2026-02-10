#include "math3d.h"
#include "canvas.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define WIDTH 800
#define HEIGHT 600

void draw_cube(canvas_t* canvas, mat4_t mvp) {
    // Cube vertices (perfect unit cube)
    vec3_t cube[] = {
        {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}
    };

    // Cube edges (12 edges total)
    int edges[12][2] = {
        {0,1}, {1,2}, {2,3}, {3,0}, // Front face
        {4,5}, {5,6}, {6,7}, {7,4}, // Back face
        {0,4}, {1,5}, {2,6}, {3,7}  // Connecting edges
    };

    // Project all vertices
    vec3_t proj[8];
    for (int i = 0; i < 8; i++) {
        proj[i] = mat4_mul_vec3(mvp, cube[i]);
        // Convert to screen coordinates
        proj[i].x = (proj[i].x + 1.0f) * 0.5f * WIDTH;
        proj[i].y = (1.0f - (proj[i].y + 1.0f) * 0.5f) * HEIGHT;
    }

    // Draw all edges
    for (int i = 0; i < 12; i++) {
        float brightness = (i < 4) ? 1.0f : (i < 8) ? 0.3f : 0.6f;
        draw_line_f(canvas,
                   proj[edges[i][0]].x, proj[edges[i][0]].y,
                   proj[edges[i][1]].x, proj[edges[i][1]].y,
                   brightness);
    }
}

int main() {
    canvas_t* canvas = create_canvas(WIDTH, HEIGHT);
    if (!canvas) return 1;

    float aspect = (float)WIDTH / (float)HEIGHT;
    float fov = 60.0f * (M_PI / 180.0f);
    float near = 0.1f;
    float far = 100.0f;

    // Create perspective projection matrix
    mat4_t proj = mat4_perspective(fov, aspect, near, far);

    float duration = 5.0f;
    float fps = 24.0f;
    int max_frames = (int)(duration * fps);

    for (int frame = 0; frame < max_frames; frame++) {
        canvas_clear(canvas, 0.0f);

        float time = (float)frame / fps;
        float angle = time * 2.0f;  // Rotation speed

        // Model transformations - KEY CHANGES HERE:
        mat4_t scale = mat4_scale_uniform(0.5f);  // Reduced scale to 0.5 (50% size)
        mat4_t rot = mat4_rotate_xyz(angle, angle * 0.5f, angle * 0.3f);
        mat4_t trans = mat4_translate(0.0f, 0.0f, -2.5f);  // Adjusted distance

        // Model matrix: translate * rotate * scale
        mat4_t model = mat4_mul(trans, mat4_mul(rot, scale));

        // MVP matrix
        mat4_t mvp = mat4_mul(proj, model);

        draw_cube(canvas, mvp);

        char filename[64];
        snprintf(filename, sizeof(filename), "frame_%04d.pgm", frame);
        save_canvas_as_pgm(canvas, filename);
    }

    free_canvas(canvas);
    return 0;
}