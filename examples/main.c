#include "pixelpng.h"
#include "pixel.h"
#include "chunk.h"

#include <stdint.h>
#include <stdio.h>

void draw_background(pixelPNG* png, int width, int height, uint8_t grayscale_value) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			set_pixel_grayscale(png, x, y, grayscale_value);
			set_pixel_rgb(png, x, y, grayscale_value, grayscale_value, grayscale_value);
		}
	}
}

void draw_circle(pixelPNG* png, int x, int y, int radius) {
	int width = png->ihdr_chunk->width;
	int height = png->ihdr_chunk->height;

	for (int _y = y - radius; _y < y + radius; _y++) {
		for (int _x = x - radius; _x < x + radius; _x++) {
			int centered_x = _x - x;
			int centered_y = _y - y;
			int distance_from_center = (centered_x * centered_x) + (centered_y * centered_y);
			if (distance_from_center <= (radius * radius)) {
				set_pixel_grayscale(png, _x, _y, 1);

				uint8_t red = (distance_from_center * 255) / (radius * radius);
				uint8_t green = (_x * 255) / width;
				uint8_t blue = (_y * 255) / height;
				set_pixel_rgb(png, _x, _y, red, green, blue);
			}
		}
	}
}

void draw_rectangle(pixelPNG* png, int start_x, int start_y, int end_x, int end_y, uint8_t r, uint8_t g, uint8_t b) {
	for (int y = start_y; y < end_y; y++) {
		for (int x = start_x; x < end_x; x++) {
			set_pixel_rgb(png, x, y, r, g, b);
		}
	}
}

uint8_t scale_grayscale(uint8_t grayscale, uint8_t bit_depth) {
	float grayscale_float = (float)grayscale / 255;
	uint8_t scaling_factor = (1 << bit_depth) - 1;

	uint8_t scaled_grayscale = (uint8_t)(grayscale_float * scaling_factor);

	return (scaled_grayscale);
}

// grayscale is always 0-255, bit_depth scales it if necessary
// scaling is done by rounding to the nearest value
// grayscale 128 with bit_depth 1 would scale down to 1, whereas 127 would scale down to 0
void draw_rectangle_grayscale(pixelPNG* png, int start_x, int start_y, int end_x, int end_y, uint8_t grayscale, uint8_t bit_depth) {
	uint8_t scaled_grayscale = scale_grayscale(grayscale, bit_depth);

	for (int y = start_y; y < end_y; y++) {
		for (int x = start_x; x < end_x; x++) {
			set_pixel_grayscale(png, x, y, scaled_grayscale);
		}
	}
}

void example1() {
	int width = 1000;
	int height = 1000;
	pixelPNG* png = initialize_png(width, height);

	draw_background(png, width, height, 0);
	draw_circle(png, width / 2, height / 2, width / 3);

	generate_png(png, "examples/circle_grayscale.png", 1, GRAYSCALE, 0, 0, 0);
	generate_png(png, "examples/circle_grayscale2.png", 2, GRAYSCALE, 0, 0, 0);
	generate_png(png, "examples/circle_grayscale4.png", 4, GRAYSCALE, 0, 0, 0);
	generate_png(png, "examples/circle_grayscale8.png", 8, GRAYSCALE, 0, 0, 0);
	generate_png(png, "examples/circle_rgb.png", 8, RGB, 0, 0, 0);

	free_png(png);
}

void example2() {
	int width = 10;
	int height = 10;
	pixelPNG* png = initialize_png(width, height);

	draw_background(png, width, height, 0);
	draw_rectangle(png, 0, 0, width, height, 255, 0, 0);
	generate_png(png, "examples/rectangle_rgb.png", 8, RGB, 0, 0, 0);

	draw_background(png, width, height, 0);
	draw_rectangle_grayscale(png, 0, 0, width, height, 255, 1);
	generate_png(png, "examples/rectangle_grayscale.png", 1, GRAYSCALE, 0, 0, 0);

	draw_background(png, width, height, 0);
	draw_rectangle_grayscale(png, 0, 0, width, height, 255, 2);
	generate_png(png, "examples/rectangle_grayscale2.png", 2, GRAYSCALE, 0, 0, 0);

	draw_background(png, width, height, 0);
	draw_rectangle_grayscale(png, 0, 0, width, height, 255, 4);
	generate_png(png, "examples/rectangle_grayscale4.png", 4, GRAYSCALE, 0, 0, 0);

	draw_background(png, width, height, 0);
	draw_rectangle_grayscale(png, 0, 0, width, height, 255, 8);
	generate_png(png, "examples/rectangle_grayscale8.png", 8, GRAYSCALE, 0, 0, 0);

	free_png(png);
}

int main() {
	// Generate a blank PNG with a black background and using an auxilliary function (draw_circle)
	// draw a circle in a fun "rainbow" color
	// Then output 2 final results, one as a 1-bit grayscale image, and the other as an 8-bit rgb image
	example1();
	example2();
}
