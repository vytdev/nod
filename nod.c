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
  NOD *ctx = (NOD*)fn(NULL, sizeof(NOD));
  if (!ctx)
    return NULL;
  ctx->alloc_fn = fn;
  return ctx;
}


void nod_close (NOD *N)
{
  if (N)
    N->alloc_fn(N, 0);
}
