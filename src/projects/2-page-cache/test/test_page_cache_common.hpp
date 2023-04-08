#ifndef CS564_PROJECT_TEST_PAGE_CACHE_COMMON_HPP
#define CS564_PROJECT_TEST_PAGE_CACHE_COMMON_HPP

#include "page_cache.hpp"
#include "utilities/test.hpp"

template <typename T> void commonNumPages() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  TEST_ASSERT(pageCache.getNumPages() == 0, "incorrect number of pages");
  Page *page1 = pageCache.fetchPage(1, true);
  TEST_ASSERT(pageCache.getNumPages() == 1, "incorrect number of pages");
  pageCache.unpinPage(page1, true);
  TEST_ASSERT(pageCache.getNumPages() == 0, "incorrect number of pages");
}

template <typename T> void commonFetchMiss() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  Page *page1 = pageCache.fetchPage(1, false);
  TEST_ASSERT(page1 == nullptr, "expected null pointer");
  TEST_ASSERT(pageCache.getNumFetches() == 1, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 0, "incorrect number of hits");
}

template <typename T> void commonFetchTwice() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  Page *page1 = pageCache.fetchPage(1, true);
  Page *page2 = pageCache.fetchPage(1, true);
  TEST_ASSERT(page1 == page2, "page pointers are not equal");
  TEST_ASSERT(pageCache.getNumFetches() == 2, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 1, "incorrect number of hits");
}

template <typename T> void commonFetchFull() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  Page *page1 = pageCache.fetchPage(1, true);
  pageCache.unpinPage(page1, false);
  Page *page2 = pageCache.fetchPage(2, true);
  TEST_ASSERT(page2 != nullptr, "expected valid pointer");
  TEST_ASSERT(pageCache.getNumFetches() == 2, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 0, "incorrect number of hits");
}

template <typename T> void commonFetchPinnedFull() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  pageCache.fetchPage(1, true);
  Page *page2 = pageCache.fetchPage(2, true);
  TEST_ASSERT(page2 == nullptr, "expected null pointer");
  TEST_ASSERT(pageCache.getNumFetches() == 2, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 0, "incorrect number of hits");
}

template <typename T> void commonChangePageId() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  Page *page1, *page2;
  page1 = pageCache.fetchPage(1, true);
  pageCache.changePageId(page1, 2);
  page2 = pageCache.fetchPage(2, false);
  TEST_ASSERT(page1 == page2, "expected equivalent pointers");
  page1 = pageCache.fetchPage(1, false);
  TEST_ASSERT(page1 == nullptr, "expected null pointer");
  TEST_ASSERT(pageCache.getNumFetches() == 3, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 1, "incorrect number of hits");
}

template <typename T> void commonDiscardPages() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(2);
  pageCache.fetchPage(1, true);
  pageCache.fetchPage(2, true);
  pageCache.discardPages(2);
  Page *page1 = pageCache.fetchPage(1, false);
  TEST_ASSERT(page1 != nullptr, "expected valid pointer");
  Page *page2 = pageCache.fetchPage(2, false);
  TEST_ASSERT(page2 == nullptr, "expected null pointer");
  TEST_ASSERT(pageCache.getNumFetches() == 4, "incorrect number of fetches");
  TEST_ASSERT(pageCache.getNumHits() == 1, "incorrect number of hits");
}

template <typename T> void commonAll() {
  TEST_RUN(commonFetchMiss<T>);
  TEST_RUN(commonFetchTwice<T>);
  TEST_RUN(commonFetchFull<T>);
  TEST_RUN(commonFetchPinnedFull<T>);
  TEST_RUN(commonChangePageId<T>);
  TEST_RUN(commonDiscardPages<T>);
  TEST_RUN(commonNumPages<T>);
}

#endif // CS564_PROJECT_TEST_PAGE_CACHE_COMMON_HPP
