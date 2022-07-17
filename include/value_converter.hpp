#pragma once
#include "aliases.hpp"
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

namespace ArgParse::Internal {

std::string invalid_value_msg(std::string_view name, std::string_view sval);

std::string incomplete_conversion_msg(std::string_view name,
                                      std::string_view sval);

template <typename T> struct ValueConverter {
  T m_value;
  OptErrMsg m_err_msg;

  ValueConverter(std::string_view name, std::string_view sval) {
    if constexpr (std::is_convertible_v<std::string, T>) {
      m_value = sval;
    } else {
      std::istringstream ins(sval.data());

      ins >> m_value;
      if (ins.fail()) {
        m_err_msg = invalid_value_msg(name, sval);
      } else if (!ins.eof()) {
        m_err_msg = incomplete_conversion_msg(name, sval);
      }
    }
  }
};
} // namespace ArgParse::Internal
