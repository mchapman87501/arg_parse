#pragma once

#include "aliases.hpp"
#include <string>
#include <utility>

namespace ArgParse {

/**
 * @brief Describes the result of parsing a command-line option or positional
 * parameter.  This is for internal use.
 *
 */
struct ParseResult {
  ParseResult(bool matched, OptErrMsg error_msg)
      : m_matched(matched), m_error_msg(std::move(error_msg)) {}

  static ParseResult no_match() { return {false, {}}; }

  static ParseResult match_with_error(std::string_view error_msg) {
    return {true, std::string(error_msg)};
  }

  static ParseResult match() { return {true, {}}; }

  [[nodiscard]] bool matched() const { return m_matched; }
  [[nodiscard]] OptErrMsg error_msg() const { return m_error_msg; }

private:
  const bool m_matched;
  const OptErrMsg m_error_msg;
};

} // namespace ArgParse