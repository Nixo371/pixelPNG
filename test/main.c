#include "pixelpng.h"
#include "pixel.h"

int main() {
	int width = 1000;
	int height = 1000;
	pixelPNG* png = initialize_png(width, height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int centered_x = x - (width / 2);
			int centered_y = y - (height / 2);
			if ((centered_x * centered_x) + (centered_y * centered_y) <= (400 * 400)) {
				set_pixel_grayscale(png, x, y, 0);
			}
			else {
				set_pixel_grayscale(png, x, y, 1);
			}
		}
	}
	
	generate_png(png, "circle.png", 1, GRAYSCALE, 0, 0, 0);
	free_png(png);
}
