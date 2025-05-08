/*
 * nod -- An embedded scripting language.
 * This file is part of the 'nod' project.
 * Copyright (C) 2025  Vincent Yanzee J. Tan
 * Licensed under the MIT License.
 * See LICENSE for more info.
 * Repo:  https://github.com/vytdev/nod
 */

#if !defined(NOD_INTERNAL)
#  error "nod/core.h is for internal-use only. Please use nod/nod.h instead"
#endif

#ifndef NOD_CORE_H_
#define NOD_CORE_H_   1
#include "nod.h"

#define NOD_INITSTACKSZ (1<<15)  /* 32kB */


struct NOD {
  nod_allocfn_t   alloc_fn;

  /* nod stack (grows upwards) */
  nod_sz_t  s_sz;   /* curr stack sz. */
  nod_sz_t  s_top;  /* curr stack pos. */
  char     *s_ptr;  /* stack alloc ptr. */

  /* bytecode */
  char     *code;
  nod_sz_t  codesz;
};


#define nodI_malloc(N, s)     ((N)->alloc_fn(NULL, (s)))
#define nodI_mfree(N, p)      ((N)->alloc_fn((p), 0))
#define nodI_mresize(N, p, s) ((N)->alloc_fn((p), (s)))

void *nodI_stalloc (NOD *N, nod_sz_t sz);
void nodI_stfree (NOD *N, nod_sz_t sz);

#endif /* NOD_CORE_H_ */
