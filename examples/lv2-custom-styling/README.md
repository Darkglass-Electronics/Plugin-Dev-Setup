# Styling plugin example

This is an example plugin that demonstrates custom block image and block settings styling.

All the parameters are fake, the plugin won't do any audio processing.

For coverage reasons the plugin provides 1 parameter of each type supported in Anagram:

 1. Lists (by use of `lv2:enumeration` port property together with scalepoints)
 2. Toggle (by use of `lv2:toggled` port property)
 3. Integer (by use of `lv2:integer` port property)
 4. Regular floats (for cases without the port properties mentioned above)
 5. Output/metering (the lv2 control port is output instead of input, reporting values to host on every run)

## Building and deploy

Note: this step assumes you either are using the docker shell or have imported the build environment through "local.env".

```sh
# change to this directory
cd /path/to/lv2-custom-styling/

# setup project for building (-S specifies cmake project folder, -B specifies build output folder)
cmake -S . -B build

# build the project
$(which cmake) --build build

# copy over lv2 bundles into Anagram
scp -O -r build/*.lv2 root@192.168.51.1:/root/.lv2/

# restart Anagram system services that use plugins
ssh root@192.168.51.1 "systemctl restart jack2 lvgl-app"
```
