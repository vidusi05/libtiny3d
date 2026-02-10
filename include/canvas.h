#ifndef CANVAS_H
#define CANVAS_H

typedef struct {
    int width;
    int height;
    float* pixels;
} canvas_t;

canvas_t* create_canvas(int width, int height);
void canvas_clear(canvas_t* canvas, float value);
void set_pixel_f(canvas_t* canvas, int x, int y, float intensity);
void save_canvas_as_pgm(canvas_t* canvas, const char* filename);
void draw_clock_lines(canvas_t* canvas, float radius, float thickness);
void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness);
void free_canvas(canvas_t* canvas);  // ← Add this line


#endif // CANVAS_H


