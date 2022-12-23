#include <heapctrl.h>

#include <malloc.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>

static u64 gs_page_size = 0;

void* dccalloc(u64 count, u64 esize)
{        
    void* nptr = calloc(count, esize);

    if (nptr == NULL)
    {
        return NULL;
    }

    return nptr;
}

void* dcmalloc(u64 nsize)
{
    if (gs_page_size == 0)
    {
        gs_page_size = sysconf(_SC_PAGE_SIZE);
    }

    if (nsize > gs_page_size)
    {
        return dccalloc(1, nsize);
    }

    return malloc(nsize);
}

char* dcstrdup(const char *dups)
{
  const u64 slen = strlen(dups);
  char* region = (char*)dcmalloc(slen+1);

  if (region == NULL) return NULL;

  strncpy(region, dups, slen);
  
  slen[region] = '\0';

  return region;
}

bool dcfree(void* fptr)
{
    if (fptr == NULL) return false;

    free(fptr);

    return true;
}


