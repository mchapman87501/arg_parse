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

  std::string m_cout;
  std::string m_cerr;

  // Answer whether condition() == expected; dump diagnostics if not.
  bool check_outcome() const;

public:
  ArgParseResult(const ArgParse::ArgumentParser::Ptr parser,
                 const ArgParse::ArgSeq &args, bool should_exit,
                 int expected_code);

  std::string_view cout() const { return m_cout; }
  std::string_view cerr() const { return m_cerr; }

  bool cout_contains(std::string_view substr) const;
  bool cerr_contains(std::string_view substr) const;
};
} // namespace Tests