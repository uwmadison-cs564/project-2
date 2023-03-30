# CS 564 Project 2: SQLite Page Cache

## Overview

Many database management systems (DBMSs) are designed to handle databases that are too large to fit entirely in main memory. The *buffer manager* is the component of a DBMS responsible for transferring pages between disk and memory as needed. The buffer manager also decides which pages should be replaced when the buffer pool is full. Because the buffer manager handles these details, higher-level DBMS code can be written without worrying about whether pages are in memory or not.

In this project, you will be customizing the buffer manager of SQLite. SQLite is the most widely deployed DBMS in existence. It is found on nearly every smartphone, computer, internet browser, and automobile. There are likely over 1 trillion SQLite databases in active use. Specifically, you will extend SQLite with two custom page replacement algorithms.

SQLite's buffer manager is known as the *pager*. An important component of the pager is the *page cache*. Whenever the pager needs a page, it first checks the page cache to see if the page is already in memory. If the page is already in memory, the page cache provides a pointer to the page. If the page is not in memory, the page cache allocates space for a new page by either allocating memory or replacing an existing page.

SQLite allows you to design your own custom page cache by implementing a small set of [C functions](https://sqlite.org/c3ref/pcache_methods2.html). SQLite's page cache API has a few differences from a typical buffer manager. First, the page cache is allowed to grow and shrink as needed. At any time, SQLite may set the maximum number of pages in the page cache. While this constraint is more of a guideline than a rule, the page cache should try to honor it. Second, the page cache does not maintain a pin count for pages. When a page is unpinned, it is safe to discard, regardless of the number of prior requests. Third, SQLite may change the page ID associated with a page. If a different page with the new page ID is already in the cache, it is discarded. Finally, SQLite may instruct the page cache to discard all pages in a certain range of page IDs.

## Page cache functions

To make things easier for you, we have written a C++ wrapper around SQLite's page cache API. To explore the C++ wrapper, begin by examining `page_cache.hpp`. This header file contains definitions for the `Page` and `PageCache` classes. The `Page` class is a small wrapper around the SQLite struct `sqlite3_pcache_page` that makes it easier to allocate and deallocate pages. The `PageCache` class is an abstract base class that you will extend as you implement your page replacement policies.

For each page replacement policy, you will implement the functions in `PageCache` that are marked `virtual`. The logic you should implement is as follows.

### Set the maximum number of pages in the cache

```cpp
void setMaxNumPages(int maxNumPages)
```

Discard unpinned pages until either the number of pages in the cache is less than or equal to `maxNumPages` or all the pages in the cache are pinned. If there are still too many pages after discarding all unpinned pages, pages will continue to be discarded after being unpinned in the `unpinPage` function.

### Get the number of pages in the cache, both pinned and unpinned

```cpp
int getNumPages() const
```

### Fetch and pin a page

```cpp
Page *fetchPage(unsigned pageId, bool allocate)
```

- If the page is already in the cache, return a pointer to the page.
- If the page is not already in the cache, use the `allocate` parameter to determine how to proceed.
	- If `allocate` is false, return a null pointer.
	- If `allocate` is true, examine the number of pages in the cache.
		- If the number of pages in the cache is less than the maximum, allocate and return a pointer to a new page.
		- If the number of pages in the cache is greater than or equal to the maximum, try to replace a page.
			- If there is at least one unpinned page, return a pointer to an existing unpinned page as determined by the replacement policy.
			- If all pages are pinned, return a null pointer.

This function should be $O(1)$ or $O(\log n)$ with respect to the number of pages in the cache.

### Unpin a page

```cpp
void unpinPage(Page *page, bool discard)
```

The page is unpinned regardless of the number of prior fetches, meaning it can be safely discarded.

- If `discard` is true, then discard the page.
- If `discard` is false, examine the number of pages in the cache.
	- If the number of pages in the cache is greater than or equal to the maximum, discard the page.
	- If the number of pages in the cache is less than the maximum, do not discard the page.

This function should be $O(1)$ or $O(\log n)$ with respect to the number of pages in the cache.

### Change the page ID associated with a page

```cpp
void changePageId(Page *page, unsigned newPageId)
```

If a page with page ID `newPageId` is already in the cache, it is assumed that the page is unpinned, and the page is discarded.

### Discard all pages with page IDs greater that or equal to a limit

```cpp
void discardPages(unsigned pageIdLimit)
```

If any of these pages are pinned, then they are implicitly unpinned, meaning they can be safely discarded.

## Page replacement policies

You will implement two page replacement policies: **LRU** and **LRU-2**. We talked about how to implement LRU in class. LRU-K is a generalization of LRU that replaces the page whose K-th most recent access is the least recent. The advantage of LRU-K over LRU is that LRU-K considers both the frequency *and* recency of a page reference, whereas LRU considers only the recency. If you are interested in reading more about LRU-K, you can check out the [paper](https://www.cs.cmu.edu/~natassa/courses/15-721/papers/p297-o_neil.pdf). You will implement LRU-2. Specifically, your page replacement policy will replace the page whose second-to-last access is furthest in the past.

In the code repository, we provide class stubs for your page cache implementations. In addition, we provide an example page cache implementation, which uses a random page replacement policy, in `page_cache_random.cpp` and `page_cache_random.hpp`. You can use this as a guide for your own implementations.

## Building

Ensure you have the dependencies installed. To build the project, you only need CMake version 3.4 or newer and a C++ 17 enabled compiler. You can verify your CMake installation by running:

```bash
cmake --version
```

If CMake is installed on your system, you'll see output similar to `cmake version 3.x.x`. If you don't have CMake, follow these [installation instructions](https://cmake.org/install).

Clone the repository to your computer. If you're unfamiliar with GitHub, you can follow this [quick tutorial](https://docs.github.com/en/get-started/quickstart). Replace `URL` below with the GitHub URL of the repository.

```bash
git clone URL
```

Alternatively, if you plan to host your code on GitHub, you'll need to duplicate the repository into your own **private** repository. To do so, follow these [instructions](https://docs.github.com/en/repositories/creating-and-managing-repositories/duplicating-a-repository).

Then, navigate into the project directory.

```bash
cd project
```

Create and navigate into a build directory.

```bash
mkdir build && cd build
```

Generate the build files.

```bash
cmake ..
```

Finally, build the project.

```bash
cmake --build .
```

Optionally, run the tests. Some tests will fail if you have just started the project.

```bash
ctest .
```

## Developing

If you decide to host your code on GitHub, **use a private repository**. You should not change the directory structure of the project. The only files you may modify are:
- `page_cache_lru.cpp/hpp`
- `page_cache_lru_2.cpp/hpp`

### Tests

For each page cache implementation, there is a test executable in the subdirectory `test`. As of now, the only tests that are written are those in `test_page_cache_common.hpp`. These are general tests that are agnostic to the page replacement policy. Passing these tests is your first milestone.

As the project progresses, we will add additional tests to the code repository. Some of these tests will be specific to the page replacement policy (*e.g.*, ensuring that LRU replaces the least recently used page). Other tests will connect your page cache to SQLite and run SQL queries.

### Style

You are expected to develop your code using good C++ style. You don't have to follow any specific convention, but ensure that your code is consistent, organized, clear, and well-documented.

## Connecting to SQLite

You may have noticed that we haven't talked much about how your page cache implementations will interface with SQLite. Soon we will provide more information on how you can register a page cache with SQLite and evaluate its performance by running real SQL queries. For now, focus on completing the logic of your page cache implementations.

## Deliverables

Submit a zipped archive (`.zip`) of the top-level project directory to Canvas. Do not include build files in your submission. There should only be one submission per group. Points may be deducted if you do not follow these instructions.
