#ifndef CS564_PROJECT_TEST_PAGE_CACHE_COMMON_HPP
#define CS564_PROJECT_TEST_PAGE_CACHE_COMMON_HPP

#include "dependencies/sqlite/sqlite3.hpp"
#include "page_cache.hpp"
#include "utilities/test.hpp"

#include <random>

static int numRows = 10000;

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

void loadSQLiteDatabase() {
  sqlite::Database db("test.sqlite");
  sqlite::Connection conn;

  db.connect(conn).expect(SQLITE_OK);

  conn.execute("DROP TABLE IF EXISTS T");

  conn.execute("CREATE TABLE T (a INTEGER PRIMARY KEY, b INTEGER)")
      .expect(SQLITE_OK);

  conn.begin().expect(SQLITE_OK);

  for (int i = 0; i < numRows; ++i) {
    conn.execute("INSERT INTO T VALUES (" + std::to_string(i) + ", " +
                 std::to_string(i) + ")")
        .expect(SQLITE_OK);
  }

  conn.commit().expect(SQLITE_OK);
}

template <typename T, typename F>
void commonSQLRun(F &&f, unsigned long long &numFetches,
                  unsigned long long &numHits) {
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

  conn.execute("PRAGMA cache_size=10");

  f(conn);

  numFetches = pageCache->getNumFetches();
  numHits = pageCache->getNumHits();
}

template <typename T>
void commonSQLScan(unsigned long long &numFetches,
                   unsigned long long &numHits) {
  commonSQLRun<T>(
      [&](sqlite::Connection &conn) {
        for (int i = 0; i < 10; ++i) {
          conn.execute("SELECT SUM(b) FROM T").expect(SQLITE_OK);
        }
      },
      numFetches, numHits);
}

template <typename T>
void commonSQLScanWithHotSet(unsigned long long &numFetches,
                             unsigned long long &numHits) {
  commonSQLRun<T>(
      [&](sqlite::Connection &conn) {
        for (int i = 0; i < 10; ++i) {
          conn.execute("SELECT SUM(b) FROM T").expect(SQLITE_OK);
          conn.execute("SELECT SUM(b) FROM T WHERE a < 3000").expect(SQLITE_OK);
        }
      },
      numFetches, numHits);
}

template <typename T>
void commonSQLUniformRandom(unsigned long long &numFetches,
                            unsigned long long &numHits) {
  std::minstd_rand rng(0); // NOLINT(cert-msc51-cpp)
  std::uniform_int_distribution<int> dis(0, numRows - 1);

  commonSQLRun<T>(
      [&](sqlite::Connection &conn) {
        for (int i = 0; i < 100; ++i) {
          conn.execute("SELECT b FROM T WHERE a = " + std::to_string(dis(rng)))
              .expect(SQLITE_OK);
        }
      },
      numFetches, numHits);
}

template <typename T>
void commonSQLBinomialRandom(unsigned long long &numFetches,
                             unsigned long long &numHits) {
  std::minstd_rand rng(0); // NOLINT(cert-msc51-cpp)
  std::binomial_distribution<int> dis(numRows - 1, 0.9);

  commonSQLRun<T>(
      [&](sqlite::Connection &conn) {
        for (int i = 0; i < 100; ++i) {
          conn.execute("SELECT b FROM T WHERE a = " + std::to_string(dis(rng)))
              .expect(SQLITE_OK);
        }
      },
      numFetches, numHits);
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
