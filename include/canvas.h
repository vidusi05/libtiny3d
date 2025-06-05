#ifndef CANVAS_H
#define CANVAS_H

typedef struct {
    int width;
    int height;
    float* pixels;  // Single pointer to float array
} canvas_t;

// Use consistent naming (either all with canvas_ prefix or none)
canvas_t* create_canvas(int width, int height);
void free_canvas(canvas_t* canvas);
void canvas_clear(canvas_t* canvas, float value);
void set_pixel_f(canvas_t* canvas, float x, float y, float intensity);
void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness);
void save_canvas_as_pgm(canvas_t* canvas, const char* filename);

#endif