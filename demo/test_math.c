#include "math3d.h"
#include "canvas.h"
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

// Helper to calculate z-depth for sorting
float get_avg_z(vec3_t a, vec3_t b) {
    return (a.z + b.z) * 0.5f;
}

void draw_cube(canvas_t* canvas, mat4_t mvp) {
    vec3_t cube[] = {
        // Front face
        vec3_from_cartesian(-0.5f, -0.5f,  0.5f),
        vec3_from_cartesian( 0.5f, -0.5f,  0.5f),
        vec3_from_cartesian( 0.5f,  0.5f,  0.5f),
        vec3_from_cartesian(-0.5f,  0.5f,  0.5f),
        // Back face
        vec3_from_cartesian(-0.5f, -0.5f, -0.5f),
        vec3_from_cartesian( 0.5f, -0.5f, -0.5f),
        vec3_from_cartesian( 0.5f,  0.5f, -0.5f),
        vec3_from_cartesian(-0.5f,  0.5f, -0.5f)
    };

    typedef struct {
        int a, b;
        float depth;
    } Edge;

    Edge edges[] = {
        // Front face (z = +0.5)
        {0,1, 0.5f}, {1,2, 0.5f}, {2,3, 0.5f}, {3,0, 0.5f},
        // Back face (z = -0.5)
        {4,5, -0.5f}, {5,6, -0.5f}, {6,7, -0.5f}, {7,4, -0.5f},
        // Connecting edges
        {0,4, 0.0f}, {1,5, 0.0f}, {2,6, 0.0f}, {3,7, 0.0f}
    };

    // Transform all vertices first
    vec3_t proj[8];
    for (int i = 0; i < 8; i++) {
        proj[i] = mat4_mul_vec3(mvp, cube[i]);
        proj[i].x = (proj[i].x + 1.0f) * 0.5f * WIDTH;
        proj[i].y = (1.0f - (proj[i].y + 1.0f) * 0.5f) * HEIGHT;
    }

    // Draw back faces first (dimmer)
    for (int i = 4; i < 8; i++) {
        draw_line_f(canvas, 
                   proj[edges[i].a].x, proj[edges[i].a].y,
                   proj[edges[i].b].x, proj[edges[i].b].y,
                   0.3f); // Gray for back faces
    }

    // Draw front faces last (brighter)
    for (int i = 0; i < 4; i++) {
        draw_line_f(canvas, 
                   proj[edges[i].a].x, proj[edges[i].a].y,
                   proj[edges[i].b].x, proj[edges[i].b].y,
                   1.0f); // White for front faces
    }

    // Draw connecting edges
    for (int i = 8; i < 12; i++) {
        draw_line_f(canvas, 
                   proj[edges[i].a].x, proj[edges[i].a].y,
                   proj[edges[i].b].x, proj[edges[i].b].y,
                   0.6f); // Medium gray for sides
    }
}

int main() {
    canvas_t* canvas = create_canvas(WIDTH, HEIGHT);
    
    // Time-based rotation
    clock_t start = clock();
    
    while (1) {  // Animation loop
        canvas_clear(canvas, 0.0f);
        
        // Calculate rotation based on time
        float time = (float)(clock() - start) / CLOCKS_PER_SEC;
        float angle = time * 0.5f;  // Rotate 0.5 radians per second
        
        mat4_t model = mat4_identity();
        model = mat4_translate(0.0f, 0.0f, -4.0f);
        model = mat4_rotate_xyz(angle, angle*0.5f, angle*0.3f);  // Rotate on all axes
        
        // Draw and save
        draw_cube(canvas, mat4_mul(proj, model));
        save_canvas_as_pgm(canvas, "cube.pgm");
        
        // Break after 10 seconds for this demo
        if (time > 10.0f) break;
    }
    
    free_canvas(canvas);
    return 0;
}