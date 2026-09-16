#!/bin/sh

set -e

cd "$(dirname "${0}")"

docker buildx build \
    --build-arg debug=false \
    --build-arg target= \
    --tag "darkglass-anagram" \
    .
