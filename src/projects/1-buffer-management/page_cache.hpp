#ifndef SQLITE_EDU_PAGE_CACHE_HPP
#define SQLITE_EDU_PAGE_CACHE_HPP

#include "dependencies/sqlite/sqlite3.h"

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

class RequestLog {
public:
  /**
   * Log the result of a request.
   * @param key Key of the requested page.
   * @param hit True if the request was a hit and false otherwise.
   */
  void logRequest(unsigned key, bool hit);

  /**
   * Get the requests logged since construction.
   * @return List of requests.
   */
  [[nodiscard]] const std::vector<std::pair<unsigned, bool>> &
  getRequests() const;

private:
  std::vector<std::pair<unsigned, bool>> requests_;
};

class Page : public sqlite3_pcache_page {
public:
  /**
   * Construct a Page.
   * @param pageSize Size in bytes of the page.
   * @param extraSize Size in bytes of the buffer to store extra information.
   * @param key Key of the page.
   */
  Page(int pageSize, int extraSize, unsigned key);

  ~Page();

  Page(const Page &) = delete;

  Page &operator=(const Page &other) = delete;

  /**
   * Get the key of the page.
   * @return Key of the page.
   */
  [[nodiscard]] unsigned getKey() const;

private:
  unsigned key_;
};

class PageCache {
public:
  /**
   * Construct a PageCache.
   * @param pageSize Page size in bytes. Assumed to be a power of two.
   * @param extraSize Extra space in bytes. Assumed to be less than 250.
   */
  PageCache(int pageSize, int extraSize);

  /**
   * Get the number of pages in the cache, both pinned and unpinned.
   * @return Number of pages in the cache.
   */
  [[nodiscard]] int getPageCount() const;

  /**
   * Request and pin a page. If the page is already in the cache, return a
   * pointer to the page. If the page is not already in the cache, use the
   * allocate parameter to determine how to proceed. If allocate is true,
   * allocate and return a pointer to a new page. If allocate is false, do not
   * allocate a new page and return nullptr. Log the result of the request.
   * @param key Key of the requested page.
   * @param allocate Allocate a new page on a miss.
   * @return Pointer to the requested page.
   */
  Page *request(unsigned key, bool allocate);

  /**
   * Release a page. The page is unpinned regardless of the number of prior
   * requests, meaning it can be safely evicted. If evict is true, the page is
   * evicted. If evict is false, the page need not be evicted.
   * @param page Pointer to the page to be released.
   * @param evict Evict the page.
   */
  void release(Page *page, bool evict);

  /**
   * Change the key value associated with a page. If a page with key newKey is
   * already in the cache, it is evicted.
   * @param page Pointer to the page to be rekeyed.
   * @param newKey New key.
   */
  void rekey(Page *page, unsigned newKey);

  /**
   * Evict all pages with keys greater than or equal to limit. If any of these
   * pages are pinned, then they are implicitly unpinned, meaning they can be
   * safely evicted.
   * @param limit Evict all pages with keys greater than or equal to this value.
   */
  void truncate(unsigned limit);

private:
  std::shared_ptr<RequestLog> requestLog_;
};

#endif // SQLITE_EDU_PAGE_CACHE_HPP
