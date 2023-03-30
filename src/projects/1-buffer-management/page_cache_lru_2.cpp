#include "page_cache_lru_2.hpp"
#include "utilities/exception.hpp"

LRU2ReplacementPageCache::LRU2ReplacementPageCache(int pageSize, int extraSize)
    : PageCache(pageSize, extraSize) {
  // TODO: Implement.
  throw NotImplementedException("LRU2ReplacementPageCache::LRU2ReplacementPageCache");
}

void LRU2ReplacementPageCache::setMaxNumPages(int maxNumPages) {
  // TODO: Implement.
  throw NotImplementedException("LRU2ReplacementPageCache::setMaxNumPages");
}

int LRU2ReplacementPageCache::getNumPages() const {
  // TODO: Implement.
  throw NotImplementedException("LRU2ReplacementPageCache::getNumPages");
}

Page *LRU2ReplacementPageCache::fetchPage(unsigned pageId, bool allocate) {
  // TODO: Implement.
  throw NotImplementedException("LRU2ReplacementPageCache::fetchPage");
}

void LRU2ReplacementPageCache::unpinPage(Page *page, bool discard) {
  // TODO: Implement.
  throw NotImplementedException("LRU2ReplacementPageCache::unpinPage");
}

void LRU2ReplacementPageCache::changePageId(Page *page, unsigned newPageId) {
  // TODO: Implement.
  throw NotImplementedException("LRU2ReplacementPageCache::changePageId");
}

void LRU2ReplacementPageCache::discardPages(unsigned pageIdLimit) {
  // TODO: Implement.
  throw NotImplementedException("LRU2ReplacementPageCache::discardPages");
}
