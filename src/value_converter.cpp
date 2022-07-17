#include "value_converter.hpp"

namespace ArgParse::Internal {
std::string invalid_value_msg(std::string_view name, std::string_view sval) {
  return std::string("Invalid value for '") + name.data() + "': '" +
         sval.data() + "'.";
}

std::string incomplete_conversion_msg(std::string_view name,
                                      std::string_view sval) {
  return std::string("Could not completely convert value for '") + name.data() +
         "': '" + sval.data() + "'.";
}

} // namespace ArgParse::Internal