#pragma once

#include "aliases.hpp"
#include <string>

namespace ArgParse {

/**
 * @brief Describes the result of parsing a command-line option or positional
 * parameter.  This is for internal use.
 *
 */
struct ParseResult {
  ParseResult(bool matched, OptErrMsg error_msg)
      : m_matched(matched), m_error_msg(error_msg) {}

  static ParseResult no_match() { return ParseResult(false, {}); }

  static ParseResult match_with_error(std::string_view error_msg) {
    return ParseResult(true, std::string(error_msg));
  }

  static ParseResult match() { return ParseResult(true, {}); }

  bool matched() const { return m_matched; }
  OptErrMsg error_msg() const { return m_error_msg; }

private:
  const bool m_matched;
  const OptErrMsg m_error_msg;
};

} // namespace ArgParse