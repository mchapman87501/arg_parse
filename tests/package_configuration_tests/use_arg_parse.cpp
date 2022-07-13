#include "arg_parse.hpp"
#include <iostream>

int main(int argc, char **argv) {
  auto parser = ArgParse::ArgumentParser::create("Example program");
  auto verbose = ArgParse::flag(parser, "-v", "--verbose", "Be verbose.");

  parser->parse_args(argc, argv);
  if (parser->should_exit()) {
    return parser->exit_code();
  }

  if (verbose->is_set()) {
    std::cout << "Here is some verbose output." << std::endl;
  }
  return 0;
}
