# Gain plugin example using JUCE

This is an example gain plugin using JUCE and CMake.

It is the most simple possible plugin created with JUCE that meets Anagram requirements:
 - has enable/bypass control
 - has parameter smoothing
 - uses mono IO (a dual-mono version would produce the same sound)

## Building and deploy

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
