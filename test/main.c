#include "pixelpng.h"

int main() {
	int width = 100;
	int height = 100;
	pixelPNG* png = initialize_png(width, height, 1, GRAYSCALE, 0, 0, 0);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int centered_x = x - (width / 2);
			int centered_y = y - (height / 2);
			if ((centered_x * centered_x) + (centered_y * centered_y) <= (40 * 40)) {
				png->pixels[x][y].grayscale = 0;
			}
			else {
				png->pixels[x][y].grayscale = 1;
			}
		}
	}
	
	generate_png(png, "circle.png");
}
