#include "flag.hpp"
#include "help_fmt.hpp"

namespace ArgParse {

struct FlagImpl : public Flag {
  FlagImpl(std::string_view short_name, std::string_view long_name,
           std::string_view help_msg, bool default_value)
      : m_short(short_name), m_long(long_name), m_help_msg(help_msg),
        m_is_set(default_value) {}

  std::string usage() const override {
    return Internal::flag_usage_str(m_short, m_long);
  }

  std::string help() const override {
    return Internal::flag_help_block(m_short, m_long, m_help_msg);
  }

  bool is_set() const override { return m_is_set; }

  ParseResult parse(ArgSeq &args) override {
    if (!args.empty()) {
      std::string_view next = args.front();
      if ((next == m_short) || (next == m_long)) {
        m_is_set = true;
        args.pop_front();
        return ParseResult::match();
      }
    }

    return ParseResult::no_match();
  }

private:
  const std::string m_short;
  const std::string m_long;
  const std::string m_help_msg;
  bool m_is_set;
};

Flag::Ptr Flag::create(std::string_view short_name, std::string_view long_name,
                       std::string_view help_msg, bool default_value) {
  return std::make_shared<FlagImpl>(short_name, long_name, help_msg,
                                    default_value);
}

} // namespace ArgParse