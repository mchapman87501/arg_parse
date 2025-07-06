#include "help_fmt.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace ArgParse::Internal {

using namespace std;

string to_upper(string_view s) {
  string result(s);
  transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return toupper(c); });
  return result;
}

string value_name(string_view option_name) {
  string result = option_name.data();
  const auto after_dash = result.find_first_not_of("-");
  if (after_dash != string::npos) {
    result = result.substr(after_dash);
  }
  return to_upper(result);
}

string usage_str(string_view short_name, string_view long_name) {
  if (short_name == long_name) {
    return string(short_name);
  }
  return string(short_name) + "|" + string(long_name);
}

string bracketed(string_view s) { return string("[") + s.data() + "]"; }

string flag_help_title(string_view short_name, string_view long_name) {
  return usage_str(short_name, long_name);
}

string option_help_title(string_view short_name, string_view long_name) {
  return usage_str(short_name, long_name) + " " + value_name(long_name);
}

string flag_usage_str(string_view short_name, string_view long_name) {
  return bracketed(flag_help_title(short_name, long_name));
}

string option_usage_str(string_view short_name, string_view long_name) {
  return bracketed(option_help_title(short_name, long_name));
}

string arg_usage_str(string_view arg_name, Nargs nargs) {
  // TODO handle nargs specification.
  const string placeholder_name(to_upper(arg_name));
  switch (nargs) {
  case Nargs::one:
    return placeholder_name;
  case Nargs::zero_or_more:
    return "[" + placeholder_name + " ...]";
  case Nargs::one_or_more:
    return placeholder_name + " [" + placeholder_name + " ...]";
  }
  return placeholder_name;
}

string help_block(string_view usage_label, string_view help_msg) {
  ostringstream outs;
  outs << "    " << usage_label << endl << "        " << help_msg;
  return outs.str();
}

string flag_help_block(string_view short_name, string_view long_name,
                       string_view help_msg) {
  return help_block(flag_help_title(short_name, long_name), help_msg);
}

string option_help_block(string_view short_name, string_view long_name,
                         string_view help_msg) {
  return help_block(option_help_title(short_name, long_name), help_msg);
}

string arg_help_block(string_view arg_name, Nargs nargs, string_view help_msg) {
  return help_block(arg_usage_str(arg_name, nargs), help_msg);
}

} // namespace ArgParse::Internal