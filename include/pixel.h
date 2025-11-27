#ifndef PIXEL_H
# define PIXEL_H

# include <stdint.h>

// Forward Declarations //
typedef struct s_pixelPNG pixelPNG;
// -------------------- //


// TODO perhaps worth optimizing since maximum size is 4 bytes, and we always have 6
// I suppose grayscale is identical to r==g==b so that can be removed, and the palette can be "overriden" by alpha
typedef struct s_pixel {
	uint8_t grayscale;	// 0-255
	uint8_t red;		// 0-255
	uint8_t green;		// 0-255
	uint8_t blue;		// 0-255
	uint8_t alpha;		// 0-255
	uint8_t palette_index;	// 0-255
} pixel;

// Get
pixel get_pixel(pixelPNG* pixelPNG, int x, int y);

// Set
void set_pixel(pixelPNG* pixelPNG, int x, int y, uint8_t grayscale, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, uint8_t palette_index);
void set_pixel_grayscale(pixelPNG* pixelPNG, int x, int y, uint8_t grayscale);
void set_pixel_grayscale_alpha(pixelPNG* pixelPNG, int x, int y, uint8_t grayscale, uint8_t alpha);
void set_pixel_rgb(pixelPNG* pixelPNG, int x, int y, uint8_t red, uint8_t green, uint8_t blue);
void set_pixel_rgba(pixelPNG* pixelPNG, int x, int y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

#endif
