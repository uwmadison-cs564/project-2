#include "page_cache.hpp"

#include <cstdint>
#include <cstdlib>

Page::Page(int pageSize, int extraSize) : sqlite3_pcache_page() {
  pBufInner_ = malloc(pageSize + 15);
  pBuf = (void *)(((uintptr_t)pBufInner_ + 15) & ~(uintptr_t)0xF);
  pExtra = malloc(extraSize);
}

Page::~Page() {
  free(pBufInner_);
  free(pExtra);
}

PageCache::PageCache(int pageSize, int extraSize)
    : pageSize_(pageSize), extraSize_(extraSize), maxNumPages_(0),
      numFetches_(0), numHits_(0) {}
