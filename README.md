# Audio Plugin Development Documentation for Darkglass Anagram

This repository contains documentation and examples related to developing audio plugins for [Darkglass Anagram](https://www.darkglass.com/products/anagram/) as a platform.

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

## Under the hood

Developing for Anagram means developing for a Linux-embed arm64 multi-core system.  
Inside there is a Linux 6.1 kernel with PREEMPT_RT with glibc 2.34.

For audio [JACK2](https://jackaudio.org/) runs as the audio server, with [mod-host](https://github.com/mod-audio/mod-host/) on top taking care of plugin loading and management.  
On top we have our custom UI based on [LVGL](https://lvgl.io/), running on a separate process from the audio server and plugin host.

The only supported plugin format at the moment is [LV2](https://lv2plug.in/).  
We have a few LV2 Extensions at [github.com/Darkglass-Electronics/LV2-Extensions](https://github.com/Darkglass-Electronics/LV2-Extensions/).

## Building

See [BUILDING.md](BUILDING.md) for instructions on how to build your plugin binaries.

## Caveats

### Control Ports only for now

While mod-host supports both LV2 Control Ports and LV2 Patch Parameters, Anagram only properly supports Control Ports at the moment.  
The only supported use of LV2 Parameters right now is in regards of loading of files through Atom:Path, as used by the Neural Loaders and User Cabinet plugin "blocks".

### No display server

While the system has a screen with custom graphics, there is no display server like X11 or Wayland inside the OS.  
There are no OpenGL, X11 or Wayland related libraries in the OS.  
If your plugin binary links against libX11.so for example, it will simply fail to load.

### No parameter text

Some plugin formats like CLAP and VST make use of parameter "text", rendered by the plugin each time the host needs a string representation of an arbitrary value.  
This allows plugins to define their parameters in a normalized 0.0 <-> 1.0 range, which is fully opaque to the host.  
In such cases the host needs to ask the plugin how an arbitrary parameter value is supposed to be represented/shown in "text" form.

Due to the architecture used inside Anagram - the UI/DSP separation - this setup of asking the plugin what is the text representation of a parameter value is not supported.  
The UI side intentionally does not have access to the plugin object.

For Anagram, plugin parameters SHOULD expose their full ranges in the LV2 meta-data.

## JUCE

A special note needs to be given about JUCE.  
While it does support exporting LV2 plugins, it uses "Patch Parameters" which is not supported in Anagram at the moment.

We are developing a custom alternative LV2 wrapper for JUCE that uses Control Ports plus:

 - disables regular X11 UI (we do not use it)
 - only enables the specific features in use by the plugin (e.g. Atom port for time events)
 - has custom Anagram-specific parameter hints
 - is able to get updates on a regular basis without having to update the entire JUCE codebase
