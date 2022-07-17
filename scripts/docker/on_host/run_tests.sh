#!/bin/sh
set -e -u

mkdir -p build_artifacts
docker run --rm -v${PWD}:/source \
    build_arg_parse "/source/scripts/docker/in_container/test_with_coverage.sh"
