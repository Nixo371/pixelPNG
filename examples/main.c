#include "pixelpng.h"
#include "pixel.h"
#include "chunk.h"

#include <stdint.h>

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

void example1() {
	int width = 1000;
	int height = 1000;
	pixelPNG* png = initialize_png(width, height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			set_pixel_grayscale(png, x, y, 0);
			set_pixel_rgb(png, x, y, 0, 0, 0);
		}
	}
	
	draw_circle(png, width / 2, height / 2, width / 3);

	generate_png(png, "examples/circle_grayscale.png", 1, GRAYSCALE, 0, 0, 0);
	generate_png(png, "examples/circle_grayscale2.png", 2, GRAYSCALE, 0, 0, 0);
	generate_png(png, "examples/circle_grayscale4.png", 4, GRAYSCALE, 0, 0, 0);
	generate_png(png, "examples/circle_grayscale8.png", 8, GRAYSCALE, 0, 0, 0);
	generate_png(png, "examples/circle_rgb.png", 8, RGB, 0, 0, 0);

	free_png(png);
}

int main() {
	// Generate a blank PNG with a black background and using an auxilliary function (draw_circle)
	// draw a circle in a fun "rainbow" color
	// Then output 2 final results, one as a 1-bit grayscale image, and the other as an 8-bit rgb image
	example1();
}
