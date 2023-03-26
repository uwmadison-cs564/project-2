#ifndef SQLITE_EDU_PAGE_CACHE_ADAPTER_HPP
#define SQLITE_EDU_PAGE_CACHE_ADAPTER_HPP

#include "dependencies/sqlite/sqlite3.h"
#include "page_cache.hpp"

int pageCacheInit(void *);
void pageCacheShutdown(void *);
sqlite3_pcache *pageCacheCreate(int szPage, int szExtra, int bPurgeable);
void pageCacheCachesize(sqlite3_pcache *pcache, int nCachesize);
int pageCachePagecount(sqlite3_pcache *pcache);
sqlite3_pcache_page *pageCacheFetch(sqlite3_pcache *pcache, unsigned key,
                                    int createFlag);
void pageCacheUnpin(sqlite3_pcache *pcache, sqlite3_pcache_page *page,
                    int discard);
void pageCacheRekey(sqlite3_pcache *pcache, sqlite3_pcache_page *page,
                    unsigned oldKey, unsigned newKey);
void pageCacheTruncate(sqlite3_pcache *pcache, unsigned iLimit);
void pageCacheDestroy(sqlite3_pcache *pcache);

static sqlite3_pcache_methods2 bufferManagerMethods = {
    .xInit = pageCacheInit,
    .xShutdown = pageCacheShutdown,
    .xCreate = pageCacheCreate,
    .xCachesize = pageCacheCachesize,
    .xPagecount = pageCachePagecount,
    .xFetch = pageCacheFetch,
    .xUnpin = pageCacheUnpin,
    .xRekey = pageCacheRekey,
    .xTruncate = pageCacheTruncate,
    .xDestroy = pageCacheDestroy};

#endif // SQLITE_EDU_PAGE_CACHE_ADAPTER_HPP
