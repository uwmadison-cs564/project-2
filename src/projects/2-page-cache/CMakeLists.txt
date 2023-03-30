add_library(
        page_cache
        page_cache.cpp
        page_cache.hpp
        page_cache_lru.cpp
        page_cache_lru.hpp
        page_cache_lru_2.cpp
        page_cache_lru_2.hpp
        page_cache_random.cpp
        page_cache_random.hpp
)

target_include_directories(
        page_cache
        PRIVATE
        ../..
)

target_link_libraries(
        page_cache
        utilities
)

add_subdirectory(test)
