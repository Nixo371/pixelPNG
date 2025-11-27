#include "pixelpng.h"

#include "pixel.h"
#include "chunk.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

pixelPNG* initialize_png(int width, int height) {
	pixelPNG* png = (pixelPNG *) malloc(sizeof(pixelPNG));
	
	// Create with default values since we don't care about them until we actually generate the PNG
	ChunkIHDR* ihdr = generate_ihdr_chunk(width, height, 1, GRAYSCALE, 0, 0, 0);
	png->ihdr_chunk = ihdr;

	// TODO calloc?
	pixel** pixels = (pixel **) malloc(width * sizeof(pixel *));
	for (int i = 0; i < width; i++) {
		pixels[i] = (pixel *) malloc(height * sizeof(pixel));
	}
	png->pixels = pixels;

	return (png);
}

void generate_png(pixelPNG* pixelPNG, char* file_name, unsigned char bit_depth, int color_type, unsigned char compression_method, unsigned char filter_method, unsigned char interlace_method) {
	FILE* file = fopen(file_name, "wb");
	if (file == NULL) {
		perror("fopen");
		fprintf(stderr, "Could not open/create the file '%s'\n", file_name);
		return;
	}

	// 137 80 78 71 13 10 26 10
	const char png_file_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
	size_t wrote = fwrite(png_file_signature, 1, 8, file);
	if (wrote != 8) {
		perror("fwrite signature");
		fclose(file);
		return;
	}

	pixelPNG->ihdr_chunk->bit_depth = bit_depth;
	pixelPNG->ihdr_chunk->color_type = color_type;
	pixelPNG->ihdr_chunk->compression_method = compression_method;
	pixelPNG->ihdr_chunk->filter_method = filter_method;
	pixelPNG->ihdr_chunk->interlace_method = interlace_method;
	write_ihdr_chunk(file, pixelPNG->ihdr_chunk);

	ChunkIDAT* idat_chunk = generate_idat_chunk(pixelPNG->pixels, pixelPNG->ihdr_chunk);
	write_idat_chunk(file, idat_chunk);
	// TODO Make this a function
	free(idat_chunk->data);
	free(idat_chunk);

	write_chunk(file, 0, IEND, NULL);

	fclose(file);
}

void free_png(pixelPNG* pixelPNG) {
	for (int i = 0; i < pixelPNG->ihdr_chunk->width; i++) {
		free(pixelPNG->pixels[i]);
	}
	free(pixelPNG->pixels);
	free(pixelPNG->ihdr_chunk);
	free(pixelPNG);
}
