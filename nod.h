/*
 * nod -- An embedded scripting language.
 * Copyright (C) 2025  Vincent Yanzee J. Tan
 * Licensed under the MIT License.
 * See LICENSE for more info.
 * Repo:  https://github.com/vytdev/nod
 */

#ifndef NOD_H_
#define NOD_H_   1

#define NOD_SHORTNAME  "Nod"
#define NOD_COPYRIGHT  "Copyright (C) 2025  Vincent Yanzee J. Tan"

#define NOD_VERSION    0
#define NOD_PATCH      1
#define NOD_RELEASE  (NOD_VERSION * 10000L + NOD_PATCH)

#define NODI_TOSTRAUX(x) #x
#define NODI_TOSTR(x)    NODI_TOSTRAUX(x)

#define NOD_VERSTR    NODI_TOSTR(NOD_VERSION) "." NODI_TOSTR(NOD_PATCH)
#define NOD_LABEL     NOD_SHORTNAME " " NOD_VERSTR


/* nod state. */
typedef struct NOD NOD;

typedef void * (* nod_allocfn_t) (void *, unsigned long);
typedef unsigned long nod_sz_t;


#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Creates a new nod instance.
 */
NOD *nod_open (nod_allocfn_t fn);

/*
 * Close a nod instance.
 */
void nod_close (NOD *N);

/*
 * Load a bytecode.
 */
void nod_load (NOD *N, void *code, nod_sz_t sz);

#if defined(__cplusplus)
} /* extern "C" */
#endif
#endif /* NOD_H_ */
