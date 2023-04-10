#include "page_cache_lru_2.hpp"
#include "test_page_cache_common.hpp"

const char *databaseName = "lru2.sqlite";

void lru2Replacement1() {
  LRU2ReplacementPageCache pageCache(4096, 8);
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

void lru2Replacement2() {
  LRU2ReplacementPageCache pageCache(4096, 8);
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

void lru2Replacement3() {
  LRU2ReplacementPageCache pageCache(4096, 8);
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

void lru2Replacement4() {
  LRU2ReplacementPageCache pageCache(4096, 8);
  pageCache.setMaxNumPages(2);
  Page *page1, *page2;
  page1 = pageCache.fetchPage(1, true);
  pageCache.unpinPage(page1, false);
  page1 = pageCache.fetchPage(1, true);
  pageCache.unpinPage(page1, false);
  page2 = pageCache.fetchPage(2, true);
  pageCache.unpinPage(page2, false);
  pageCache.fetchPage(3, true);
  page2 = pageCache.fetchPage(2, false);
  // Page 2 should have been replaced.
  TEST_ASSERT(page2 == nullptr, "expected null pointer");
}

void lru2Replacement5() {
  LRU2ReplacementPageCache pageCache(4096, 8);
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
  page1 = pageCache.fetchPage(1, false);
  // Page 1 should have been replaced.
  TEST_ASSERT(page1 == nullptr, "expected null pointer");
}

void lru2ReplacementSQLScan() {
  int numHits;
  commonSQLScan<LRU2ReplacementPageCache>(databaseName, numHits);
  TEST_ASSERT(numHits == 262, "incorrect number of hits");
}

void lru2ReplacementSQLScanWithHotSet() {
  int numHits;
  commonSQLScanWithHotSet<LRU2ReplacementPageCache>(databaseName, numHits);
  TEST_ASSERT(numHits == 352, "incorrect number of hits");
}

void lru2ReplacementSQLUniformRandom() {
  int numHits;
  commonSQLUniformRandom<LRU2ReplacementPageCache>(databaseName, numHits);
  TEST_ASSERT(numHits == 292, "incorrect number of hits");
}

void lru2ReplacementSQLBinomialRandom() {
  int numHits;
  commonSQLBinomialRandom<LRU2ReplacementPageCache>(databaseName, numHits);
  TEST_ASSERT(numHits == 298, "incorrect number of hits");
}

int main() {
  commonAll<LRU2ReplacementPageCache>();

  TEST_RUN(lru2Replacement1);
  TEST_RUN(lru2Replacement2);
  TEST_RUN(lru2Replacement3);
  TEST_RUN(lru2Replacement4);
  TEST_RUN(lru2Replacement5);

  return TEST_EXIT_CODE;
}
