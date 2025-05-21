#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"


size_t get_file_size (FILE *fp)
{
  size_t curr, sz;
  curr = ftell(fp);
  fseek(fp, 0, SEEK_END);
  sz = ftell(fp);
  fseek(fp, curr, SEEK_SET);
  return sz;
}

size_t read_file_buffed (FILE *fp, void *out, size_t len)
{
  const int BUFF_SZ = 4096;
  size_t pos = 0, rdneed, rdgot;
  while (len) {
    rdneed = len > BUFF_SZ ? BUFF_SZ : len;
    rdgot = fread(((char*)out) + pos, sizeof(char), rdneed, fp);
    pos += rdgot;
    len -= rdgot;
    if (rdneed != rdgot)
      break;
  }
  return pos;
}

char *read_file (const char *path, size_t *sz)
{
  FILE *fp;
  size_t len;
  char *mem;
  fp = fopen(path, "rb");
  if (!fp)
    goto err;
  len = get_file_size(fp);
  mem = (char*)malloc(sizeof(char) * len);
  if (!mem)
    goto err_file;
  if (read_file_buffed(fp, mem, len) != len)
    goto err_mem;
  fclose(fp);
  if (sz)
    *sz = len;
  return mem;
err_mem:
  free(mem);
err_file:
  fclose(fp);
err:
  return NULL;
}

char *read_ascii_file (const char *path, size_t *sz)
{
  FILE *fp;
  size_t len;
  char *mem;
  fp = fopen(path, "rb");
  if (!fp)
    goto err;
  len = get_file_size(fp);
  mem = (char*)malloc(sizeof(char) * (len + 1));
  if (!mem)
    goto err_file;
  if (read_file_buffed(fp, mem, len) != len)
    goto err_mem;
  fclose(fp);
  if (sz)
    *sz = len + 1;
  mem[len] = '\0';
  return mem;
err_mem:
  free(mem);
err_file:
  fclose(fp);
err:
  return NULL;
}

void fatal_err (char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  exit(1);
}

struct BumpArena *bar_new (size_t sz)
{
  struct BumpArena *bar = (struct BumpArena*)malloc(
              sizeof(struct BumpArena)-1+sz);
  if (!bar)
    fatal_err("Out of memory!\n");
  bar->next = NULL;
  bar->size = sz;
  bar->pos = 0;
  return bar;
}

void bar_free (struct BumpArena *bar)
{
  while (bar) {
    struct BumpArena *next = bar->next;
    free(bar);
    bar = next;
  }
}

void *bar_alloc (struct BumpArena *bar, size_t sz)
{
  if (!bar)
    return NULL;
  bar_align(bar, 8);
  if (bar->size - bar->pos >= sz) {
    void *ptr = &bar->data[bar->pos];
    bar->pos += sz;
    return ptr;
  }
  if (!bar->next)
    bar->next = bar_new(bar->size * 2 + sz);
  return bar_alloc(bar->next, sz);
}
