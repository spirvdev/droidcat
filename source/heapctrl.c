#include <heapctrl.h>

#include <malloc.h>
#include <string.h>

#include <unistd.h>

#include <whiskey/release.h>

static u64 gs_page_size = 0;

void* dccalloc(u64 count, u64 esize)
{
    whiskey_log_assert(NULL, count == 0 || esize == 0, "count and esize mustn't be zero");

    void* nptr = calloc(count, esize);

    if (nptr == NULL)
    {
        return NULL;
    }

    return nptr;
}

void* dcmalloc(u64 nsize)
{
    whiskey_log_assert(NULL, nsize == 0, "nsize mustn't be zero");

    if (gs_page_size == 0)
    {
        gs_page_size = (i32)sysconf(_SC_PAGE_SIZE);
        whiskey_log_info(NULL, "System memory page size has fetched: %d\n", gs_page_size);
    }

    if (nsize > gs_page_size)
    {
        return dccalloc(1, nsize);
    }

    return malloc(nsize);
}

MAYBE_UNUSED char* dcstrdup(const char* dups)
{
    const u64 slen = strlen(dups);
    char* region = (char*)dcmalloc(slen+1);

    if (region == NULL) return NULL;

    strncpy(region, dups, slen);

    slen[region] = '\0';
    return region;
}

MAYBE_UNUSED char* dcstrndup(const char *dups, u64 sdups)
{
    if (*dups == '\0')		return NULL;
    if (strlen(dups) <= sdups)	return NULL;

    char* region = (char*)dcmalloc(sdups + 1);
    if (region == NULL) return NULL;

    strncpy(region, dups, sdups);
  
    sdups[region] = '\0';
    return region;
}

MAYBE_UNUSED bool dcfree(void* fptr)
{
    if (fptr == NULL) return false;

    free(fptr);

    return true;
}


