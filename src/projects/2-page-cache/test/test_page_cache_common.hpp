#ifndef CS564_PROJECT_TEST_PAGE_CACHE_COMMON_HPP
#define CS564_PROJECT_TEST_PAGE_CACHE_COMMON_HPP

#include "page_cache.hpp"
#include "utilities/test.hpp"

template <typename T> void commonNumPages() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  TEST_ASSERT(pageCache.getNumPages() == 0, "incorrect number of pages");
  Page *page = pageCache.fetchPage(1, true);
  TEST_ASSERT(pageCache.getNumPages() == 1, "incorrect number of pages");
  pageCache.unpinPage(page, true);
  TEST_ASSERT(pageCache.getNumPages() == 0, "incorrect number of pages");
}

template <typename T> void commonFetchMiss() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  TEST_ASSERT(pageCache.fetchPage(1, false) == nullptr,
              "expected null pointer");
}

template <typename T> void commonFetchTwice() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  Page *page1 = pageCache.fetchPage(1, true);
  Page *page2 = pageCache.fetchPage(1, true);
  TEST_ASSERT(page1 == page2, "page pointers are not equal");
}

template <typename T> void commonFetchFull() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  Page *page = pageCache.fetchPage(1, true);
  pageCache.unpinPage(page, false);
  TEST_ASSERT(pageCache.fetchPage(2, true) != nullptr,
              "expected valid pointer");
}

template <typename T> void commonFetchPinnedFull() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  pageCache.fetchPage(1, true);
  TEST_ASSERT(pageCache.fetchPage(2, true) == nullptr, "expected null pointer");
}

template <typename T> void commonChangePageId() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(1);
  Page *page = pageCache.fetchPage(1, true);
  pageCache.changePageId(page, 2);
  TEST_ASSERT(pageCache.fetchPage(1, false) == nullptr,
              "expected null pointer");
}

template <typename T> void commonDiscardPages() {
  T pageCache(4096, 8);
  pageCache.setMaxNumPages(2);
  pageCache.fetchPage(1, true);
  pageCache.fetchPage(2, true);
  pageCache.discardPages(2);
  TEST_ASSERT(pageCache.fetchPage(1, false) != nullptr,
              "expected valid pointer");
  TEST_ASSERT(pageCache.fetchPage(2, false) == nullptr,
              "expected null pointer");
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
