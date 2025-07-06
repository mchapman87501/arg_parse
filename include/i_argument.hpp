#pragma once

#include "aliases.hpp"
#include "nargs.hpp"
#include "parse_result.hpp"
#include <memory>
#include <string_view>

namespace ArgParse {

/**
 * @brief Defines the required interface of any positional argument spec.  Don't
 * use this.  Use, e.g., Argument.
 *
 */
struct IArgument {
  using Ptr = std::shared_ptr<IArgument>;

  /**
   * @brief Get a usage string for this argument specification.
   *
   * @return std::string A string to be included in any "Usage:" message
   */
  [[nodiscard]] virtual std::string usage() const = 0;

  /**
   * @brief Get a help message describing this argument specification.  The
   * string can be included in any detailed help information.
   *
   * @return std::string A help message
   */
  [[nodiscard]] virtual std::string help() const = 0;

  /**
   * @brief Get the number of positional arguments which can be supplied for
   * this argument, e.g., one, zero or more, one or more.
   *
   * @return Nargs The number of positional arguments that can be supplied for
   * this argument
   */
  [[nodiscard]] virtual Nargs nargs() const = 0;

  /**
   * @brief Consume all positional command-line arguments for this argument.
   * Arguments are consumed starting with the first value in args.
   *
   * @param args The sequence of command-line arguments that have not yet been
   * consumed
   * @return ParseResult An indication of whether this argument consumed any
   * command line arguments; and, if so, whether any errors were encountered
   */
  virtual ParseResult parse(ArgSeq &args) = 0;

  /**
   * @brief Call this after calling parse, to find out whether this argument
   * found all of the command-line arguments it needed.
   *
   * @return true If this argument got all of its required values
   * @return false If it did not
   */
  [[nodiscard]] virtual bool is_complete() const = 0;

  /**
   * @brief Get the number of command-line arguments matched by this argument.
   * Call this after calling parse on the ArgumentParser to which this argument
   * has been added.
   *
   * @return size_t The number of command-line arguments matched by this
   * argument.
   */
  [[nodiscard]] virtual size_t num_values() const = 0;
};
} // namespace ArgParse
