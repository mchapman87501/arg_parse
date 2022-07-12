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

  virtual std::string usage() const = 0;
  virtual std::string help() const = 0;
  virtual Nargs nargs() const = 0;

  virtual ParseResult parse(ArgSeq &args) = 0;
  virtual bool is_complete() const = 0;
  virtual size_t num_values() const = 0;
};
} // namespace ArgParse
