#include "arg_parse_result.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace Tests {
namespace {
std::string bool_str(bool v) { return v ? "true" : "false"; }

void show_output(std::string_view cout_str, std::string_view cerr_str) {
  std::cerr << "Parser cout:" << std::endl
            << cout_str << "Parser cerr: " << std::endl
            << cerr_str << std::flush;
}
} // namespace

ArgParseResult::ArgParseResult(const ArgParse::ArgumentParser::Ptr parser,
                               const ArgParse::ArgSeq &args, bool should_exit,
                               int expected_code)
    : m_should_exit(should_exit), m_expected_code(expected_code) {
  std::ostringstream couts;
  std::ostringstream cerrs;

  {
    Redirect cout_capture(couts, std::cout);

    Redirect cerr_capture(cerrs, std::cerr);

    parser->parse_args(args);
    m_actual_exit = parser->should_exit();
    m_actual_code = parser->exit_code();
  }
  m_cout = couts.str();
  m_cerr = cerrs.str();
  check_outcome();
}

bool ArgParseResult::check_outcome() const {
  bool result = m_actual_exit == m_should_exit;
  if (m_should_exit) {
    result = result && (m_actual_code == m_expected_code);
  }

  if (!result) {
    std::cerr << "Unexpected parser result." << std::endl;
    if (m_should_exit != m_actual_exit) {
      std::cerr << "Expected parser->should_exit() == "
                << bool_str(m_should_exit) << ", got "
                << bool_str(m_actual_exit) << std::endl;
      if (m_should_exit && (m_expected_code != m_actual_code)) {
        std::cerr << "Expected parser->exit_code() == " << m_expected_code
                  << ", got " << m_actual_code << std::endl;
      }
    }
  }

  if (!result) {
    show_output(m_cout, m_cerr);
  }

  return result;
}

bool ArgParseResult::cout_contains(std::string_view substr) const {
  const bool result = m_cout.find(substr) != std::string::npos;
  if (!result) {
    show_output(m_cout, m_cerr);
  }
  return result;
}

bool ArgParseResult::cerr_contains(std::string_view substr) const {
  const bool result = m_cerr.find(substr) != std::string::npos;
  if (!result) {
    show_output(m_cout, m_cerr);
  }
  return result;
}

} // namespace Tests