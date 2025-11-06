#include "pixelpng.h"

#include <stddef.h>
#include <stdlib.h>

ChunkIHDR* generate_ihdr_chunk(int width, int height, unsigned char bit_depth, int color_type, unsigned char compression_method, unsigned char filter_method, unsigned char interlace_method) {
	ChunkIHDR* ihdr_chunk = (ChunkIHDR *) malloc(sizeof(ChunkIHDR));

	ihdr_chunk->width = width;
	ihdr_chunk->height = height;
	ihdr_chunk->bit_depth = bit_depth;
	ihdr_chunk->color_type = color_type;
	ihdr_chunk->compression_method = compression_method;
	ihdr_chunk->filter_method = filter_method;
	ihdr_chunk->interlace_method = interlace_method;

	return (ihdr_chunk);
}

// I think I have to assume that it's 8 or less bit_depth
// Undefined behavior for bit_depth == 16
unsigned char get_grayscale_pixel_byte(pixel** pixels, int x, int y, ChunkIHDR* ihdr_chunk) {
	unsigned char byte = 0;

	int pixels_per_byte = 8 / ihdr_chunk->bit_depth;

	for (int i = 0; i < pixels_per_byte; i++) {
		byte = byte << ihdr_chunk->bit_depth;
		// Assume the values are correctly bounded
		// Undefined behavior otherwise
		byte += pixels[x][y].grayscale;

		x++;
		if (x >= ihdr_chunk->width) {
			byte = byte << ((pixels_per_byte - 1) - i);
			break;
		}
	}

	return (byte);
}

unsigned char get_pixel_byte(pixel** pixels, int x, int y, ChunkIHDR* ihdr_chunk) {
	unsigned char byte;
	switch (ihdr_chunk->color_type) {
		case GRAYSCALE:
			// TODO check that it's not bit_depth 16?
			byte = get_grayscale_pixel_byte(pixels, x, y, ihdr_chunk);
			break;
		case RGB:
			// Not implemented
			return (0);
			break;
		case PALETTE:
			// Not implemented
			return (0);
			break;
		case GRAYSCALE_ALPHA:
			// Not implemented
			return (0);
			break;
		case RGBA:
			// Not implemented
			return (0);
			break;
	}

	return (byte);
}

ChunkIDAT* generate_idat_chunk(pixel** pixels, ChunkIHDR* ihdr_chunk) {
	ChunkIDAT* idat_chunk = (ChunkIDAT *) malloc(sizeof(ChunkIDAT));

	int width = ihdr_chunk->width;
	int height = ihdr_chunk->height;

	size_t pixels_per_byte = 8 / ihdr_chunk->bit_depth;
	
	idat_chunk->length = 0;
	// Each scan line's first byte is the filter byte, so we reserve maximum possible size (excludint 16 bit size)
	size_t data_size = ((width / pixels_per_byte) + 1) * height;
	// If 1-7 extra bits, we need an extra byte
	if (width % pixels_per_byte != 0) {
		data_size += height;
	}
	idat_chunk->data = (unsigned char *) malloc(data_size * sizeof(unsigned char));

	int y = 0;
	int x = 0;
	while (y < height) {
		// TODO see if other filter modes are available
		idat_chunk->data[idat_chunk->length] = 0;
		idat_chunk->length++;
		while (x < width) {
			// TODO special case for bit_depth == 16?
			unsigned char next_byte = get_pixel_byte(pixels, x, y, ihdr_chunk);
			
			idat_chunk->data[idat_chunk->length] = next_byte;
			idat_chunk->length++;
			x += pixels_per_byte;
		}
		x = 0;
		y++;
	}

	return (idat_chunk);
}
