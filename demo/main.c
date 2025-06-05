#include "canvas.h"
#include <math.h>

#define PI 3.14159265

int main() {
    int width = 800;
    int height = 600;
    canvas_t* canvas = create_canvas(width, height);
    canvas_clear(canvas, 0.0f);
    
    // Center coordinates
    float center_x = width / 2.0f;
    float center_y = height / 2.0f;
    
    // Length of the lines (you can adjust this)
    float length = 300.0f;
    
    // Draw lines at 15-degree intervals (0°, 15°, 30°, ..., 345°)
    for (int angle = 0; angle < 360; angle += 15) {
        // Convert angle to radians
        float radians = angle * PI / 180.0f;
        
        // Calculate endpoint coordinates
        float end_x = center_x + length * cos(radians);
        float end_y = center_y + length * sin(radians);
        
        // Draw line from center to endpoint
        draw_line_f(canvas, center_x, center_y, end_x, end_y, 2.0f);
    }
    
    save_canvas_as_pgm(canvas, "output.pgm");
    free_canvas(canvas);
    return 0;
}