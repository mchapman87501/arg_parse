#include "arg_parse.hpp"
#include "arg_parse_result.hpp"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <iostream>
#include <stdexcept>

TEST_CASE("Invalid invocation") {
  using namespace ArgParse;
  auto parser = ArgumentParser::create("Parse some stuff.");
  ArgSeq no_args;
  parser->parse_args(no_args);

  CHECK(parser->should_exit());
  CHECK(parser->exit_code() != 0);
}

TEST_CASE("Show help") {
  using namespace ArgParse;

  auto short_flag = Flag::create("-s", "-s", "Short flag.");
  auto long_flag = Flag::create("-l", "--long", "Long flag.");
  // This is a hokey way to define a flag/option that has only a long name:
  auto long_only_flag =
      Flag::create("--long-only", "--long-only", "I have only a long name.");
  // Explicit booleans are supported, but the values must be 0 or 1.
  auto ltuae = Option<bool>::create("--42", "--42",
                                    "Whether or not to show the answer.");
  auto output = Option<std::filesystem::path>::create(
      "-o", "--output", "Where to write the output.");

  auto some_choices =
      Choice::create("-c", "--choice", "Some choices.", {"c1", "Choice Two"});

  // The API does not prevent creation of impossible positional arg
  // sequences: Nargs::one_or_more means or Nargs::zero_or_more no
  // subsequent positional args will be able to consume any arguments,
  // unless the user interposes a flag or option.
  auto one = Argument<std::string>::create("one", Nargs::one, "One arg.");
  auto some = Argument<int>::create("some", Nargs::one_or_more, "One or more.");
  auto any = Argument<double>::create("any", Nargs::zero_or_more, "Any.");

  const std::string description("Parse some stuff.");
  auto parser = ArgumentParser::create(description);
  parser->add_option(short_flag);
  parser->add_option(long_flag);
  parser->add_option(long_only_flag);
  parser->add_option(ltuae);
  parser->add_option(output);
  parser->add_option(some_choices);
  parser->add_arg(one);
  parser->add_arg(some);
  parser->add_arg(any);

  ArgSeq args{"<exe>", "--help"};
  Tests::ArgParseResult apr(parser, args, true, 0);
  CHECK(apr.check_outcome());
  CHECK(apr.cout_contains(description));
  CHECK(apr.cout_contains("Usage:"));
  // Verify presence of placeholder values for options and for positional args.
  std::vector<std::string> placeholders{
      "--42 42]", "OUTPUT", "ONE", "SOME [SOME ...]", "[ANY ...]",
  };
  for (const auto ph : placeholders) {
    // If one of these tests fails, Catch2 makes it hard to see the actual value
    // of ph. hence this.
    if (!apr.cout_contains(ph)) {
      std::cerr << "Output does not contain '" << ph << "'." << std::endl;
      CHECK(apr.cout_contains(ph));
    }
  }

  std::cout << "DEBUG: help output:" << std::endl << apr.cout();
}

TEST_CASE("Invoke no args") {
  using namespace ArgParse;

  auto flag = Flag::create("-v", "--verbose", "Show a lot of detail.");
  auto output = Option<std::filesystem::path>::create(
      "-o", "--output", "Where to write the output.");
  auto inputs = Argument<double>::create("values_to_add", Nargs::one,
                                         "Values to add together.");

  auto parser = ArgumentParser::create("Parse some stuff.");

  parser->add_option(flag);
  parser->add_option(output);
  parser->add_arg(inputs);

  ArgSeq args{"<exe>"};
  Tests::ArgParseResult apr(parser, args, true, 1);
  CHECK(apr.check_outcome());

  CHECK(!flag->is_set());
  CHECK(output->value() == std::filesystem::path(""));

  CHECK(inputs->values().empty());
  CHECK(apr.cerr_contains("VALUES_TO_ADD"));
}

