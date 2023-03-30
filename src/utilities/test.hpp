#ifndef CS564_PROJECT_TEST_HPP
#define CS564_PROJECT_TEST_HPP

#include <iostream>
#include <sstream>

static int testStatus = 0;
static int testExitCode = 0;
std::string testMessage;

std::ostringstream &test_message_stream() {
  static std::ostringstream s;
  return s;
}

#define TEST_EXIT_CODE testExitCode

#define TEST_ASSERT(condition, message)                                        \
  do {                                                                         \
    if (!(condition)) {                                                        \
      testStatus = 1;                                                          \
      testExitCode = 1;                                                        \
      std::ostringstream s;                                                    \
      s << __FILE__ << ':' << __LINE__ << ": " << (message);                   \
      testMessage = s.str();                                                   \
      return;                                                                  \
    }                                                                          \
  } while (false)

#define TEST_RUN(test)                                                         \
  do {                                                                         \
    testStatus = 0;                                                            \
    test();                                                                    \
    if (testStatus) {                                                          \
      std::cout << #test << " failed." << std::endl                            \
                << '\t' << testMessage << std::endl;                           \
    } else {                                                                   \
      std::cout << #test << " passed." << std::endl;                           \
    }                                                                          \
  } while (0)

#endif // CS564_PROJECT_TEST_HPP
