#pragma once

#include <types.h>

extern void* dccalloc(u64 count, u64 esize);

extern void* dcmalloc(u64 nsize);
MAYBE_UNUSED extern char* dcstrdup(const char *dups);
extern bool dcfree(void* fptr);