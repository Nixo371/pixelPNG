#include "pixelpng.h"
#include "pixel.h"

#include <stdint.h>

int main() {
	int width = 1000;
	int height = 1000;
	pixelPNG* png = initialize_png(width, height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int centered_x = x - (width / 2);
			int centered_y = y - (height / 2);
			int distance_from_center = (centered_x * centered_x) + (centered_y * centered_y);
			if (distance_from_center <= (400 * 400)) {
				set_pixel_grayscale(png, x, y, 0);
				uint8_t red = (distance_from_center * 255) / (400 * 400);
				uint8_t green = (x * 255) / width;
				uint8_t blue = (y * 255) / height;
				set_pixel_rgb(png, x, y, red, green, blue);
			}
			else {
				set_pixel_grayscale(png, x, y, 1);
				set_pixel_rgb(png, x, y, 255, 255, 255);
			}
		}
	}
	
	generate_png(png, "circle1bd.png", 1, GRAYSCALE, 0, 0, 0);
	generate_png(png, "circle2bd.png", 2, GRAYSCALE, 0, 0, 0);
	generate_png(png, "circle4bd.png", 4, GRAYSCALE, 0, 0, 0);
	generate_png(png, "circle8bd.png", 8, GRAYSCALE, 0, 0, 0);

	generate_png(png, "circlergb.png", 8, RGB, 0, 0, 0);

	free_png(png);
}
