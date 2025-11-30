#include "file.h"
#include "sys/stat.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *const restrict path, size_t *const out_size)
{
	*out_size = 0;

	FILE *f = fopen(path, "rb");
	if (!f)
		return NULL;

	fseek(f, 0, SEEK_END);
	const size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (size == 0) {
		fclose(f);
		return NULL;
	}

	char *buffer = malloc(size);
	if (!buffer) {
		fclose(f);
		return NULL;
	}

	const size_t read = fread(buffer, 1, size, f);
	fclose(f);

	if (read != size) {
		free(buffer);
		return NULL;
	}

	*out_size = size;
	return buffer;
}

size_t file_size(const char *const restrict path)
{
	struct stat st;

	if (stat(path, &st) != 0)
		return 0;

	return st.st_size;
}
