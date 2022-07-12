#pragma once

namespace ArgParse {

/**
 * @brief Represents the number of command-line arguments which an IArgument can
 * accept.
 *
 */
enum class Nargs {
  /// Exactly one
  one = 1,
  /// Zero or more
  zero_or_more = 0,
  /// At least one
  one_or_more = 2
};

} // namespace ArgParse
