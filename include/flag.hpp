#pragma once

#include "i_option.hpp"
#include "parse_result.hpp"
#include <string_view>

namespace ArgParse {
/**
 * @brief Represents a boolean command-line flag, e.g., "-v|--verbose".
 *
 */
struct Flag : public virtual IOption {
  using Ptr = std::shared_ptr<Flag>;

  /**
   * @brief Create a new flag specification.
   *
   * @param short_name The short, single-dash name of this flag ("-v")
   * @param long_name The long, double-dash name of this flag ("--verbose")
   * @param help_msg A description of the purpose of this flag
   * @return Ptr A pointer to the new instance.
   */
  static Ptr create(std::string_view short_name, std::string_view long_name,
                    std::string_view help_msg);

  /**
   * @brief Find out whether this flag is set.  Call
   * this after calling parse on the ArgumentParser to which this spec has been
   * added.
   *
   * @return whether or not this flag has been set
   */
  [[nodiscard]] virtual bool is_set() const = 0;
};

} // namespace ArgParse