#include "exception.hpp"

NotImplementedException::NotImplementedException(const std::string &what)
    : std::logic_error("not implemented: " + what) {}
