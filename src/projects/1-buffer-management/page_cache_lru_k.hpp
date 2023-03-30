#ifndef CS564_PROJECT_PAGE_CACHE_LRU_K_HPP
#define CS564_PROJECT_PAGE_CACHE_LRU_K_HPP

#include "page_cache.hpp"

class LRUKReplacementPageCache : public PageCache {
public:
  LRUKReplacementPageCache(int pageSize, int extraSize);

  void setMaxNumPages(int maxNumPages) override;

  [[nodiscard]] int getNumPages() const override;

  Page *fetchPage(unsigned int pageId, bool allocate) override;

  void unpinPage(Page *page, bool discard) override;

  void changePageId(Page *page, unsigned int newPageId) override;

  void discardPages(unsigned int pageIdLimit) override;

private:
  // TODO: Declare class members as needed.
};

#endif // CS564_PROJECT_PAGE_CACHE_LRU_K_HPP
