#include "page_cache_random.hpp"

RandomReplacementPageCache::RandomReplacementPage::RandomReplacementPage(
    int argPageSize, int argExtraSize, unsigned argPageId, bool argPinned)
    : Page(argPageSize, argExtraSize), pageId(argPageId), pinned(argPinned) {}

RandomReplacementPageCache::RandomReplacementPageCache(int pageSize,
                                                       int extraSize)
    : PageCache(pageSize, extraSize), randomGenerator_(std::random_device()()) {
}

RandomReplacementPageCache::~RandomReplacementPageCache() {
  for (auto &[pageId, page] : pages_) {
    free(page);
  }
}

void RandomReplacementPageCache::setMaxNumPages(int maxNumPages) {
  maxNumPages_ = maxNumPages;

  // Discard unpinned pages until the number of pages in the cache is less than
  // or equal to `maxNumPages_` or only pinned pages remain.
  for (auto pagesIterator = pages_.begin(); pagesIterator != pages_.end();) {
    if (getNumPages() <= maxNumPages_) {
      break;
    }

    if (!pagesIterator->second->pinned) {
      delete pagesIterator->second;
      pagesIterator = pages_.erase(pagesIterator);
    } else {
      ++pagesIterator;
    }
  }
}

int RandomReplacementPageCache::getNumPages() const {
  return (int)pages_.size();
}

Page *RandomReplacementPageCache::fetchPage(unsigned pageId, bool allocate) {
  ++numFetches_;

  // If the page is already in the cache, pin it and return the pointer.
  auto pagesIterator = pages_.find(pageId);
  if (pagesIterator != pages_.end()) {
    ++numHits_;
    pagesIterator->second->pinned = true;
    return pagesIterator->second;
  }

  // The page is not already in the cache. If parameter `allocate` is false,
  // return a null pointer.
  if (!allocate) {
    return nullptr;
  }

  // Parameter `allocate` is true. If the number of pages in the cache is less
  // than the maximum, allocate and return a pointer to a new page.
  if (getNumPages() < maxNumPages_) {
    auto page = new RandomReplacementPage(pageSize_, extraSize_, pageId, true);
    pages_.emplace(pageId, page);
    return page;
  }

  // The number of pages in the cache is greater than or equal to the maximum.
  // Choose an existing unpinned page to replace. Initialize an iterator at a
  // random page in `pages_`.
  pagesIterator = pages_.begin();
  std::advance(pagesIterator, std::uniform_int_distribution<int>(
                                  0, (int)getNumPages() - 1)(randomGenerator_));

  // Check each successive page in `pages_` to see if it is unpinned. If an
  // unpinned page is found, pin it and return the pointer.
  for (int iteration = 0; iteration < getNumPages(); ++iteration) {
    if (!pagesIterator->second->pinned) {
      // Replace the page ID in `pages_`.
      RandomReplacementPage *page = pagesIterator->second;
      pages_.erase(pagesIterator);
      pages_.emplace(pageId, page);
      return page;
    }

    // Advance the iterator, potentially wrapping around to the beginning.
    if (++pagesIterator == pages_.end()) {
      pagesIterator = pages_.begin();
    }
  }

  // All pages are pinned. Return a null pointer.
  return nullptr;
}

void RandomReplacementPageCache::unpinPage(Page *pageBase, bool discard) {
  auto *page = (RandomReplacementPage *)pageBase;

  // If discard is true or the number of pages in the cache is greater than the
  // maximum, discard the page. Otherwise, unpin the page.
  if (discard || getNumPages() > maxNumPages_) {
    pages_.erase(page->pageId);
    delete page;
  } else {
    page->pinned = false;
  }
}

void RandomReplacementPageCache::changePageId(Page *pageBase,
                                              unsigned newPageId) {
  auto *page = (RandomReplacementPage *)pageBase;

  // Remove the old page ID from `pages_` and change the page ID.
  pages_.erase(page->pageId);
  page->pageId = newPageId;

  // Attempt to insert a page with page ID `newPageId` into `pages_`.
  auto [pagesIterator, success] = pages_.emplace(newPageId, page);

  // If a page with page ID `newPageId` is already in the cache, discard it.
  if (!success) {
    delete pagesIterator->second;
    pagesIterator->second = page;
  }
}

void RandomReplacementPageCache::discardPages(unsigned pageIdLimit) {
  // Discard all pages with page ID greater than or equal to `pageIdLimit`.
  for (auto pagesIterator = pages_.begin(); pagesIterator != pages_.end();) {
    if (pagesIterator->second->pageId >= pageIdLimit) {
      delete pagesIterator->second;
      pagesIterator = pages_.erase(pagesIterator);
    } else {
      ++pagesIterator;
    }
  }
}
