#pragma once

#include "aliases.hpp"
#include "parse_result.hpp"
#include <memory>
#include <string_view>

namespace ArgParse {
/**
 * @brief Defines the required interface of any option or flag spec.  Don't use
 * this.  Use, e.g., Option or Flag.
 *
 */
struct IOption {
  using Ptr = std::shared_ptr<IOption>;

  [[nodiscard]] virtual std::string usage() const = 0;
  [[nodiscard]] virtual std::string help() const = 0;

  virtual ParseResult parse(ArgSeq &args) = 0;
};
} // namespace ArgParse
