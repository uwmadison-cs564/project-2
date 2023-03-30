#ifndef CS564_PROJECT_EXCEPTION_HPP
#define CS564_PROJECT_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class NotImplementedException : public std::logic_error {
public:
  explicit NotImplementedException(const std::string &what);
};

#endif // CS564_PROJECT_EXCEPTION_HPP
