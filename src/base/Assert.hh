#pragma once

#include <source_location>


namespace TC {

  void Expects(bool v, const std::source_location loc = std::source_location::current());
  void Ensures(bool v, const std::source_location loc = std::source_location::current());

}  // namespace TC