TEST_CASE("Using Flags and Options") {
  using namespace ArgParse;

  auto parser = ArgumentParser::create("Parse some stuff.");

  SECTION("Setting Flags") {
    auto flag = Flag::create("-f", "--flag", "a flag");
    parser->add_option(flag);

    SECTION("Unset") {
      ArgSeq args{"<exe>"};
      parser->parse_args(args);
      CHECK(!parser->should_exit());
      CHECK(!flag->is_set());
    }

    SECTION("Short") {
      ArgSeq args{"<exe>", "-f"};
      parser->parse_args(args);
      CHECK(!parser->should_exit());
      CHECK(flag->is_set());
    }

    SECTION("Long") {
      ArgSeq args{"<exe>", "--flag"};
      parser->parse_args(args);
      CHECK(!parser->should_exit());
      CHECK(flag->is_set());
    }
  }

  SECTION("Using short|long Options") {
    auto output = Option<std::filesystem::path>::create(
        "-o", "--output", "Where to write the output");
    parser->add_option(output);

    SECTION("Typed Options") {
      auto v = Option<int>::create("-v", "--value", "Integer value");
      parser->add_option(v);
      ArgSeq args{"<exe>", "--value", "-7"};
      parser->parse_args(args);
      CHECK(!parser->should_exit());
      CHECK(v->value() == -7);
    }

    SECTION("Invoke with short option") {
      const std::string out_filename = "foo_bar.txt";
      ArgSeq args{"<exe>", "-o", out_filename};
      parser->parse_args(args);
      CHECK(!parser->should_exit());
      CHECK(output->value() == std::filesystem::path(out_filename));
    }

    SECTION("Invoke with short option, missing value") {
      ArgSeq args{"<exe>", "-o"};
      parser->parse_args(args);
      CHECK(parser->should_exit());
      CHECK(output->value() == std::filesystem::path(""));
    }

    SECTION("Invoke with long option") {
      const std::string out_filename = "foo_bar_2.txt";
      ArgSeq args{"<exe>", "--output", out_filename};
      parser->parse_args(args);
      CHECK(!parser->should_exit());
      CHECK(output->value() == std::filesystem::path(out_filename));
    }

    SECTION("Invoke with long option, missing value") {
      ArgSeq args{"<exe>", "--output"};
      parser->parse_args(args);
      CHECK(parser->should_exit());
      CHECK(output->value() == std::filesystem::path(""));
    }

    SECTION("Invoke with long= option") {
      const std::string out_filename = "long_eq.txt";
      const std::string output_arg = std::string("--output=") + out_filename;
      ArgSeq args{"<exe>", output_arg};
      parser->parse_args(args);
      CHECK(!parser->should_exit());
      CHECK(output->value() == std::filesystem::path(out_filename));
    }

    SECTION("Invoke with long=, missing value") {
      ArgSeq args{"<exe>", "--output="};
      parser->parse_args(args);
      CHECK(parser->should_exit());
      CHECK(output->value() == std::filesystem::path(""));
    }

    SECTION("Invoke with long=, missing value, trailing positional") {
      auto required_value = Argument<std::string>::create(
          "required_value", Nargs::one, "A required value");
      parser->add_arg(required_value);

      ArgSeq args{"<exe>", "--output=", "the required value"};
      Tests::ArgParseResult apr(parser, args, true, 1);
      CHECK(apr.check_outcome());
      CHECK(output->value() == std::filesystem::path(""));
    }

    SECTION("Invoke with invalid option value") {
      auto num_lines =
          Option<int>::create("-n", "--num-lines", "no. lines of output");
      parser->add_option(num_lines);

      ArgSeq args{"<exe>", "-n", "not a number"};
      Tests::ArgParseResult apr(parser, args, true, 1);
      CHECK(apr.check_outcome());

      CHECK(apr.cerr_contains("Invalid value"));
    }

    SECTION("Invoke with unknown option") {
      ArgSeq args{"<exe>", "--number", "42"};
      Tests::ArgParseResult apr(parser, args, true, 1);
      CHECK(apr.check_outcome());
      CHECK(apr.cerr_contains("Unknown option"));
      CHECK(apr.cerr_contains("--number"));
    }
  }

  SECTION("Using default flags and options") {
    auto flag = Flag::create("-v", "--verbose", "Show a lot of detail");
    auto output = Option<std::filesystem::path>::create(
        "-o", "--output", "Where to write the output");
    auto x = Argument<int>::create("x", Nargs::one, "X value.");

    parser->add_option(flag);
    parser->add_option(output);
    parser->add_arg(x);

    ArgSeq args{"<exe>", "42"};

    Tests::ArgParseResult apr(parser, args, false, 0);
    CHECK(apr.check_outcome());
    CHECK(!flag->is_set());
    CHECK(x->values() == std::vector<int>{42});
    CHECK(output->value() == std::filesystem::path(""));
  }
}

namespace {
using namespace ArgParse;
auto create_no_option() {
  std::vector<std::string> no_choice;
  return Choice::create("-c", "--choice", "You have no choice", no_choice);
}
} // namespace

