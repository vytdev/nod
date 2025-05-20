#ifndef NOD_UTIL_H_
#define NOD_UTIL_H_   1

#include <stddef.h>
#include <stdio.h>

size_t get_file_size (FILE *fp);
size_t read_file_buffed (FILE *fp, void *out, size_t len);
char *read_file (const char *path, size_t *sz);

struct BumpArena
{
  struct BumpArena *next;
  size_t size, pos;
  char data[1];
};

/* create a new arena. */
struct BumpArena *bar_new (size_t sz);

/* free the arena. */
void bar_free (struct BumpArena *bar);

/* allocate `sz` bytes from `bar` */
void *bar_alloc (struct BumpArena *bar, size_t sz);

#define bar_align(bar, al) \
         ((bar)->pos = ((bar)->pos + (al) - 1) & ~((al) - 1))

#endif /* NOD_UTIL_H_ */
