#pragma once

#include "help_fmt.hpp"
#include "i_argument.hpp"
#include "value_converter.hpp"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace ArgParse {

/**
 * @brief Argument describes a type-checked positional argument.
 *
 * @tparam T The C++ type of the positional argument
 */
template <typename T> struct Argument : public IArgument {
  using Ptr = std::shared_ptr<Argument<T>>;

  /**
   * @brief Create a new argument specification.
   *
   * @param name The name of this positional argument, e.g., "filename"
   * @param nargs The number of command-line arguments that can be supplied for
   * this spec, e.g., one, zero or more, one or more
   * @param help_msg A help message describing the meaning of this parameter
   * @return Ptr A pointer to the new instance
   */
  static Ptr create(std::string_view name, Nargs nargs,
                    std::string_view help_msg) {
    return std::shared_ptr<Argument>(new Argument(name, nargs, help_msg));
  }

  /**
   * @brief Get a usage string for this argument specification.
   *
   * @return std::string A string to be included in any "Usage:" message
   */
  [[nodiscard]] std::string usage() const override {
    return Internal::arg_usage_str(m_name, m_nargs);
  }

  /**
   * @brief Get a help message describing this argument specification.  The
   * string can be included in any detailed help information.
   *
   * @return std::string A help message
   */
  [[nodiscard]] std::string help() const override {
    return Internal::arg_help_block(m_name, m_nargs, m_help_msg);
  }

  /**
   * @brief Get the number of positional arguments which can be supplied for
   * this spec, e.g., one, zero or more, one or more.
   *
   * @return Nargs The number of positional arguments that can be supplied for
   * this spec
   */
  [[nodiscard]] Nargs nargs() const override { return m_nargs; }

  /**
   * @brief Consume all positional command-line arguments for this spec.
   * Arguments are consumed starting with the first value in args.
   *
   * @param args The sequence of command-line arguments that have not yet been
   * consumed
   * @return ParseResult An indication of whether this spec consumed any command
   * line arguments; and, if so, whether any errors were encountered
   */
  ParseResult parse(ArgSeq &args) override {
    if (args.empty()) {
      return ParseResult::no_match();
    }

    if ((m_nargs == Nargs::one) && (num_values() > 0)) {
      return ParseResult::no_match();
    }

    const std::string strval(args.front());
    Internal::ValueConverter<T> converter(m_name, strval);
    args.pop_front();
    if (converter.m_err_msg) {
      return ParseResult::match_with_error(converter.m_err_msg.value());
    }
    m_values.push_back(converter.m_value);
    return ParseResult::match();
  }

  /**
   * @brief Get all of the command-line arguments that were matched by this
   * spec. Call this after calling parse.
   *
   * @return std::vector<T> The sequence of command-line arguments matched by
   * this spec
   */
  [[nodiscard]] std::vector<T> values() const { return m_values; }

  /**
   * @brief Call this after calling parse, to find out whether this spec found
   * all of the command-line arguments it needed.
   *
   * @return true If this spec got all of its required values
   * @return false If it did not
   */
  [[nodiscard]] bool is_complete() const override {
    switch (m_nargs) {
    case Nargs::one:
      return m_values.size() == 1;
    case Nargs::zero_or_more:
      return true;
    case Nargs::one_or_more:
      return m_values.size() > 0;
    }
    return false;
  }

  /**
   * @brief Get the number of command-line arguments matched by this spec.  Call
   * this after calling parse on the ArgumentParser to which this spec has been
   * added.
   *
   * @return size_t The number of command-line arguments matched by this spec.
   */
  [[nodiscard]] size_t num_values() const override { return m_values.size(); }

protected:
  Argument(std::string_view name, Nargs nargs, std::string_view help_msg)
      : m_name(name), m_nargs(nargs), m_help_msg(help_msg) {}

private:
  const std::string m_name;
  const Nargs m_nargs;
  const std::string m_help_msg;
  std::vector<T> m_values;
};
} // namespace ArgParse