#ifndef PIXELPNG_H
# define PIXELPNG_H

// https://www.libpng.org/pub/png/spec/1.2/PNG-Chunks.html

# include <stddef.h>
# include <stdint.h>

# define IHDR_CHUNK_LENGTH 13

// Forward Declarations //
typedef struct s_pixel pixel;

typedef struct s_ChunkIHDR ChunkIHDR;
// -------------------- //

enum color_type {
	GRAYSCALE = 0,
	RGB = 2,
	PALETTE = 3,
	GRAYSCALE_ALPHA = 4,
	RGBA = 6
};

typedef struct s_pixelPNG {
	ChunkIHDR* ihdr_chunk;
	pixel** pixels;
} pixelPNG;

// PNG
pixelPNG* initialize_png(int width, int height);
void generate_png(pixelPNG* pixelPNG, char* file_name, uint8_t bit_depth, int color_type, uint8_t compression_method, uint8_t filter_method, uint8_t interlace_method);
void free_png(pixelPNG* pixelPNG);

// CRC
// https://www.libpng.org/pub/png/spec/1.2/PNG-CRCAppendix.html
void make_crc_table(void);
unsigned long update_crc(unsigned long crc, unsigned char *buf, int len);
unsigned long crc(unsigned char *buf, int len);

#endif
