#include "pixelpng.h"

int main() {
	pixelPNG* png = initialize_png(2, 2, 1, GRAYSCALE, 0, 0, 0);
	
	png->pixels[0][0].grayscale = 0;
	png->pixels[0][1].grayscale = 0;
	png->pixels[1][0].grayscale = 0;
	png->pixels[1][1].grayscale = 0;

	generate_png(png, "test.png");
}
