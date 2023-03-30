#include "page_cache_lru_k.hpp"
#include "test_page_cache_common.hpp"

int main() {
  commonAll<LRUKReplacementPageCache>();
  return TEST_EXIT_CODE;
}
