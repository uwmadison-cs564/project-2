#ifndef CS564_PROJECT_TEST_PAGE_CACHE_COMMON_HPP
#define CS564_PROJECT_TEST_PAGE_CACHE_COMMON_HPP

#include "dependencies/sqlite/sqlite3.hpp"
#include "page_cache.hpp"
#include "utilities/test.hpp"

#include <iostream>

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

void loadSQLiteTable() {
  sqlite::Database db("test.sqlite");
  sqlite::Connection conn;

  db.connect(conn).expect(SQLITE_OK);

  conn.execute("DROP TABLE IF EXISTS T");

  conn.execute("CREATE TABLE T (a INTEGER PRIMARY KEY, b INTEGER)")
      .expect(SQLITE_OK);

  conn.begin().expect(SQLITE_OK);

  for (int i = 0; i < 10000; ++i) {
    conn.execute("INSERT INTO T VALUES (" + std::to_string(i) + ", " +
                 std::to_string(i) + ")")
        .expect(SQLITE_OK);
  }

  conn.commit().expect(SQLITE_OK);
}

template <typename T> void commonSQLite1() {
  static T *pageCache;

  PageCacheMethods<T> pageCacheMethods;
  pageCacheMethods.xCreate = [](int pageSize, int extraSize, int) {
    pageCache = new T(pageSize, extraSize);
    return (sqlite3_pcache *)pageCache;
  };

  sqlite::shutdown().expect(SQLITE_OK);
  sqlite::config(SQLITE_CONFIG_PCACHE2, &pageCacheMethods).expect(SQLITE_OK);
  sqlite::initialize().expect(SQLITE_OK);

  sqlite::Database db("test.sqlite");
  sqlite::Connection conn;

  db.connect(conn).expect(SQLITE_OK);

  conn.execute("PRAGMA cache_size=100");

  for (int i = 0; i < 10; ++i) {
    conn.execute("SELECT SUM(b) FROM T").expect(SQLITE_OK);
  }

  std::cout << pageCache->getNumFetches() << std::endl;
  std::cout << pageCache->getNumHits() << std::endl;
}

template <typename T> void commonAll() {
  TEST_RUN(commonFetchMiss<T>);
  TEST_RUN(commonFetchTwice<T>);
  TEST_RUN(commonFetchFull<T>);
  TEST_RUN(commonFetchPinnedFull<T>);
  TEST_RUN(commonChangePageId<T>);
  TEST_RUN(commonDiscardPages<T>);
  TEST_RUN(commonNumPages<T>);

  loadSQLiteTable();

  TEST_RUN(commonSQLite1<T>);
}

#endif // CS564_PROJECT_TEST_PAGE_CACHE_COMMON_HPP
