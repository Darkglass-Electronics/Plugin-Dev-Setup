# Gain plugin example using JUCE

This is an example gain plugin using JUCE and CMake.

It is the most simple possible plugin created with JUCE that meets Anagram requirements:
 - has enable/bypass control
 - has parameter smoothing
 - uses mono IO (a dual-mono version would produce the same sound)
 - contains name abbreviation
 - contains block images

## Changes compared to stock JUCE plugins

### Use Anagram-specific LV2 wrapper

In the `CMakeLists.txt` file you will need to import [juce-anagram-lv2](https://github.com/Darkglass-Electronics/juce-anagram-lv2.git)
and then call it for each plugin, specifying a few custom Anagram details.

```cmake
# Setup a JUCE plugin the usual way
juce_add_plugin(DarkglassExampleGainJUCE ...)

# Import the juce-anagram-lv2 project
add_subdirectory(juce-anagram-lv2)

# Enable the custom Anagram LV2 wrapper
# For more options see https://github.com/Darkglass-Electronics/juce-anagram-lv2/blob/main/CMakeLists.txt
juce_anagram_lv2_setup(DarkglassExampleGainJUCE
    CATEGORY "lv2:UtilityPlugin"
    BLOCK_IMAGE_OFF "images/block-off.png"
    BLOCK_IMAGE_ON "images/block-on.png"
)
```

### Create block images

Anagram uses 200x200 PNG image files for the "block" image of each plugin.  
This is specified by the `BLOCK_IMAGE_OFF` and `BLOCK_IMAGE_ON` CMake properties.

### Set category

Stock JUCE does not have a way to set the LV2 category, so we implement our own way here.  
This is specified by the `CATEGORY` CMake property.

Note that Anagram does not use stock LV2 categories,
see [CATEGORIES.md](https://github.com/Darkglass-Electronics/Plugin-Dev-Setup/blob/main/CATEGORIES.md)
for the LV2 -> Anagram category conversion.

## Building and deploy

Note: this step assumes you either are using the docker shell or have imported the build environment through "local.env".

```sh
# change to this directory
cd /path/to/juce-gain-cmake/

# setup project for building (-S specifies cmake project folder, -B specifies build output folder)
cmake -S . -B build

# build the project
$(which cmake) --build build

# copy over lv2 bundles into Anagram
scp -O -r build/*_artefacts/*/LV2/*.lv2 root@192.168.51.1:/root/.lv2/

# restart Anagram system services that use plugins
ssh root@192.168.51.1 "systemctl restart jack2 lvgl-app"
```
