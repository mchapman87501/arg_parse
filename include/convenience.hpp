#pragma once

#include "argument.hpp"
#include "argument_parser.hpp"
#include "choice.hpp"
#include "flag.hpp"
#include "option.hpp"
#include <string_view>

namespace ArgParse {

/**
 * @brief Add a new Flag to an ArgumentParser.
 *
 * @param parser The Parser to which to add the Flag
 * @param short_name The short, single-dash name of the Flag ("-v")
 * @param long_name The long, double-dash name of the Flag ("--verbose")
 * @param help_msg A description of the purpose of the Flag
 * @return Flag::Ptr The new Flag
 */
Flag::Ptr flag(ArgumentParser::Ptr parser, std::string_view short_name,
               std::string_view long_name, std::string_view help_msg) {
  auto result = Flag::create(short_name, long_name, help_msg);
  parser->add_option(result);
  return result;
}

/**
 * @brief Add a new Option to an ArgumentParser.
 *
 * @tparam T The type of value held by the Option
 * @param parser The Parser to which to add the Option
 * @param short_name The short, single-dash name of the Option ("-o")
 * @param long_name The long, double-dash name of the Option ("--output")
 * @param help_msg A description of the purpose of the Option
 * @param default_value The default value for the Option
 * @return auto The new Option
 */
template <typename T>
auto option(ArgumentParser::Ptr parser, std::string_view short_name,
            std::string_view long_name, std::string_view help_msg,
            const T default_value = {}) {
  auto result =
      Option<T>::create(short_name, long_name, help_msg, default_value);
  parser->add_option(result);
  return result;
}

/**
 * @brief      Add a new Choice to an ArgumentParser.
 *
 * @param  parser      The Parser to which to add the Choice
 * @param  short_name  The short, single-dash name of the Choice ("-c")
 * @param  long_name   The long, double-dash name of the Choice ("--choice")
 * @param  help_msg    A description of the purpose the Choice
 * @param  choices     Valid values for the Choice
 * @return  The new Choice
 */
Choice::Ptr choice(ArgumentParser::Ptr parser, std::string_view short_name,
                   std::string_view long_name, std::string_view help_msg,
                   const std::vector<std::string> &choices) {
  auto result = Choice::create(short_name, long_name, help_msg, choices);
  parser->add_option(result);
  return result;
}

/**
 * @brief Add a new Argument to an ArgumentParser.
 *
 * @tparam T The type of value(s) held by the Argument
 * @param parser The Parser to which to add the Argument
 * @param name The Argument's name
 * @param nargs The number of values the Argument can accept
 * @param help_msg A description of the purpose of the Argument
 * @return auto The new Argument
 */
template <typename T>
auto argument(ArgumentParser::Ptr parser, std::string_view name, Nargs nargs,
              std::string_view help_msg) {
  auto result = Argument<T>::create(name, nargs, help_msg);
  parser->add_arg(result);
  return result;
}
} // namespace ArgParse