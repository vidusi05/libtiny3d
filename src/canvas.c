#include "canvas.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

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
        canvas->pixels[i] = value;  // Direct float assignment
    }
}

void set_pixel_f(canvas_t* canvas, float x, float y, float intensity) {
    int x0 = (int)x;
    int y0 = (int)y;
    float fx = x - x0;
    float fy = y - y0;

    float w00 = (1 - fx) * (1 - fy);
    float w01 = (1 - fx) * fy;
    float w10 = fx * (1 - fy);
    float w11 = fx * fy;

    if (x0 >= 0 && x0 < canvas->width && y0 >= 0 && y0 < canvas->height)
        canvas->pixels[y0 * canvas->width + x0] += w00 * intensity;
    if (x0 >= 0 && x0 < canvas->width && (y0 + 1) < canvas->height)
        canvas->pixels[(y0 + 1) * canvas->width + x0] += w01 * intensity;
    if ((x0 + 1) < canvas->width && y0 >= 0 && y0 < canvas->height)
        canvas->pixels[y0 * canvas->width + (x0 + 1)] += w10 * intensity;
    if ((x0 + 1) < canvas->width && (y0 + 1) < canvas->height)
        canvas->pixels[(y0 + 1) * canvas->width + (x0 + 1)] += w11 * intensity;
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
            set_pixel_f(canvas, px, py, 1.0f);
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