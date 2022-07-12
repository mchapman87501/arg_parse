#pragma once

#include "aliases.hpp"
#include "flag.hpp"
#include "i_argument.hpp"
#include "i_option.hpp"
#include <string>
#include <string_view>

namespace ArgParse {

/**
 * @brief Parses command-line arguments.
 *
 */
struct ArgumentParser {
  using Ptr = std::shared_ptr<ArgumentParser>;

  /**
   * @brief Create a new instance.
   *
   * @param description Describes the purpose of the program that's parsing the
   * command line.
   * @return Ptr Pointer to a new instance.
   */
  static Ptr create(std::string_view description);

  /**
   * @brief Add an Option or a Flag.
   *
   * @param option An option (-o|--output FILENAME) or a flag (-v|--verbose)
   */
  virtual void add_option(IOption::Ptr option) = 0;

  /**
   * @brief Add a positional argument, e.g., a required filename.
   *
   * @param arg A positional argument
   */
  virtual void add_arg(IArgument::Ptr arg) = 0;

  /**
   * @brief Parse a sequence of command-line arguments.
   *
   * @param args Arguments to parse
   */
  virtual void parse_args(const ArgSeq &args) = 0;

  /**
   * @brief Find out whether or not the program should exit due to invalid
   * command-line arguments. Call this after calling parse_args.
   *
   * @return true if the command-line arguments were invalid
   * @return false if the command-line arguments were valid
   */
  virtual bool should_exit() const = 0;

  /**
   * @brief Get the recommended exit code.
   * Call this after calling parse_args.  If should_exit() returns true,
   * the program should be terminated with the exit code returned by this
   * method.
   *
   * @return int The recommended exit code
   */
  virtual int exit_code() const = 0;

  /**
   * @brief Print a parse error message to cerr, followed by usage.
   *
   * @param msg The error message to print
   * @param exit_code The recommended exit code for this error
   */
  virtual void show_error(std::string_view msg, int exit_code) = 0;

protected:
  ArgumentParser() {}
};
} // namespace ArgParse