#include "chunk.h"

#include <stdlib.h>

void free_ihdr_chunk(ChunkIHDR *ihdr_chunk) {
	free(ihdr_chunk);
}

void free_idat_chunk(ChunkIDAT *idat_chunk) {
	free(idat_chunk->data);
	free(idat_chunk);
}
