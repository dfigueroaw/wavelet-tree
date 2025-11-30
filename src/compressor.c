#include "compressor.h"
#include "file.h"
#include "types.h"
#include "unistd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static i32 run_cmd(const char *const cmd)
{
	i32 ret = system(cmd); // NOLINT
	return (ret == 0);
}

static char *make_temp_path(const char *const suffix)
{
	char tmpl[64] = "/tmp/compressXXXXXXXXX";
	i32 fd = mkstemp(tmpl);
	if (fd < 0)
		return NULL;

	close(fd);

	size_t len = strlen(tmpl) + strlen(suffix) + 1;
	char *path = malloc(len);
	if (!path)
		return NULL;

	snprintf(path, len, "%s%s", tmpl, suffix);
	return path;
}

static size_t gzip_compress(const char *const path)
{
	char *const out = make_temp_path(".gz");
	if (!out)
		return 0;

	char cmd[256];
	snprintf(cmd, sizeof(cmd), "gzip -c \"%s\" > \"%s\"", path, out);

	if (!run_cmd(cmd)) {
		free(out);
		return 0;
	}

	size_t size = file_size(out);
	free(out);

	return size;
}

static size_t bzip2_compress(const char *const path)
{
	char *const out = make_temp_path(".bz2");
	if (!out)
		return 0;

	char cmd[256];
	snprintf(cmd, sizeof(cmd), "bzip2 -c \"%s\" > \"%s\"", path, out);

	if (!run_cmd(cmd)) {
		free(out);
		return 0;
	}

	size_t size = file_size(out);
	free(out);

	return size;
}

static size_t xz_compress(const char *const path)
{
	char *const out = make_temp_path(".xz");
	if (!out)
		return 0;

	char cmd[256];
	snprintf(cmd, sizeof(cmd), "xz -c \"%s\" > \"%s\"", path, out);

	if (!run_cmd(cmd)) {
		free(out);
		return 0;
	}

	size_t size = file_size(out);
	free(out);

	return size;
}

static size_t zstd_compress(const char *const path)
{
	char *const out = make_temp_path(".zst");
	if (!out)
		return 0;

	char cmd[256];
	snprintf(cmd, sizeof(cmd), "zstd -c \"%s\" > \"%s\"", path, out);

	if (!run_cmd(cmd)) {
		free(out);
		return 0;
	}

	size_t size = file_size(out);
	free(out);

	return size;
}

const Compressor GZIP_COMPRESSOR = (Compressor){
	.name = "gzip",
	.compress = gzip_compress,
};
const Compressor BZIP2_COMPRESSOR = (Compressor){
	.name = "bzip2",
	.compress = bzip2_compress,
};
const Compressor XZ_COMPRESSOR = (Compressor){
	.name = "xz",
	.compress = xz_compress,
};
const Compressor ZSTD_COMPRESSOR = (Compressor){
	.name = "zstd",
	.compress = zstd_compress,
};
