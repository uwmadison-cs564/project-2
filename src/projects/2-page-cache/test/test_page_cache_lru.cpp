#include "page_cache_lru.hpp"
#include "test_page_cache_common.hpp"

const char *databaseName = "lru.sqlite";

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
}

void lruReplacementSQLScan() {
  int numHits;
  commonSQLScan<LRUReplacementPageCache>(databaseName, numHits);
  TEST_ASSERT(numHits == 262, "incorrect number of hits");
}

void lruReplacementSQLScanWithHotSet() {
  int numHits;
  commonSQLScanWithHotSet<LRUReplacementPageCache>(databaseName, numHits);
  TEST_ASSERT(numHits == 331, "incorrect number of hits");
}

void lruReplacementSQLUniformRandom() {
  int numHits;
  commonSQLUniformRandom<LRUReplacementPageCache>(databaseName, numHits);
  TEST_ASSERT(numHits == 292, "incorrect number of hits");
}

void lruReplacementSQLBinomialRandom() {
  int numHits;
  commonSQLBinomialRandom<LRUReplacementPageCache>(databaseName, numHits);
  TEST_ASSERT(numHits == 298, "incorrect number of hits");
}

int main() {
  commonAll<LRUReplacementPageCache>();

  TEST_RUN(lruReplacement1);
  TEST_RUN(lruReplacement2);
  TEST_RUN(lruReplacement3);
  TEST_RUN(lruReplacement4);

  return TEST_EXIT_CODE;
}
