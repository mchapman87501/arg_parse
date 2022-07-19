#pragma once

#include "help_fmt.hpp"
#include "i_option.hpp"
#include "value_converter.hpp"
#include <functional>
#include <iostream>
#include <sstream>

namespace ArgParse {

namespace Internal {
using Setter = std::function<OptErrMsg(std::string_view opt_name,
                                       std::string_view opt_sval)>;
ParseResult parse_and_set(std::string_view short_name,
                          std::string_view long_name, Setter setter,
                          ArgSeq &args);

} // namespace Internal

/**
 * @brief Represents a command-line option with an associated value.
 *
 * @tparam T The type of the value for this option spec.
 */
template <typename T> struct Option : public IOption {
  using Ptr = std::shared_ptr<Option<T>>;

  /**
   * @brief Create a new command-line option spec.
   *
   * @param short_name The short name of the option, e.g., "-o"
   * @param long_name The long name of the option, e.g., "--output"
   * @param help_msg A description of the purpose of this option
   * @return Ptr A pointer to the created instance
   */
  static Ptr create(std::string_view short_name, std::string_view long_name,
                    std::string_view help_msg) {
    return std::shared_ptr<Option>(new Option(short_name, long_name, help_msg));
  }

  ParseResult parse(ArgSeq &args) override {
    auto setter{
        [this](std::string_view name, std::string_view sval) -> OptErrMsg {
          Internal::ValueConverter<T> converter(name, sval);
          if (converter.m_err_msg) {
            return converter.m_err_msg;
          }

          // Do any Option-specific validation.
          if (!valid_value(converter.m_value)) {
            return Internal::invalid_value_msg(name, sval);
          }

          m_value = converter.m_value;
          return {};
        }};

    return Internal::parse_and_set(m_short, m_long, setter, args);
  }

  /**
   * @brief Get the value of this option.  Call this after calling parse on the
   * ArgumentParser to which this option has been added.
   *
   * @return T the value of this option
   */
  [[nodiscard]] T value() const { return m_value; }

  [[nodiscard]] std::string usage() const override {
    return Internal::option_usage_str(m_short, m_long);
  }

  [[nodiscard]] std::string help() const override {
    return Internal::option_help_block(m_short, m_long, m_help_msg);
  }

protected:
  const std::string m_short;
  const std::string m_long;
  const std::string m_help_msg;

  T m_value;

  Option(std::string_view short_name, std::string_view long_name,
         std::string_view help_msg)
      : m_short(short_name), m_long(long_name), m_help_msg(help_msg) {}

  [[nodiscard]] virtual bool valid_value(const T &v) const { return true; }
};

} // namespace ArgParse