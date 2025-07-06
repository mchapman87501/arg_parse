#include "argument_parser.hpp"
#include "i_argument.hpp"
#include "i_option.hpp"
#include <iostream>
#include <sstream>
#include <vector>

namespace ArgParse {

struct Impl : public ArgumentParser {
  Impl(std::string_view description) : m_description(description) {
    m_help_flag =
        Flag::create("-h", "--help", "Show this help message and exit.");
    m_opt_specs.push_back(m_help_flag);
  }

  void add_option(IOption::Ptr option) override {
    m_opt_specs.push_back(option);
  }

  void add_arg(IArgument::Ptr arg) override { m_arg_specs.push_back(arg); }

private:
  void consume_cmd_name(ArgSeq &mut_args) {
    m_invoked_as = mut_args.front();
    mut_args.pop_front();
  }

  bool consume_option(ArgSeq &mut_args) {
    for (auto spec : m_opt_specs) {
      auto parse_result = spec->parse(mut_args);
      if (parse_result.matched()) {
        return process_parse_result(parse_result);
      }
    }
    // No option matched.  Unknown option?
    if (!mut_args.empty()) {
      std::string arg(mut_args.front());
      if (arg.starts_with("-")) {
        show_error("Unknown option '" + arg + "'", 1);
        return false;
      }
    }
    return false;
  }

  bool consume_arg(ArgSeq &mut_args) {
    for (auto spec : m_arg_specs) {
      auto parse_result = spec->parse(mut_args);
      if (parse_result.matched()) {
        return process_parse_result(parse_result);
      }
    }
    return false;
  }

  bool process_parse_result(const ParseResult &parse_result) {
    if (parse_result.error_msg()) {
      show_error(parse_result.error_msg().value(), 1);
      return false;
    }
    return true;
  }

  void report_unused_args(ArgSeq &mut_args) {
    std::ostringstream outs;
    outs << "Unsupported argument(s):";
    for (const auto arg : mut_args) {
      outs << " " << arg;
    }

    std::string msg(outs.str());
    show_error(msg, 1);
  }

  std::string expected_arg_count(IArgument::Ptr const spec) {
    switch (spec->nargs()) {
    case Nargs::one:
      return "1";
    case Nargs::zero_or_more:
      // Shouldn't reach here, I think; a zero_or_more argument
      // should be complete if it has zero values.
      return ">= 0";
    case Nargs::one_or_more:
      return ">= 1";
    }
    return "?";
  }

  void validate_arg_specs() {
    for (auto spec : m_arg_specs) {
      if (!spec->is_complete()) {
        std::ostringstream outs;
        outs << "Wrong number of value(s) for required parameter '"
             << spec->usage() << "'.  Expected " << expected_arg_count(spec)
             << ", got " << spec->num_values() << std::endl;
        show_error(outs.str(), 1);
        return;
      }
    }
  }

public:
  void parse_args(int argc, char *argv[]) override {
    ArgSeq args;
    for (int i = 0; i < argc; ++i) {
      args.push_back(argv[i]);
    }
    parse_args(args);
  }

  void parse_args(const ArgSeq &args) override {
    if (args.empty()) {
      show_error("Internal Error: empty args vector", 2);
      return;
    }

    ArgSeq mut_args(args);
    consume_cmd_name(mut_args);

    while (!mut_args.empty()) {
      // Allow interleaving options with positional args...
      bool did_match = consume_option(mut_args) || consume_arg(mut_args);

      // Bail as soon as a help flag is encountered.
      if (m_help_flag->is_set()) {
        show_help();
        return;
      }

      if (!did_match && !mut_args.empty()) {
        report_unused_args(mut_args);
        return;
      }
    }
    validate_arg_specs();
  }

  [[nodiscard]] bool should_exit() const override {
    return m_exit_code.has_value();
  }

  [[nodiscard]] int exit_code() const override {
    return m_exit_code.value_or(0);
  }

  void show_error(std::string_view message, int exit_code) override {
    std::cerr << "Error: " << message << std::endl;
    show_usage(std::cerr, exit_code);
  }

private:
  std::string m_description;
  std::string m_invoked_as;

  Flag::Ptr m_help_flag;

  std::vector<IOption::Ptr> m_opt_specs;
  std::vector<IArgument::Ptr> m_arg_specs;

  std::optional<int> m_exit_code;

  void show_help() {
    std::cout << m_description << std::endl;
    show_usage(std::cout, 0);
  }

  void show_usage(std::ostream &outs, int exit_code) {
    outs << "Usage: " << m_invoked_as;
    for (auto spec : m_opt_specs) {
      outs << " " << spec->usage();
    }
    for (auto spec : m_arg_specs) {
      outs << " " << spec->usage();
    }
    outs << std::endl;

    if (!m_opt_specs.empty()) {
      outs << "Options:" << std::endl;
      for (auto spec : m_opt_specs) {
        outs << spec->help() << std::endl;
      }
    }

    if (!m_arg_specs.empty()) {
      outs << "Arguments:" << std::endl;
      for (auto spec : m_arg_specs) {
        outs << spec->help() << std::endl;
      }
    }

    m_exit_code = exit_code;
  }
};

ArgumentParser::Ptr ArgumentParser::create(std::string_view description) {
  return std::make_shared<Impl>(description);
}
} // namespace ArgParse