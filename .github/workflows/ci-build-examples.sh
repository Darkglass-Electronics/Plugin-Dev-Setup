#!/bin/bash

if [ -e ~/.bashrc ]; then
    source ~/.bashrc
fi

set -e

cd $(dirname "${0}")/../..

function build_and_validate()
{

echo "----------------------------------------------------- Building examples"
for d in $(ls -d examples/*); do
    pushd ${d}
    if [ -e CMakeLists.txt ]; then
        rm -rf build
        ${CMAKE:=cmake} -S . -B build
        $(which cmake) --build build -j$(nproc)
    fi
    popd
done

rm -rf lv2
mkdir -p lv2

for p in $(find examples -name '*.lv2'); do
    echo "----------------------------------------------------- Validating $(basename ${p})"
    lv2_validate \
        ${TARGET_DIR}/usr/lib/lv2/dg-meta/*.ttl \
        ${TARGET_DIR}/usr/lib/lv2/dg-properties.lv2/*.ttl \
        ${TARGET_DIR}/usr/lib/lv2/kx-control-input-port-change-request.lv2/*.ttl \
        ${TARGET_DIR}/usr/lib/lv2/kx-meta/*.ttl \
        ${TARGET_DIR}/usr/lib/lv2/kx-programs.lv2/*.ttl \
        ${TARGET_DIR}/usr/lib/lv2/kx-properties.lv2/*.ttl \
        ${TARGET_DIR}/usr/lib/lv2/mod.lv2/*.ttl \
        ${TARGET_DIR}/usr/lib/lv2/modgui.lv2/*.ttl \
        ${TARGET_DIR}/usr/lib/lv2/mod-hmi.lv2/*.ttl \
        ${TARGET_DIR}/usr/lib/lv2/mod-license.lv2/*.ttl \
        ${p}/*.ttl
    cp -r ${p} lv2/
done

if [ -z "${TARGET_DIR}" ]; then
    echo "----------------------------------------------------- Validating (lv2lint)"
    PLUGINS=$(env LV2_PATH="$(pwd)/lv2" lv2ls)
    env LV2_PATH="$(pwd)/lv2:/usr/lib/lv2" lv2lint -s lv2_generate_ttl ${PLUGINS}
fi

}

# regular build
build_and_validate

# if already using mod-plugin-builder environment, stop here
if [ -n "${TARGET_DIR}" ]; then
  exit 0
fi

# build through mod-plugin-builder
platform=${1:=darkglass-anagram}
[ -e mod-plugin-builder ] || git clone https://github.com/mod-audio/mod-plugin-builder --depth=1
source ./mod-plugin-builder/local.env ${platform}
build_and_validate
