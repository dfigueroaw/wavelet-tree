#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stddef.h>

typedef struct {
	const char *name;
	size_t (*compress)(const char *path);
} Compressor;

extern const Compressor GZIP_COMPRESSOR;
extern const Compressor BZIP2_COMPRESSOR;
extern const Compressor XZ_COMPRESSOR;
extern const Compressor ZSTD_COMPRESSOR;

#endif
