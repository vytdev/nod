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
  for (;;) {
    rdneed = len > BUFF_SZ ? BUFF_SZ : len;
    rdgot = fread(((char*)out) + pos, sizeof(char), rdneed, fp);
    pos += rdgot;
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
