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

  [[nodiscard]] virtual std::string usage() const = 0;
  [[nodiscard]] virtual std::string help() const = 0;
  [[nodiscard]] virtual Nargs nargs() const = 0;

  virtual ParseResult parse(ArgSeq &args) = 0;
  [[nodiscard]] virtual bool is_complete() const = 0;
  [[nodiscard]] virtual size_t num_values() const = 0;
};
} // namespace ArgParse
