# Audio Plugin Development Setup for Darkglass Anagram

This repository contains documentation and examples related to developing audio plugins for [Darkglass Anagram](https://www.darkglass.com/products/anagram/) as a platform.

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

## Documentation

See [BUILDING.md](BUILDING.md) for instructions on how to build your plugin binaries.

See [CAVEATS.md](CAVEATS.md) for a few caveats to be aware of when developing plugins for Anagram.

See [LV2-FEATURES.md](LV2-FEATURES.md) for the list of LV2 features supported by Anagram.

See [REQUIREMENTS.md](REQUIREMENTS.md) for details on how we expect plugins to behave.

## Under the hood

Developing for Anagram means developing for a Linux-embed ARMv8.2 64-bit multi-core system, with 4 "little" CPU cores dedicated to system and graphics and 2 "big" CPU cores dedicated to audio.  
Inside there is a Linux 6.1 PREEMPT_RT kernel with glibc 2.34.

For audio [JACK2](https://jackaudio.org/) runs as the audio server, with [mod-host](https://github.com/mod-audio/mod-host/) on top taking care of plugin loading and management.  
On top we have our custom UI based on [LVGL](https://lvgl.io/), running on a separate process from the audio server and plugin host.

The only supported plugin format at the moment is [LV2](https://lv2plug.in/).  
We have a few LV2 Extensions at [github.com/Darkglass-Electronics/LV2-Extensions](https://github.com/Darkglass-Electronics/LV2-Extensions/).

## JUCE

A special note needs to be given about JUCE.  
While it does support exporting LV2 plugins, it uses "Patch Parameters" which is not supported in Anagram at the moment.

We are developing a custom alternative LV2 wrapper for JUCE that uses Control Ports plus:

 - disables regular X11 UI (we do not use it)
 - only enables the specific features in use by the plugin (e.g. Atom port for time events)
 - has custom Anagram-specific parameter hints
 - is able to get updates on a regular basis without having to update the entire JUCE codebase
