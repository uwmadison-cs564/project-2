#include "page_cache_lru.hpp"
#include "test_page_cache_common.hpp"

void lruReplacement1() {
  LRUReplacementPageCache pageCache(4096, 8);
  pageCache.setMaxNumPages(2);
  Page *page1, *page2;
  page1 = pageCache.fetchPage(1, true);
  pageCache.unpinPage(page1, false);
  page2 = pageCache.fetchPage(2, true);
  pageCache.unpinPage(page2, false);
  pageCache.fetchPage(3, true);
  page1 = pageCache.fetchPage(1, false);
  // Page 1 should have been replaced.
  TEST_ASSERT(page1 == nullptr, "expected null pointer");
  TEST_ASSERT(pageCache.getNumFetches() == 4, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 0, "incorrect number of hits");
}

void lruReplacement2() {
  LRUReplacementPageCache pageCache(4096, 8);
  pageCache.setMaxNumPages(2);
  Page *page1, *page2;
  page1 = pageCache.fetchPage(1, true);
  page2 = pageCache.fetchPage(2, true);
  pageCache.unpinPage(page2, false);
  pageCache.unpinPage(page1, false);
  pageCache.fetchPage(3, true);
  page2 = pageCache.fetchPage(2, false);
  // Page 2 should have been replaced.
  TEST_ASSERT(page2 == nullptr, "expected null pointer");
  TEST_ASSERT(pageCache.getNumFetches() == 4, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 0, "incorrect number of hits");
}

void lruReplacement3() {
  LRUReplacementPageCache pageCache(4096, 8);
  pageCache.setMaxNumPages(3);
  Page *page2, *page3;
  pageCache.fetchPage(1, true);
  page2 = pageCache.fetchPage(2, true);
  pageCache.unpinPage(page2, false);
  page3 = pageCache.fetchPage(3, true);
  pageCache.unpinPage(page3, false);
  pageCache.fetchPage(4, true);
  page2 = pageCache.fetchPage(2, false);
  // Page 2 should have been replaced.
  TEST_ASSERT(page2 == nullptr, "expected null pointer");
  TEST_ASSERT(pageCache.getNumFetches() == 5, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 0, "incorrect number of hits");
}

void lruReplacement4() {
  LRUReplacementPageCache pageCache(4096, 8);
  pageCache.setMaxNumPages(2);
  Page *page1, *page2;
  page1 = pageCache.fetchPage(1, true);
  pageCache.unpinPage(page1, false);
  page2 = pageCache.fetchPage(2, true);
  pageCache.unpinPage(page2, false);
  page2 = pageCache.fetchPage(2, true);
  pageCache.unpinPage(page2, false);
  page1 = pageCache.fetchPage(1, true);
  pageCache.unpinPage(page1, false);
  pageCache.fetchPage(3, true);
  page2 = pageCache.fetchPage(2, false);
  // Page 2 should have been replaced.
  TEST_ASSERT(page2 == nullptr, "expected null pointer");
  TEST_ASSERT(pageCache.getNumFetches() == 6, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 2, "incorrect number of hits");
}

int main() {
  commonAll<LRUReplacementPageCache>();
  TEST_RUN(lruReplacement1);
  TEST_RUN(lruReplacement2);
  TEST_RUN(lruReplacement3);
  TEST_RUN(lruReplacement4);
  return TEST_EXIT_CODE;
}
