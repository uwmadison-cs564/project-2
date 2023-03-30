#include "page_cache_lru.hpp"
#include "utilities/exception.hpp"

LRUReplacementPageCache::LRUReplacementPageCache(int pageSize, int extraSize)
    : PageCache(pageSize, extraSize) {
  // TODO: Implement.
  throw NotImplementedException("LRUPageCache::LRUPageCache");
}

void LRUReplacementPageCache::setMaxNumPages(int maxNumPages) {
  // TODO: Implement.
  throw NotImplementedException("LRUPageCache::setMaxNumPages");
}

int LRUReplacementPageCache::getNumPages() const {
  // TODO: Implement.
  throw NotImplementedException("LRUPageCache::getNumPages");
}

Page *LRUReplacementPageCache::fetchPage(unsigned pageId, bool allocate) {
  // TODO: Implement.
  throw NotImplementedException("LRUPageCache::fetchPage");
}

void LRUReplacementPageCache::unpinPage(Page *page, bool discard) {
  // TODO: Implement.
  throw NotImplementedException("LRUPageCache::unpinPage");
}

void LRUReplacementPageCache::changePageId(Page *page, unsigned newPageId) {
  // TODO: Implement.
  throw NotImplementedException("LRUPageCache::changePageId");
}

void LRUReplacementPageCache::discardPages(unsigned pageIdLimit) {
  // TODO: Implement.
  throw NotImplementedException("LRUPageCache::discardPages");
}
