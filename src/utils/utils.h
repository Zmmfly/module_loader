#ifndef __FM_LD_UTILS__
#define __FM_LD_UTILS__

#include <stdint.h>

typedef void(*walk_fn_t)(const char *path, void *arg);

int dir_walk(const char *path, walk_fn_t fn, void *arg);
bool is_dir(const char *path);
bool is_file(const char *path);

#endif