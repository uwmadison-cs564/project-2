#ifndef SQLITE_EDU_EXCEPTIONS_HPP
#define SQLITE_EDU_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

class NotImplementedException : public std::logic_error {
public:
  explicit NotImplementedException(const std::string &what);
};

#endif // SQLITE_EDU_EXCEPTIONS_HPP
