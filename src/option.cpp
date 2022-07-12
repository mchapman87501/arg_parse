#include "option.hpp"
#include "help_fmt.hpp"

namespace ArgParse {

using namespace std;

namespace {
struct ParsedOptVal {
  static ParsedOptVal no_match() { return ParsedOptVal(false, "", {}); }

  static ParsedOptVal no_value_provided(string_view option_arg) {
    string error_msg("No value provided: '" + string(option_arg) + "'");
    return ParsedOptVal(true, "", string(error_msg));
  }

  static ParsedOptVal match(string_view strval) {
    return ParsedOptVal(true, strval, {});
  }

  bool matched() const { return m_parse_result.matched(); }
  bool has_error() const { return m_parse_result.error_msg().has_value(); }
  ParseResult as_parse_result() const { return m_parse_result; }
  string_view value() const { return m_strval; }

private:
  ParseResult m_parse_result;
  const string m_strval;

  ParsedOptVal(bool matched, string_view strval, OptErrMsg error_msg)
      : m_parse_result(matched, error_msg), m_strval(strval) {}
};

ParsedOptVal get_long_eq_strval(string_view long_name, string_view opt_arg) {
  const string long_eq(long_name.data() + string("="));

  if (opt_arg.starts_with(long_eq)) {
    const string value_str = string(opt_arg.substr(long_eq.size()));
    return value_str.empty() ? ParsedOptVal::no_value_provided(opt_arg)
                             : ParsedOptVal::match(value_str);
  }
  return ParsedOptVal::no_match();
}

ParsedOptVal get_opt_strval(string_view long_name, string_view opt_arg,
                            ArgSeq &remaining) {
  auto result = get_long_eq_strval(long_name, opt_arg);
  if (result.matched()) {
    return result;
  }

  // It wasn't a "--long_opt=<value>".
  if (remaining.empty()) {
    return ParsedOptVal::no_value_provided(opt_arg);
  }

  string strval = remaining.front().data();
  remaining.pop_front();
  return ParsedOptVal::match(strval);
}
} // anonymous namespace

namespace Internal {
ParseResult parse_and_set(string_view short_name, string_view long_name,
                          Setter set_from_str, ArgSeq &args) {
  bool matched = (!args.empty() && ((args.front() == short_name) ||
                                    (args.front().starts_with(long_name))));
  if (matched) {
    string_view opt(args.front());
    args.pop_front();
    auto opt_sval = get_opt_strval(long_name, opt, args);
    if (opt_sval.has_error()) {
      return opt_sval.as_parse_result();
    } else {
      auto err_msg = set_from_str(opt, opt_sval.value());
      return err_msg ? ParseResult::match_with_error(err_msg.value())
                     : ParseResult::match();
    }
  }
  return ParseResult::no_match();
}
} // namespace Internal

} // namespace ArgParse
