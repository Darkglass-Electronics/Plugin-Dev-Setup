# Gain plugin example using DPF

This is an example gain plugin using DPF and CMake.

It is the most simple possible plugin created with DPF that meets Anagram requirements:
 - has enable/bypass control
 - has parameter smoothing
 - uses mono IO (a dual-mono version would produce the same sound)
 - contains name abbreviation
 - contains block images

## Changes compared to stock DPF plugins

### Anagram specific properties

The `DISTRHO_PLUGIN_ABBREVIATION` macro must be defined in your `DistrhoPluginInfo.h` file.
It must a string containing only 2 or 3 characters in uppercase.
See [REQUIREMENTS.md](https://github.com/Darkglass-Electronics/Plugin-Dev-Setup/blob/main/REQUIREMENTS.md)

```C
#define DISTRHO_PLUGIN_ABBREVIATION "GAI"
```

The `DISTRHO_PLUGIN_ANAGRAM_BLOCK_IMAGE_OFF` and `DISTRHO_PLUGIN_ANAGRAM_BLOCK_IMAGE_ON` must also be defined in your `DistrhoPluginInfo.h` file.
These are paths to a in-bundle/local 200x200 PNG image file to be used as the plugin's block image asset when OFF and ON.

```C
#define DISTRHO_PLUGIN_ANAGRAM_BLOCK_IMAGE_OFF "anagram-block-off.png"
#define DISTRHO_PLUGIN_ANAGRAM_BLOCK_IMAGE_ON "anagram-block-on.png"
```

## Building and deploy

Note: this step assumes you either are using the docker shell or have imported the build environment through "local.env".

```sh
# change to this directory
cd /path/to/dpf-gain-cmake/

# setup project for building (-S specifies cmake project folder, -B specifies build output folder)
cmake -S . -B build

# build the project
$(which cmake) --build build

# copy over lv2 bundles into Anagram
scp -O -r build/bin/*.lv2 root@192.168.51.1:/root/.lv2/

# restart Anagram system services that use plugins
ssh root@192.168.51.1 "systemctl restart jack2 lvgl-app"
```
