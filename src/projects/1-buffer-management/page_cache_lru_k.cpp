#include "page_cache_lru_k.hpp"
#include "utilities/exception.hpp"

LRUKReplacementPageCache::LRUKReplacementPageCache(int pageSize, int extraSize)
    : PageCache(pageSize, extraSize) {
  // TODO: Implement.
  throw NotImplementedException("LRUKPageCache::LRUKPageCache");
}

void LRUKReplacementPageCache::setMaxNumPages(int maxNumPages) {
  // TODO: Implement.
  throw NotImplementedException("LRUKPageCache::setMaxNumPages");
}

int LRUKReplacementPageCache::getNumPages() const {
  // TODO: Implement.
  throw NotImplementedException("LRUKPageCache::getNumPages");
}

Page *LRUKReplacementPageCache::fetchPage(unsigned pageId, bool allocate) {
  // TODO: Implement.
  throw NotImplementedException("LRUKPageCache::fetchPage");
}

void LRUKReplacementPageCache::unpinPage(Page *page, bool discard) {
  // TODO: Implement.
  throw NotImplementedException("LRUKPageCache::unpinPage");
}

void LRUKReplacementPageCache::changePageId(Page *page, unsigned newPageId) {
  // TODO: Implement.
  throw NotImplementedException("LRUKPageCache::changePageId");
}

void LRUKReplacementPageCache::discardPages(unsigned pageIdLimit) {
  // TODO: Implement.
  throw NotImplementedException("LRUKPageCache::discardPages");
}
