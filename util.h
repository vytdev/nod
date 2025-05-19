#ifndef NOD_UTIL_H_
#define NOD_UTIL_H_   1

#include <stddef.h>
#include <stdio.h>

size_t get_file_size (FILE *fp);
size_t read_file_buffed (FILE *fp, void *out, size_t len);
char *read_file (const char *path, size_t *sz);

#endif /* NOD_UTIL_H_ */
