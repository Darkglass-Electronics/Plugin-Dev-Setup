# Filter plugin example using raw LV2

This is an example filter plugin using raw LV2 and CMake.

It meets Anagram requirements:
 - has enable/bypass control
 - has parameter smoothing
 - uses mono IO (a dual-mono version would produce the same sound)
 - contains name abbreviation

TODO:
 - contains block images

### Anagram specific properties

This plugin has 2 controls:

 - Type
 - Frequency

The **Type** parameter influences the behaviour of the **Frequency** one, where the first value is "Off".  
This plugin implements the "Control Port State Update" extension to indicate that **Frequency** is "Inactive" when **Type** is "Off".

## Building and deploy

Note: this step assumes you either are using the docker shell or have imported the build environment through "local.env".

```sh
# change to this directory
cd /path/to/lv2-filter-cmake/

# setup project for building (-S specifies cmake project folder, -B specifies build output folder)
cmake -S . -B build

# build the project
$(which cmake) --build build

# copy over lv2 bundles into Anagram
scp -O -r build/*.lv2 root@192.168.51.1:/root/.lv2/

# restart Anagram system services that use plugins
ssh root@192.168.51.1 "systemctl restart jack2 lvgl-app"
```
