#include "canvas.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "math3d.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

canvas_t* create_canvas(int width, int height) {
    canvas_t* canvas = (canvas_t*)malloc(sizeof(canvas_t));
    canvas->width = width;
    canvas->height = height;
    canvas->pixels = (float*)calloc(width * height, sizeof(float));
    return canvas;
}

void free_canvas(canvas_t* canvas) {
    if (canvas) {
        free(canvas->pixels);
        free(canvas);
    }
}

void canvas_clear(canvas_t* canvas, float value) {
    for (int i = 0; i < canvas->width * canvas->height; i++) {
        canvas->pixels[i] = value;
    }
}

void set_pixel_f(canvas_t* canvas, int x, int y, float intensity) {
    // Simplified: just set pixel if within bounds
    if (x >= 0 && x < canvas->width && y >= 0 && y < canvas->height)
        canvas->pixels[y * canvas->width + x] += intensity;
}

void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float steps = fmaxf(fabsf(dx), fabsf(dy));

    float xinc = dx / steps;
    float yinc = dy / steps;

    float x = x0, y = y0;
    for (int i = 0; i <= steps; i++) {
        for (float a = 0; a < 2 * M_PI; a += 0.2f) {
            float px = x + thickness * cosf(a);
            float py = y + thickness * sinf(a);
            set_pixel_f(canvas, (int)px, (int)py, 1.0f);  // Cast to int as set_pixel_f expects ints
        }
        x += xinc;
        y += yinc;
    }
}

void save_canvas_as_pgm(canvas_t* canvas, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) return;

    fprintf(fp, "P5\n%d %d\n255\n", canvas->width, canvas->height);

    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            float val = canvas->pixels[y * canvas->width + x];
            unsigned char c = (unsigned char)(fminf(1.0f, val) * 255);
            fputc(c, fp);
        }
    }

    fclose(fp);
}

void draw_clock_lines(canvas_t* canvas, float radius, float thickness) {
    float cx = canvas->width / 2.0f;
    float cy = canvas->height / 2.0f;

    for (int i = 0; i < 24; i++) {
        float angle = i * (M_PI / 12.0f);
        float x1 = cx + radius * cosf(angle);
        float y1 = cy + radius * sinf(angle);
        draw_line_f(canvas, cx, cy, x1, y1, thickness);
    }
}
