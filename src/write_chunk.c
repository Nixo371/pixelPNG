#include "chunk.h"

#include "pixelpng.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <zconf.h>
#include <zlib.h>

void write_chunk(FILE *file, size_t length, int chunk_type, unsigned char *chunk_data) {
	if (chunk_data == NULL && length > 0) {
		fprintf(stderr, "write_chunk: length > 0 when chunk_data == NULL");
		return;
	}

	// Length
	unsigned char length_bytes[4];
	length_bytes[0] = (length >> 24) & 0xFF;
	length_bytes[1] = (length >> 16) & 0xFF;
	length_bytes[2] = (length >> 8) & 0xFF;
	length_bytes[3] = (length) & 0xFF;
	fwrite(length_bytes, 1, 4, file);

	// Chunk Type
	char chunk_type_str[4];
	switch (chunk_type) {
		case IHDR:
			chunk_type_str[0] = 'I';
			chunk_type_str[1] = 'H';
			chunk_type_str[2] = 'D';
			chunk_type_str[3] = 'R';
			break;
		case IDAT:
			chunk_type_str[0] = 'I';
			chunk_type_str[1] = 'D';
			chunk_type_str[2] = 'A';
			chunk_type_str[3] = 'T';
			break;
		case IEND:
			chunk_type_str[0] = 'I';
			chunk_type_str[1] = 'E';
			chunk_type_str[2] = 'N';
			chunk_type_str[3] = 'D';
			break;
	}
	fwrite(chunk_type_str, 1, 4, file);

	// Chunk Data
	if (chunk_data != NULL) {
		fwrite(chunk_data, 1, length, file);
	}

	// CRC
	unsigned char* full_data = (unsigned char *) malloc((length + 4) * sizeof(unsigned char));
	size_t index = 0;
	for (int i = 0; i < 4; i++) {
		full_data[index] = chunk_type_str[i];
		index++;
	}
	for (size_t i = 0; i < length; i++) {
		if (chunk_data == NULL) {
			break;
		}

		full_data[index] = chunk_data[i];
		index++;
	}

	unsigned long crc_data = crc(full_data, length + 4);

	unsigned char crc_bytes[4];
	crc_bytes[0] = (crc_data >> 24) & 0xFF;
	crc_bytes[1] = (crc_data >> 16) & 0xFF;
	crc_bytes[2] = (crc_data >> 8) & 0xFF;
	crc_bytes[3] = (crc_data) & 0xFF;
	fwrite(crc_bytes, 1, 4, file);

	free(full_data);
}

void write_ihdr_chunk(FILE *file, ChunkIHDR *ihdr_chunk) {
	size_t length = IHDR_CHUNK_LENGTH;

	unsigned char *chunk_data = (unsigned char *) malloc(length * sizeof(unsigned char));

	chunk_data[0] = ihdr_chunk->width >> 24;
	chunk_data[1] = ihdr_chunk->width >> 16;
	chunk_data[2] = ihdr_chunk->width >> 8;
	chunk_data[3] = ihdr_chunk->width;

	chunk_data[4] = ihdr_chunk->height >> 24;
	chunk_data[5] = ihdr_chunk->height >> 16;
	chunk_data[6] = ihdr_chunk->height >> 8;
	chunk_data[7] = ihdr_chunk->height;

	chunk_data[8] = ihdr_chunk->bit_depth;
	chunk_data[9] = ihdr_chunk->color_type;
	chunk_data[10] = ihdr_chunk->compression_method;
	chunk_data[11] = ihdr_chunk->filter_method;
	chunk_data[12] = ihdr_chunk->interlace_method;

	write_chunk(file, length, IHDR, chunk_data);

	free(chunk_data);
}

void write_idat_chunk(FILE *file, ChunkIDAT *idat_chunk) {
	uLong bound = compressBound(idat_chunk->length);
	unsigned char *comp = malloc(bound);
	uLong comp_len = bound;

	compress2(comp, &comp_len, idat_chunk->data, idat_chunk->length, Z_BEST_COMPRESSION);

	write_chunk(file, comp_len, IDAT, comp);

	free(comp);
}
