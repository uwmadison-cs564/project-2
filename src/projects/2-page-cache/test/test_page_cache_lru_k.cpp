#include "page_cache_lru_2.hpp"
#include "test_page_cache_common.hpp"

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
  TEST_ASSERT(pageCache.getNumFetches() == 4, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 0, "incorrect number of hits");
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
  TEST_ASSERT(pageCache.getNumFetches() == 4, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 0, "incorrect number of hits");
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
  TEST_ASSERT(pageCache.getNumFetches() == 5, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 0, "incorrect number of hits");
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
  TEST_ASSERT(pageCache.getNumFetches() == 5, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 1, "incorrect number of hits");
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
  TEST_ASSERT(pageCache.getNumFetches() == 6, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 2, "incorrect number of hits");
}

void lru2ReplacementSQLScan() {
  unsigned long long numFetches;
  unsigned long long numHits;
  commonSQLScan<LRU2ReplacementPageCache>(numFetches, numHits);
  TEST_ASSERT(numFetches == 272, "incorrect number of fetches");
  TEST_ASSERT(numHits == 20, "incorrect number of hits");
}

void lru2ReplacementSQLScanWithHotSet() {
  unsigned long long numFetches;
  unsigned long long numHits;
  commonSQLScanWithHotSet<LRU2ReplacementPageCache>(numFetches, numHits);
  TEST_ASSERT(numFetches == 362, "incorrect number of fetches");
  TEST_ASSERT(numHits == 166, "incorrect number of hits");
}

void lru2ReplacementSQLUniformRandom() {
  unsigned long long numFetches;
  unsigned long long numHits;
  commonSQLUniformRandom<LRU2ReplacementPageCache>(numFetches, numHits);
  TEST_ASSERT(numFetches == 302, "incorrect number of fetches");
  TEST_ASSERT(numHits == 229, "incorrect number of hits");
}

void lru2ReplacementSQLBinomialRandom() {
  unsigned long long numFetches;
  unsigned long long numHits;
  commonSQLBinomialRandom<LRU2ReplacementPageCache>(numFetches, numHits);
  TEST_ASSERT(numFetches == 302, "incorrect number of fetches");
  TEST_ASSERT(numHits == 298, "incorrect number of hits");
}

int main() {
  commonAll<LRU2ReplacementPageCache>();

  TEST_RUN(lru2Replacement1);
  TEST_RUN(lru2Replacement2);
  TEST_RUN(lru2Replacement3);
  TEST_RUN(lru2Replacement4);
  TEST_RUN(lru2Replacement5);

  loadSQLiteDatabase();

  TEST_RUN(lru2ReplacementSQLScan);
  TEST_RUN(lru2ReplacementSQLScanWithHotSet);
  TEST_RUN(lru2ReplacementSQLUniformRandom);
  TEST_RUN(lru2ReplacementSQLBinomialRandom);

  return TEST_EXIT_CODE;
}
