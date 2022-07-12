#pragma once

#include "nargs.hpp"
#include <string>

/**
 * @brief Internal utilities are for internal use in formatting usage and
 * detailed help messages.
 *
 */
namespace ArgParse::Internal {
/**
 * @brief Get a usage string for a flag.
 *
 * @param short_name Short name of the flag, e.g., "-f"
 * @param long_name Long name of the flag, e.g., "--flag"
 * @return std::string The usage string
 */
std::string flag_usage_str(std::string_view short_name,
                           std::string_view long_name);

/**
 * @brief Get a usage string for an option.
 *
 * @param short_name Short name of the option, e.g., "-o"
 * @param long_name Long name of the option, e.g., "--option"
 * @return std::string The usage string
 */
std::string option_usage_str(std::string_view short_name,
                             std::string_view long_name);

/**
 * @brief Get a usage string for a positional argument.
 *
 * @param name Name of the positional argument
 * @param nargs Number of positional command-line arguments which can be
 * provided for the given name
 * @return std::string The usage string
 */
std::string arg_usage_str(std::string_view name, Nargs nargs);

/**
 * @brief Get a detailed help string describing a flag.
 *
 * @param short_name Short name of the flag, e.g., "-f"
 * @param long_name Long name of the flag, e.g., "--flag"
 * @param help_msg A help message describing the meaning of the flag
 * @return std::string A detailed help message
 */
std::string flag_help_block(std::string_view short_name,
                            std::string_view long_name,
                            std::string_view help_msg);

/**
 * @brief Get a detailed help string describing an option.
 *
 * @param short_name Short name of the option, e.g., "-o"
 * @param long_name Long name of the option, e.g., "--option"
 * @param help_msg A help message describing the meaning of the option
 * @return std::string A detailed help message
 */
std::string option_help_block(std::string_view short_name,
                              std::string_view long_name,
                              std::string_view help_msg);

/**
 * @brief Get a detailed help string describing a positional argument.
 *
 * @param arg_name The name of the argument
 * @param nargs Specification of how many values may be provided for the
 * argument
 * @param help_msg A help message describing the meaning of the argument
 * @return std::string A detailed help message
 */
std::string arg_help_block(std::string_view arg_name, Nargs nargs,
                           std::string_view help_msg);
} // namespace ArgParse::Internal