#include "arg_parse.hpp"
#include <iostream>

int main(int argc, char **argv) {
  using namespace std;
  using namespace ArgParse;

  auto parser = ArgumentParser::create(
      "If you can build this, you can use the CMake config.");
  auto verbose = Flag::create("-v", "--verbose", "Be verbose.");
  parser->add_option(verbose);

  ArgSeq args;
  for (int i = 0; i < argc; ++i) {
    args.push_back(argv[i]);
  }

  parser->parse_args(args);
  if (parser->should_exit()) {
    return parser->exit_code();
  }

  if (verbose->is_set()) {
    cout << "You set '-v', so here is some verbose output." << endl;
  }
  return 0;
}