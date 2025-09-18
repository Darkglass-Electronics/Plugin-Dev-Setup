# Gain plugin example using rust-lv2

This is an example gain plugin using rust-lv2.

It is the most simple possible plugin created with rust-lv2 that meets Anagram requirements:
 - has enable/bypass control
 - has parameter smoothing
 - uses mono IO (a dual-mono version would produce the same sound)

TODO:

 - contains name abbreviation
 - contains block images

## Changes compared to stock rust-lv2 plugins

All custom changes are added in the LV2 ttl meta-data.

TODO show example here

## Building and deploy

Note: this step assumes you either are using the docker shell or have imported the build environment through "local.env".

```sh
# change to this directory
cd /path/to/rust-lv2-gain/

# build the project
~/.cargo/bin/cargo build \
    --release \
    --target aarch64-unknown-linux-gnu \
    --config "target.aarch64-unknown-linux-gnu.rustflags=['-Ctarget-cpu=cortex-a76','-Ctarget-feature=+neon','-Clink-args=--sysroot=${STAGING_DIR}']" \
    --config "target.aarch64-unknown-linux-gnu.linker='${CC}'"

# copy over lv2 bundles into Anagram
scp -O -r *.lv2 root@192.168.51.1:/root/.lv2/

# restart Anagram system services that use plugins
ssh root@192.168.51.1 "systemctl restart jack2 lvgl-app"
```
