#include "exceptions.hpp"

NotImplementedException::NotImplementedException(const std::string &what)
    : std::logic_error(what) {}
