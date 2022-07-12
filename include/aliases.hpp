#pragma once

#include <deque>
#include <optional>
#include <string_view>

namespace ArgParse {
/// A sequence of command-line arguments.  The first argument is
/// the name by which the executable was invoked.
using ArgSeq = std::deque<std::string_view>;

/// An optional error message
using OptErrMsg = std::optional<std::string>;
} // namespace ArgParse
