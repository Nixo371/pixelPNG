#ifndef PIXELPNG_H
# define PIXELPNG_H

// https://www.libpng.org/pub/png/spec/1.2/PNG-Chunks.html

enum color_type {
	GRAYSCALE = 0,
	RGB = 2,
	PALETTE = 3,
	GRAYSCALE_ALPHA = 4,
	RGBA = 6
};

// TODO perhaps worth optimizing since maximum size is 4 bytes, and we always have 6
// I suppose grayscale is identical to r==g==b so that can be removed, and the palette can be "overriden" by alpha
typedef struct {
	unsigned char grayscale;	// 0-255
	unsigned char red;		// 0-255
	unsigned char green;		// 0-255
	unsigned char blue;		// 0-255
	unsigned char alpha;		// 0-255
	unsigned char palette_index;	// 0-255
} pixel;

typedef struct {
	int width;				// pixels
	int height;				// pixels
	unsigned char bit_depth;		// Valid values are 1, 2, 4, 8, and 16
	unsigned char color_type;		// Valid values are 0, 2, 3, 4, and 6
	unsigned char compression_method;	// Only 0 is valid for now
	unsigned char filter_method;		// Only 0 is valid for now
	unsigned char interlace_method;		// 0 (no interlace) or 1 (Adam7 interlace)
} IHDR;

typedef struct {
	
} IDAT;

typedef struct {
	
} pixelPNG;

#endif
