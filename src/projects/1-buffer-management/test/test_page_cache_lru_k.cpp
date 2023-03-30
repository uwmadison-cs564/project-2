#include "page_cache_lru_2.hpp"
#include "test_page_cache_common.hpp"

int main() {
  commonAll<LRU2ReplacementPageCache>();
  return TEST_EXIT_CODE;
}
