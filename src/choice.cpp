#include "choice.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

namespace {
auto lowercase(std::string_view sval) {
  std::string result(sval);
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return result;
}
} // namespace

namespace ArgParse {
struct ChoiceImpl : public Choice {
  ChoiceImpl(std::string_view short_name, std::string_view long_name,
             std::string_view help_msg,
             const std::vector<std::string> &valid_choices)
      : Choice(short_name, long_name, help_msg),
        m_valid_choices(valid_choices) {

    if (valid_choices.empty()) {
      throw std::invalid_argument("At least one choice must be specified.");
    }
  }

  std::string help() const override {
    std::ostringstream outs;
    outs << "  Valid values (case-insensitive): (";
  std:;
    std::string sep = "";
    for (const auto choice : m_valid_choices) {
      outs << sep << "'" << choice << "'";
      sep = ", ";
    }
    outs << ")";

    return Internal::option_help_block(m_short, m_long,
                                       m_help_msg + outs.str());
  }

protected:
  bool valid_value(const std::string &v) override {
    const std::string v_lcase = lowercase(v);
    for (const auto &curr_choice : m_valid_choices) {
      const std::string curr_lcase = lowercase(curr_choice);
      if (curr_lcase == v_lcase) {
        return true;
      }
    }
    return false;
  }

private:
  std::vector<std::string> m_valid_choices;
};

Choice::Ptr Choice::create(std::string_view short_name,
                           std::string_view long_name,
                           std::string_view help_msg,
                           const std::vector<std::string> &valid_choices) {
  return Choice::Ptr(
      new ChoiceImpl(short_name, long_name, help_msg, valid_choices));
};
} // namespace ArgParse