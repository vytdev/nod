/*
 * nod -- An embedded scripting language.
 * This file is part of the 'nod' project.
 * Copyright (C) 2025  Vincent Yanzee J. Tan
 * Licensed under the MIT License.
 * See LICENSE for more info.
 * Repo:  https://github.com/vytdev/nod
 */

#include <stddef.h> /* for NULL. */
#include "nod.h"

#define NOD_INTERNAL
#include "core.h"


NOD *nod_open (nod_allocfn_t fn)
{
  NOD *N = (NOD*)fn(NULL, sizeof(NOD));
  if (!N)
    return NULL;
  N->alloc_fn = fn;
  /* setup stack.  */
  N->s_ptr = (char*)nodI_malloc(N, NOD_INITSTACKSZ);
  if (!N->s_ptr) {
    nodI_mfree(N, N);
    return NULL;
  }
  N->s_sz = NOD_INITSTACKSZ;
  N->s_top = 0;
  /* setup code. */
  N->code = NULL;
  N->codesz = 0;
  return N;
}


void nod_close (NOD *N)
{
  if (!N)
    return;
  nodI_mfree(N, N->s_ptr);
  nodI_mfree(N, N);
}


void nod_load (NOD *N, void *code, nod_sz_t sz)
{
  N->code = code;
  N->codesz = sz;
}


void *nodI_stalloc (NOD *N, nod_sz_t sz)
{
  nod_sz_t lasttop = N->s_top;
  nod_sz_t newtop = lasttop + sz;
  /* try resize the stack */
  if (newtop > N->s_sz) {
    char *sptr;
    nod_sz_t newsz = N->s_sz;
    /* accommodate 'sz'. */
    for (;newsz - lasttop < sz;)
      newsz <<= 1; /* newsz * 2 */
    sptr = (char*)nodI_mresize(N, N->s_ptr, newsz);
    if (!sptr)
      return NULL;
    N->s_ptr = sptr;
  }
  N->s_top = newtop;
  return (void*)(N->s_ptr + lasttop);
}


void nodI_stfree (NOD *N, nod_sz_t sz)
{
  if (N->s_top < sz)
    N->s_top = 0;
  else
    N->s_top -= sz;
}
