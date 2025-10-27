#include "pixelpng.h"

#include <stdio.h>
#include <stdlib.h>

pixelPNG* initialize_png(int width, int height, unsigned char bit_depth, int color_type, unsigned char compression_method, unsigned char filter_method, unsigned char interlace_method) {
	pixelPNG* png = (pixelPNG *) malloc(sizeof(pixelPNG));
	
	ChunkIHDR* ihdr = generate_ihdr_chunk(width, height, bit_depth, color_type, compression_method, filter_method, interlace_method);
	png->ihdr_chunk = ihdr;

	pixel** pixels = (pixel **) malloc(width * sizeof(pixel *));
	for (int i = 0; i < width; i++) {
		pixels[i] = (pixel *) malloc(height * sizeof(pixel));
	}
	png->pixels = pixels;

	return (png);
}

void generate_png(pixelPNG* pixelPNG, char* file_name) {
	FILE* file = fopen(file_name, "w");

	// 137 80 78 71 13 10 26 10
	char png_file_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
	fwrite(png_file_signature, 1, 8, file);

	write_ihdr_chunk(file, pixelPNG->ihdr_chunk);

	ChunkIDAT* idat_chunk = generate_idat_chunk(pixelPNG->pixels, pixelPNG->ihdr_chunk);
	write_idat_chunk(file, idat_chunk);

	write_chunk(file, 0, IEND, NULL);

	fclose(file);
}
