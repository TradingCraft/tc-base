/** \file Assert.cc
 * Definitions for Debugging support.
 *
 *
 *
 */

#include <stdexcept>
#include <sstream>
#include "Assert.hh"


namespace TC {

void Expects(bool v, const std::source_location loc)
{
  if(!v) {
    std::ostringstream os;
    os << "Expect failure at " << loc.file_name() << '(' << loc.line() << ':' << loc.column() << ") in " << loc.function_name();
    throw std::runtime_error(os.str());
  }
}

void Ensures(bool v, const std::source_location loc)
{
  if(!v) {
    std::ostringstream os;
    os << "Ensure failure at " << loc.file_name() << '(' << loc.line() << ':' << loc.column() << ") in " << loc.function_name();
    throw std::runtime_error(os.str());
  }
}

}  // namespace TC


