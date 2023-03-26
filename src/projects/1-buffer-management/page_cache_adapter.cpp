#include "page_cache_adapter.hpp"

#include <iostream>

int pageCacheInit(void *) { return SQLITE_OK; }

void pageCacheShutdown(void *) {}

sqlite3_pcache *pageCacheCreate(int szPage, int szExtra, int) {
  return (sqlite3_pcache *)new PageCache(szPage, szExtra);
}

void pageCacheCachesize(sqlite3_pcache *pageCacheBase, int nCachesize) {
  std::cout << "pageCacheCachesize" << std::endl;
}

int pageCachePagecount(sqlite3_pcache *pageCacheBase) {
  auto *pageCache = (PageCache *)pageCacheBase;
  return pageCache->getPageCount();
}

sqlite3_pcache_page *pageCacheFetch(sqlite3_pcache *pageCacheBase, unsigned key,
                                    int createFlag) {
  auto *pageCache = (PageCache *)pageCacheBase;
  return pageCache->request(key, createFlag != 0);
}

void pageCacheUnpin(sqlite3_pcache *pageCacheBase,
                    sqlite3_pcache_page *pageBase, int discard) {
  auto *pageCache = (PageCache *)pageCacheBase;
  auto *page = (Page *)pageBase;
  pageCache->release(page, discard);
}

void pageCacheRekey(sqlite3_pcache *pageCacheBase,
                    sqlite3_pcache_page *pageBase, unsigned oldKey,
                    unsigned newKey) {
  auto *pageCache = (PageCache *)pageCacheBase;
  auto *page = (Page *)pageBase;
  pageCache->rekey(page, newKey);
}

void pageCacheTruncate(sqlite3_pcache *pageCacheBase, unsigned iLimit) {
  auto *pageCache = (PageCache *)pageCacheBase;
  pageCache->truncate(iLimit);
}

void pageCacheDestroy(sqlite3_pcache *pageCacheBase) {
  delete (PageCache *)pageCacheBase;
}
