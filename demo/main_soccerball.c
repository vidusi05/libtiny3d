#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"

#define PI 3.14159265358979323846f
#define CANVAS_SIZE 800
#define VIEWPORT_RADIUS 350.0f

int main() {
    canvas_t* canvas_clock = create_canvas(CANVAS_SIZE, CANVAS_SIZE);
    canvas_t* canvas_ball = create_canvas(CANVAS_SIZE, CANVAS_SIZE);
    if (!canvas_clock || !canvas_ball) {
        printf("Failed to create canvases\n");
        return 1;
    }

    canvas_clear(canvas_clock, 0.0f);
    draw_clock_lines(canvas_clock, VIEWPORT_RADIUS, 2.0f);
    save_canvas_as_pgm(canvas_clock, "clock_face_static.pgm");

    mesh_t* soccer_ball = generate_soccer_ball();
    if (!soccer_ball) {
        printf("Failed to generate soccer ball\n");
        return 1;
    }

    camera_t camera = {
        .position = vec3_from_cartesian(0.0f, 0.0f, 4.0f),
        .target = vec3_from_cartesian(0.0f, 0.0f, 0.0f),
        .up = vec3_from_cartesian(0.0f, 1.0f, 0.0f),
        .fov = PI * 0.25f,
        .aspect = 1.0f,
        .near = 0.1f,
        .far = 100.0f
    };

    viewport_t viewport = {
        .center_x = CANVAS_SIZE * 0.5f,
        .center_y = CANVAS_SIZE * 0.5f,
        .radius = VIEWPORT_RADIUS,
        .screen_width = CANVAS_SIZE,
        .screen_height = CANVAS_SIZE
    };

    int num_frames = 120;
    float rotation_speed = 2.0f * PI / num_frames;

    printf("Generating %d frames of rotating soccer ball...\n", num_frames);

    for (int frame = 0; frame < num_frames; frame++) {
        canvas_clear(canvas_ball, 0.0f);

        float angle_y = frame * rotation_speed;
        float angle_x = frame * rotation_speed * 0.3f;
        float angle_z = frame * rotation_speed * 0.1f;

        mat4_t rotation_matrix = mat4_rotate_xyz(angle_x, angle_y, angle_z);
        mat4_t scale_matrix = mat4_scale(1.2f, 1.2f, 1.2f);
        mat4_t model_matrix = mat4_mul(rotation_matrix, scale_matrix);

        render_wireframe(canvas_ball, soccer_ball, model_matrix, camera, viewport);

        char filename[256];
        snprintf(filename, sizeof(filename), "soccer_ball_frame_%03d.pgm", frame);
        save_canvas_as_pgm(canvas_ball, filename);

        if (frame % 10 == 0) {
            printf("Generated frame %d/%d\n", frame + 1, num_frames);
        }
    }

    printf("Animation complete! Frames saved as soccer_ball_frame_XXX.pgm\n");

    canvas_clear(canvas_ball, 0.0f);
    mat4_t demo_rotation = mat4_rotate_xyz(0.3f, 0.8f, 0.1f);
    mat4_t demo_scale = mat4_scale(1.2f, 1.2f, 1.2f);
    mat4_t demo_model = mat4_mul(demo_rotation, demo_scale);
    render_wireframe(canvas_ball, soccer_ball, demo_model, camera, viewport);
    save_canvas_as_pgm(canvas_ball, "soccer_ball_demo.pgm");

    printf("Demo frame saved as soccer_ball_demo.pgm\n");

    free_mesh(soccer_ball);
    free_canvas(canvas_clock);
    free_canvas(canvas_ball);

    return 0;
}