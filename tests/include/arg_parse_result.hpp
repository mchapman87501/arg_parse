#pragma once

#include "aliases.hpp"
#include "argument_parser.hpp"

#include "redirect.hpp"
#include <iostream>
#include <string>

namespace Tests {
class ArgParseResult {
  const bool m_should_exit;
  bool m_actual_exit;

  const int m_expected_code;
  int m_actual_code;

  const bool m_verbose;
  std::string m_cout;
  std::string m_cerr;

public:
  ArgParseResult(const ArgParse::ArgumentParser::Ptr parser,
                 const ArgParse::ArgSeq &args, bool should_exit,
                 int expected_code, bool verbose = false);

  // Answer whether condition() == expected; dump diagnostics if not.
  [[nodiscard]] bool check_outcome() const;

  [[nodiscard]] std::string_view cout() const { return m_cout; }
  [[nodiscard]] std::string_view cerr() const { return m_cerr; }

  [[nodiscard]] bool cout_contains(std::string_view substr) const;
  [[nodiscard]] bool cerr_contains(std::string_view substr) const;
};
} // namespace Tests