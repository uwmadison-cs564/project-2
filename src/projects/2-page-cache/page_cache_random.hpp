#ifndef CS564_PROJECT_PAGE_CACHE_RANDOM_HPP
#define CS564_PROJECT_PAGE_CACHE_RANDOM_HPP

#include "page_cache.hpp"

#include <random>
#include <unordered_map>

class RandomReplacementPageCache : public PageCache {
public:
  RandomReplacementPageCache(int pageSize, int extraSize);

  ~RandomReplacementPageCache() override;

  void setMaxNumPages(int maxNumPages) override;

  [[nodiscard]] int getNumPages() const override;

  Page *fetchPage(unsigned pageId, bool allocate) override;

  void unpinPage(Page *page, bool discard) override;

  void changePageId(Page *page, unsigned newPageId) override;

  void discardPages(unsigned pageIdLimit) override;

private:
  struct RandomReplacementPage : public Page {
    RandomReplacementPage(int pageSize, int extraSize, unsigned pageId,
                          bool pinned);

    unsigned pageId;
    bool pinned;
  };

  std::unordered_map<unsigned, RandomReplacementPage *> pages_;
  std::minstd_rand randomGenerator_;
};

#endif // CS564_PROJECT_PAGE_CACHE_RANDOM_HPP