TEST_CASE("Choices") {
  using namespace ArgParse;

  auto parser = ArgumentParser::create("Parse some stuff.");

  SECTION("No Choice") {
    CHECK_THROWS_AS(create_no_option(), std::invalid_argument);
  }

  SECTION("Multiple Choice(s)") {
    std::vector<std::string> choices{"first", "SECOND", "third", "multi word"};
    auto choice =
        Choice::create("-c", "--choice", "Some valid choices", choices);
    parser->add_option(choice);

    SECTION("valid short choice") {
      ArgSeq args{"<exe>", "-c", "first"};
      Tests::ArgParseResult apr(parser, args, false, 0);
      CHECK(apr.check_outcome());
    }

    SECTION("valid long choice") {
      ArgSeq args{"<exe>", "--choice", "third"};
      Tests::ArgParseResult apr(parser, args, false, 0);
      CHECK(apr.check_outcome());
    }

    SECTION("valid long= choice") {
      ArgSeq args{"<exe>", "--choice=multi word"};
      Tests::ArgParseResult apr(parser, args, false, 0);
      CHECK(apr.check_outcome());
    }

    SECTION("valid choice, case-insensitive") {
      ArgSeq args{"<exe>", "-c", "sEcOnD"};
      Tests::ArgParseResult apr(parser, args, false, 0);
      CHECK(apr.check_outcome());
    }

    SECTION("invalid choice") {
      ArgSeq args{"<exe>", "-c", "Invalid Choice"};
      Tests::ArgParseResult apr(parser, args, true, 1);
      CHECK(apr.check_outcome());
      CHECK(apr.cerr_contains("Usage:"));
      CHECK(apr.cerr_contains("Invalid Choice"));
    }
  }
}

// TODO test for redundant setting of flags/options/arguments.

TEST_CASE("Positionals") {
  using namespace ArgParse;

  auto parser = ArgumentParser::create("Parse some stuff.");

  SECTION("::one") {
    auto value = Argument<int>::create("value", Nargs::one, "value");
    parser->add_arg(value);

    SECTION("positive") {
      ArgSeq args{"<exe>", "1"};
      parser->parse_args(args);

      CHECK(!parser->should_exit());
      CHECK(value->is_complete());
      std::vector<int> expected{1};
      CHECK(value->values() == expected);
    }

    SECTION("too few") {
      ArgSeq args{"<exe>"};
      Tests::ArgParseResult apr(parser, args, true, 1);
      CHECK(apr.check_outcome());
      CHECK(!value->is_complete());
      CHECK(apr.cerr_contains("Wrong number"));
    }

    SECTION("too many") {
      ArgSeq args{"<exe>", "1", "200"};
      Tests::ArgParseResult apr(parser, args, true, 1);
      CHECK(apr.check_outcome());

      CHECK(value->is_complete());

      std::vector<int> expected{1};
      CHECK(value->values() == expected);
      CHECK(apr.cerr_contains("Unsupported argument"));
      CHECK(apr.cerr_contains("200"));
    }

    SECTION("invalid value") {
      ArgSeq args{"<exe>", "not a number"};
      Tests::ArgParseResult apr(parser, args, true, 1);
      CHECK(apr.check_outcome());
      CHECK(apr.cerr_contains("'not a number'"));
    }
  }

  SECTION("::one_or_more") {
    auto value = Argument<int>::create("value", Nargs::one_or_more, "value");
    parser->add_arg(value);

    SECTION("positive, one") {
      ArgSeq args{"<exe>", "1"};
      parser->parse_args(args);

      CHECK(!parser->should_exit());
      CHECK(value->is_complete());
      std::vector<int> expected{1};
      CHECK(value->values() == expected);
    }

    SECTION("positive, many") {
      ArgSeq args{"<exe>", "1", "2", "3", "4"};
      parser->parse_args(args);

      CHECK(!parser->should_exit());
      CHECK(value->is_complete());
      std::vector<int> expected{1, 2, 3, 4};
      CHECK(value->values() == expected);
    }

    SECTION("too few") {
      ArgSeq args{"<exe>"};
      Tests::ArgParseResult apr(parser, args, true, 1);
      CHECK(apr.check_outcome());
      CHECK(!value->is_complete());
      CHECK(apr.cerr_contains("Wrong number"));
    }

    SECTION("invalid value") {
      ArgSeq args{"<exe>", "1", "2", "not a number"};
      Tests::ArgParseResult apr(parser, args, true, 1);
      CHECK(apr.check_outcome());
      CHECK(apr.cerr_contains("'not a number'"));
    }
  }

  SECTION("::zero_or_more") {
    auto value = Argument<int>::create("value", Nargs::zero_or_more, "value");
    parser->add_arg(value);

    SECTION("positive, none") {
      ArgSeq args{"<exe>"};
      parser->parse_args(args);

      CHECK(!parser->should_exit());
      CHECK(value->is_complete());
      CHECK(value->values().empty());
    }

    SECTION("positive, one") {
      ArgSeq args{"<exe>", "1"};
      parser->parse_args(args);

      CHECK(!parser->should_exit());
      CHECK(value->is_complete());
      std::vector<int> expected{1};
      CHECK(value->values() == expected);
    }

    SECTION("positive, many") {
      ArgSeq args{"<exe>", "1", "2", "3", "4"};
      parser->parse_args(args);

      CHECK(!parser->should_exit());
      CHECK(value->is_complete());
      std::vector<int> expected{1, 2, 3, 4};
      CHECK(value->values() == expected);
    }

    SECTION("invalid value") {
      ArgSeq args{"<exe>", "1", "2", "not a number"};
      Tests::ArgParseResult apr(parser, args, true, 1);
      CHECK(apr.check_outcome());
      CHECK(apr.cerr_contains("'not a number'"));
    }
  }
}

