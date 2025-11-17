# Audio Plugin Development Documentation for Darkglass Anagram

This repository contains documentation and examples related to developing audio plugins for [Darkglass Anagram](https://www.darkglass.com/products/anagram/) as a platform.

## Caveats

### Control Ports only for now

While mod-host supports both LV2 Control Ports and LV2 Patch Parameters, Anagram only properly supports Control Ports at the moment.  
The only supported use of LV2 Parameters right now is in regards of loading of files through Atom:Path, as used by the Neural Loaders and User Cabinet plugin "blocks".

### No display server

While the system has a screen with custom graphics, there is no display server like X11 or Wayland inside the OS.  
There are no OpenGL, Vulkan, X11 or Wayland related libraries in the OS.  
If your plugin binary links against `libX11.so` for example, it will simply fail to load.

### No GPU

The Anagram hardware does not contain a GPU, so accelerated graphics are not possible.  
There is a 2D accelerator chip inside, but not enabled at the moment.

We set up LVGL to use 4 rendering threads, matching the 4 "little" cores present on the RK3582 SoM used by the Anagram.

### No parameter text

Some plugin formats like CLAP and VST make use of parameter "text", rendered by the plugin each time the host needs a string representation of an arbitrary value.  
This allows plugins to define their parameters in a normalized 0.0 <-> 1.0 range, which is fully opaque to the host.  
In such cases the host needs to ask the plugin how an arbitrary parameter value is supposed to be represented/shown in "text" form and vice-versa.

Due to the architecture used inside Anagram - the UI/DSP separation - this setup of asking the plugin what is the text representation of a parameter value is not supported.  
The UI side intentionally does not have access to the plugin object.

For Anagram, plugin parameters SHOULD expose their full ranges in the LV2 meta-data.
