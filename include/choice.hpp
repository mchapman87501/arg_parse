#pragma once

#include "option.hpp"
#include <string>
#include <vector>

namespace ArgParse {
struct Choice : public Option<std::string> {
  using Ptr = std::shared_ptr<Choice>;

  Choice(const Choice &src) = delete;
  Choice(Choice &&src) = delete;
  Choice &operator=(const Choice &src) = delete;
  Choice &operator=(Choice &&src) = delete;

  static Ptr create(std::string_view short_name, std::string_view long_name,
                    std::string_view help_msg,
                    const std::vector<std::string> &valid_choices);

protected:
  Choice(std::string_view short_name, std::string_view long_name,
         std::string_view help_msg)
      : Option<std::string>(short_name, long_name, help_msg) {}
};
} // namespace ArgParse
