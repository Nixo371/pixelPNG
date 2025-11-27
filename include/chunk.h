#ifndef CHUNK_H
# define CHUNK_H

# include <stdint.h>
# include <stddef.h>
# include <stdio.h>

// Forward Declarations //
typedef struct s_pixel pixel;
// -------------------- //

enum chunk_type {
	IHDR,
	IDAT,
	IEND
};

typedef struct s_ChunkIHDR {
	int width;			// pixels
	int height;			// pixels
	uint8_t bit_depth;		// Valid values are 1, 2, 4, 8, and 16
	uint8_t color_type;		// Valid values are 0, 2, 3, 4, and 6
	uint8_t compression_method;	// Only 0 is valid for now
	uint8_t filter_method;		// Only 0 is valid for now
	uint8_t interlace_method;	// 0 (no interlace) or 1 (Adam7 interlace)
} ChunkIHDR;

typedef struct s_ChunkIDAT {
	size_t length;
	unsigned char* data;
} ChunkIDAT;

// Generate Chunk Data
ChunkIHDR* generate_ihdr_chunk(int width, int height, uint8_t bit_depth, int color_type, uint8_t compression_method, uint8_t filter_method, uint8_t interlace_method);
ChunkIDAT* generate_idat_chunk(pixel** pixels, ChunkIHDR* ihdr_chunk);

// Write Chunks
void write_chunk(FILE* file, size_t length, int chunk_type, unsigned char* chunk_data);
void write_ihdr_chunk(FILE* file, ChunkIHDR* ihdr_chunk);
void write_idat_chunk(FILE* file, ChunkIDAT* idat_chunk);

#endif
