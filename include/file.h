#ifndef FILE_H
#define FILE_H

#include <stddef.h>

char *read_file(const char *restrict path, size_t *out_size);

size_t file_size(const char *restrict path);

#endif
