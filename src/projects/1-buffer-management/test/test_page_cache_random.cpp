#include "page_cache_random.hpp"
#include "test_page_cache_common.hpp"

int main() {
  commonAll<RandomReplacementPageCache>();
  return TEST_EXIT_CODE;
}
