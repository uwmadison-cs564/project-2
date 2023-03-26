#include "page_cache.hpp"
#include "utilities/exceptions.hpp"

void RequestLog::logRequest(unsigned key, bool hit) {
  requests_.emplace_back(key, hit);
}

const std::vector<std::pair<unsigned, bool>> &RequestLog::getRequests() const {
  return requests_;
}

Page::Page(int pageSize, int extraSize, unsigned key) : sqlite3_pcache_page() {
  pBuf = std::aligned_alloc(8, pageSize);
  pExtra = std::aligned_alloc(8, extraSize);
  key_ = key;
}

Page::~Page() {
  free(pBuf);
  free(pExtra);
}

unsigned Page::getKey() const { return key_; }

PageCache::PageCache(int pageSize, int extraSize) {}

int PageCache::getPageCount() const {
  // TODO: Implement.
  throw NotImplementedException("getPageCount not implemented");
}

Page *PageCache::request(unsigned key, bool allocate) {
  // TODO: Implement.
  throw NotImplementedException("request not implemented");
}

void PageCache::release(Page *page, bool discard) {
  // TODO: Implement.
  throw NotImplementedException("release not implemented");
}

void PageCache::rekey(Page *page, unsigned newKey) {
  // TODO: Implement.
  throw NotImplementedException("rekey not implemented");
}

void PageCache::truncate(unsigned limit) {
  // TODO: Implement.
  throw NotImplementedException("truncate not implemented");
}
