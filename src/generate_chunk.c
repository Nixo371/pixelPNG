#include "chunk.h"

#include "pixel.h"
#include "pixelpng.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
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

uint8_t* get_grayscale_pixel_bytes(pixel** pixels, int x, int y, ChunkIHDR* ihdr_chunk) {
	// Max of 2 bytes for 16 bit depth
	uint8_t* bytes = calloc(2, sizeof(uint8_t));

	int pixels_per_byte = 8 / ihdr_chunk->bit_depth;
	// TODO if pixels_per_byte == 0 (8 / 16)

	for (int i = 0; i < pixels_per_byte; i++) {
		bytes[0] = bytes[0] << ihdr_chunk->bit_depth;
		// Assume the values are correctly bounded
		// Undefined behavior otherwise
		bytes[0] += pixels[x][y].grayscale;

		x += 1;
		if (x >= ihdr_chunk->width) {
			bytes[0] <<= (((pixels_per_byte - 1) - i) * ihdr_chunk->bit_depth);
			break;
		}
	}

	return (bytes);
}

uint8_t* get_rgb_pixel_bytes(pixel** pixels, int x, int y, ChunkIHDR* ihdr_chunk) {
	// Max of 6 bytes for 16 bit depth (2 for r, g, and b)
	uint8_t* bytes = calloc(6, sizeof(uint8_t));

	// TODO implement 16 bit depth
	// int bytes_per_pixel = ihdr_chunk->bit_depth / 8;
	
	// This is so the compiler won't complain since I haven't implemented 16 bits yet
	// ffs I'll also have to update the pixel struct to support 16-bits per color
	if (ihdr_chunk->color_type != RGB) {
		printf("actually how the fuck\n");
		exit(EXIT_FAILURE);
	}

	bytes[0] = pixels[x][y].red;
	bytes[1] = pixels[x][y].green;
	bytes[2] = pixels[x][y].blue;

	return (bytes);
}

uint8_t* get_grayscale_alpha_pixel_bytes(pixel** pixels, int x, int y, ChunkIHDR* ihdr_chunk) {
	// Max of 4 bytes for 16 bit depth
	uint8_t* bytes = calloc(4, sizeof(uint8_t));

	// This is so the compiler won't complain since I haven't implemented 16 bits yet
	if (ihdr_chunk->color_type != GRAYSCALE_ALPHA) {
		printf("actually how the fuck\n");
		exit(EXIT_FAILURE);
	}

	bytes[0] = pixels[x][y].grayscale;
	bytes[1] = pixels[x][y].alpha;

	return (bytes);
}

uint8_t* get_rgba_pixel_bytes(pixel** pixels, int x, int y, ChunkIHDR* ihdr_chunk) {
	// Max of 8 bytes for 16 bit depth (2 for r, g, b, and a)
	uint8_t* bytes = calloc(8, sizeof(uint8_t));

	// TODO implement 16 bit depth
	// int bytes_per_pixel = ihdr_chunk->bit_depth / 8;
	
	// This is so the compiler won't complain since I haven't implemented 16 bits yet
	if (ihdr_chunk->color_type != RGBA) {
		printf("actually how the fuck\n");
		exit(EXIT_FAILURE);
	}

	bytes[0] = pixels[x][y].red;
	bytes[1] = pixels[x][y].green;
	bytes[2] = pixels[x][y].blue;
	bytes[3] = pixels[x][y].alpha;

	return (bytes);
}

uint8_t* get_pixel_bytes(pixel** pixels, int x, int y, ChunkIHDR* ihdr_chunk) {
	uint8_t* bytes;
	switch (ihdr_chunk->color_type) {
		case GRAYSCALE:
			bytes = get_grayscale_pixel_bytes(pixels, x, y, ihdr_chunk);
			break;
		case RGB:
			bytes = get_rgb_pixel_bytes(pixels, x, y, ihdr_chunk);
			break;
		case PALETTE:
			// Not implemented
			return (0);
			break;
		case GRAYSCALE_ALPHA:
			bytes = get_grayscale_alpha_pixel_bytes(pixels, x, y, ihdr_chunk);
			break;
		case RGBA:
			bytes = get_rgba_pixel_bytes(pixels, x, y, ihdr_chunk);
			break;
	}

	return (bytes);
}

ChunkIDAT* generate_idat_chunk(pixel** pixels, ChunkIHDR* ihdr_chunk) {
	ChunkIDAT* idat_chunk = (ChunkIDAT *) malloc(sizeof(ChunkIDAT));

	int width = ihdr_chunk->width;
	int height = ihdr_chunk->height;

	size_t pixels_per_byte = 1;
	int bytes_per_pixel;

	switch (ihdr_chunk->color_type) {
		case GRAYSCALE:
			bytes_per_pixel = ihdr_chunk->bit_depth / 8;
			if (bytes_per_pixel == 0) {
				bytes_per_pixel = 1;
			}

			pixels_per_byte = 8 / ihdr_chunk->bit_depth;
			break;
		case RGB:
			bytes_per_pixel = (ihdr_chunk->bit_depth / 8) * 3;
			break;
		case PALETTE:
			// TODO
			bytes_per_pixel = 1;
			break;
		case GRAYSCALE_ALPHA:
			bytes_per_pixel = (ihdr_chunk->bit_depth / 8) * 2;
			break;
		case RGBA:
			bytes_per_pixel = (ihdr_chunk->bit_depth / 8) * 4;
			break;
	}

	idat_chunk->length = 0;
	// Each scan line's first byte is the filter byte, so we reserve maximum possible size (excludint 16 bit size)
	size_t data_size = ((width / pixels_per_byte) + 1) * height * bytes_per_pixel;
	// If 1-7 extra bits, we need an extra byte
	if (width % pixels_per_byte != 0) {
		data_size += height;
	}
	idat_chunk->data = (uint8_t *) calloc(data_size, sizeof(uint8_t));

	int x = 0;
	int y = 0;
	while (y < height) {
		// TODO see if other filter modes are available
		// First byte of each row reserved for filter data
		idat_chunk->data[idat_chunk->length] = 0;
		idat_chunk->length += 1;
		while (x < width) {
			// TODO special case for bit_depth == 16?
			uint8_t* next_bytes = get_pixel_bytes(pixels, x, y, ihdr_chunk);
			for (int i = 0; i < bytes_per_pixel; i++) {
				idat_chunk->data[idat_chunk->length] = next_bytes[i];
				idat_chunk->length += 1;
			}
			x += pixels_per_byte;

			free(next_bytes);
		}
		x = 0;
		y++;
	}

	return (idat_chunk);
}
