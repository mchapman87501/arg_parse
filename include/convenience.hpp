#pragma once

#include "argument.hpp"
#include "argument_parser.hpp"
#include "flag.hpp"
#include "option.hpp"
#include <string_view>

namespace ArgParse {

Flag::Ptr flag(ArgumentParser::Ptr parser, std::string_view short_name,
               std::string_view long_name, std::string_view help_msg) {
  auto result = Flag::create(short_name, long_name, help_msg);
  parser->add_option(result);
  return result;
}

template <typename T>
auto option(ArgumentParser::Ptr parser, std::string_view short_name,
            std::string_view long_name, std::string_view help_msg) {
  auto result = Option<T>::create(short_name, long_name, help_msg);
  parser->add_option(result);
  return result;
}

template <typename T>
auto argument(ArgumentParser::Ptr parser, std::string_view name, Nargs nargs,
              std::string_view help_msg) {
  auto result = Argument<T>::create(name, nargs, help_msg);
  parser->add_arg(result);
  return result;
}
} // namespace ArgParse