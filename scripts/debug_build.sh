#!/bin/sh
set -e -u

cmake --preset profile --fresh
cmake --build --preset profile --target arg_parse_coverage_report
open build/profile/arg_parse_coverage_report/index.html