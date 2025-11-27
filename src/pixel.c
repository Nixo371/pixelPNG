#include "pixel.h"
#include "pixelpng.h"

int clamp(int value, int min, int max) {
	if (value < min) {
		return (min);
	}
	if (value > max) {
		return (max);
	}

	return (value);
}

pixel get_pixel(pixelPNG* pixelPNG, int x, int y) {
	return (pixelPNG->pixels[x][y]);
}

void set_pixel(pixelPNG* pixelPNG, int x, int y, uint8_t grayscale, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, uint8_t palette_index) {
	pixelPNG->pixels[x][y].grayscale = grayscale;
	pixelPNG->pixels[x][y].red = red;
	pixelPNG->pixels[x][y].green = green;
	pixelPNG->pixels[x][y].blue = blue;
	pixelPNG->pixels[x][y].alpha = alpha;
	pixelPNG->pixels[x][y].palette_index = palette_index;
}

void set_pixel_grayscale(pixelPNG* pixelPNG, int x, int y, uint8_t grayscale) {
	grayscale = clamp(grayscale, 0, 255);

	set_pixel(pixelPNG, x, y, grayscale, grayscale, grayscale, grayscale, 255, 0);
}

void set_pixel_grayscale_alpha(pixelPNG* pixelPNG, int x, int y, uint8_t grayscale, uint8_t alpha) {
	grayscale = clamp(grayscale, 0, 255);
	alpha = clamp(alpha, 0, 255);

	set_pixel(pixelPNG, x, y, grayscale, grayscale, grayscale, grayscale, alpha, 0);
}

void set_pixel_rgb(pixelPNG* pixelPNG, int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
	red = clamp(red, 0, 255);
	green = clamp(green, 0, 255);
	blue = clamp(blue, 0, 255);
	uint8_t grayscale = (red + green + blue) / 3;

	set_pixel(pixelPNG, x, y, grayscale, red, green, blue, 255, 0);
}

void set_pixel_rgba(pixelPNG* pixelPNG, int x, int y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	red = clamp(red, 0, 255);
	green = clamp(green, 0, 255);
	blue = clamp(blue, 0, 255);
	alpha = clamp(alpha, 0, 255);
	uint8_t grayscale = (red + green + blue) / 3;

	set_pixel(pixelPNG, x, y, grayscale, red, green, blue, alpha, 0);
}