TEST_CASE("Detect invalid values") {
  using namespace ArgParse;

  auto parser = ArgumentParser::create("Parse some stuff.");
  auto option = Option<std::string>::create("-s", "--sval", "a string value");
  auto value = Argument<int>::create("value", Nargs::zero_or_more, "value");
  parser->add_option(option);
  parser->add_arg(value);

  SECTION("Integer w. trailing text") {
    ArgSeq args{"<exe>", "23tail"};
    Tests::ArgParseResult apr(parser, args, true, 1);
    CHECK(apr.check_outcome());
    CHECK(apr.cerr_contains("Could not completely convert"));
  }

  SECTION("Integer from float") {
    ArgSeq args{"<exe>", "23.5"};
    Tests::ArgParseResult apr(parser, args, true, 1);
    CHECK(apr.check_outcome());
    CHECK(apr.cerr_contains("Could not completely convert"));
  }

  SECTION("Multi-word string") {
    ArgSeq args{"<exe>", "--sval", "word after word"};
    Tests::ArgParseResult apr(parser, args, false, 0);
    CHECK(apr.check_outcome());
    CHECK(option->value() == "word after word");
  }
}

TEST_CASE("Using argc, argv") {
  using namespace ArgParse;

  auto parser = ArgumentParser::create("test the argc, argv interface");
  auto arguments = Argument<std::string>::create(
      "aargh", Nargs::zero_or_more, "Any arguments you care to provide.");
  parser->add_arg(arguments);

  SECTION("No args") {
    const char *argv[] = {"<exe>"};
    int argc = 1;
    parser->parse_args(argc, (char **)argv);
    CHECK(!parser->should_exit());
    CHECK(arguments->values().empty());
  }

  SECTION("one arg") {
    const char *argv[] = {"<exe>", "ett"};
    int argc = 2;
    parser->parse_args(argc, (char **)argv);
    CHECK(!parser->should_exit());
    std::vector<std::string> expected{"ett"};
    auto actual = arguments->values();
    CHECK(actual == expected);
  }

  SECTION("Multiple args") {
    const char *argv[] = {"<exe>", "une", "två", "drei"};
    int argc = 4;
    parser->parse_args(argc, (char **)argv);
    CHECK(!parser->should_exit());
    std::vector<std::string> expected{"une", "två", "drei"};
    auto actual = arguments->values();
    CHECK(actual == expected);
  }
}

TEST_CASE("Convenience functions") {
  using namespace ArgParse;

  const std::string description("Test convenience functions.");
  auto parser = ArgumentParser::create(description);

  auto short_flag = flag(parser, "-s", "-s", "Short flag.");
  auto long_flag = flag(parser, "-l", "--long", "Long flag.");
  auto ltuae = option<bool>(parser, "--42", "--42",
                            "Whether or not to show the answer.");

  auto a_choice = choice(parser, "-c", "--choice", "Choose one",
                         {"first", "Second", "Third"});
  auto one = argument<std::string>(parser, "one", Nargs::one, "One arg.");
  auto some =
      argument<double>(parser, "some", Nargs::one_or_more, "One or more.");

  SECTION("Help") {
    ArgSeq args{"<exe>", "--help"};
    Tests::ArgParseResult apr(parser, args, true, 0);
    CHECK(apr.check_outcome());
    CHECK(apr.cout_contains(description));
    CHECK(apr.cout_contains("Usage:"));
    std::vector<std::string> placeholders{
        "--42 42]", "--choice CHOICE", "Third", "ONE", "SOME [SOME ...]",
    };
    for (const auto ph : placeholders) {
      // If one of these tests fails, Catch2 makes it hard to see the actual
      // value of ph. hence this.
      if (!apr.cout_contains(ph)) {
        std::cerr << "Output does not contain '" << ph << "'." << std::endl;
        CHECK(apr.cout_contains(ph));
      }
    }
  }

  SECTION("Verify options/args were added to parser") {
    ArgSeq args{"<exe>", "-s",  "--long", "--42=1", "-c",
                "THIRD", "une", "2",      "3"};
    Tests::ArgParseResult apr(parser, args, false, 0);
    CHECK(apr.check_outcome());
    CHECK(short_flag->is_set());
    CHECK(long_flag->is_set());
    CHECK(ltuae->value());

    std::vector<std::string> one_expected{"une"};
    CHECK(one->values() == one_expected);

    std::vector<double> some_expected{2.0, 3.0};
    CHECK(some->values() == some_expected);
  }
}