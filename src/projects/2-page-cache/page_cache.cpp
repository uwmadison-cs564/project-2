#include "page_cache.hpp"

#include <memory>

Page::Page(int pageSize, int extraSize) : sqlite3_pcache_page() {
  pBuf = std::aligned_alloc(8, pageSize);
  pExtra = malloc(extraSize);
}

Page::~Page() {
  free(pBuf);
  free(pExtra);
}

PageCache::PageCache(int pageSize, int extraSize)
    : pageSize_(pageSize), extraSize_(extraSize), maxNumPages_(0),
      numFetches_(0), numHits_(0) {}
