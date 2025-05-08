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

struct NOD {
  nod_allocfn_t   alloc_fn;
};

#endif /* NOD_CORE_H_ */
