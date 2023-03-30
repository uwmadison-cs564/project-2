#include "page_cache_lru.hpp"
#include "test_page_cache_common.hpp"

int main() {
  commonAll<LRUReplacementPageCache>();
  return TEST_EXIT_CODE;
}
