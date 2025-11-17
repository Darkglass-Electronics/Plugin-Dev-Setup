# Audio Plugin Development Setup for Darkglass Anagram

This repository contains documentation and examples related to developing audio plugins for [Darkglass Anagram](https://www.darkglass.com/products/anagram/) as a platform.

## LV2

Audio plugins created for Anagram must be in [LV2](https://lv2plug.in/) format.  
Dealing with LV2 specifics is outside the scope of this documentation, though we provide source-code examples that can be used as starting point or as reference implementation.

Darkglass has a few custom LV2 properties to extend the plugin format to suit Anagram UI/UX.  
All the examples provide in this repository implement these custom properties.

There are no special licensing requirements for making LV2 plugins.

So far these plugin frameworks official support exporting as LV2:

 - [DPF](https://github.com/DISTRHO/DPF/)
 - [JUCE](https://juce.com/)

It is also possible to create LV2 plugins using [rust](https://docs.rs/lv2/latest/lv2/).

## Documentation

See [BUILDING.md](BUILDING.md) for instructions on how to build your plugin binaries.

See [CATEGORIES.md](CATEGORIES.md) for the list of plugin categories used in Anagram and how it translates from LV2 ones.

See [CAVEATS.md](CAVEATS.md) for a few caveats to be aware of when developing plugins for Anagram.

See [LICENSING.md](LICENSING.md) for details on licensing and IP protection.

See [LV2-FEATURES.md](LV2-FEATURES.md) for the list of LV2 features supported by Anagram.

See [MULTI-THREADING.md](MULTI-THREADING.md) for how to properly handle multi-threading for Anagram, if necessary.

See [REQUIREMENTS.md](REQUIREMENTS.md) for details on how we expect plugins to behave.

## Under the hood

Developing for Anagram means developing for a Linux-embed ARMv8.2 64-bit multi-core system, with 4 "little" CPU cores dedicated to system and graphics and 2 "big" CPU cores dedicated to audio.  
Inside there is a Linux 6.1 PREEMPT_RT kernel with (g)libc 2.34.

For audio [JACK2](https://jackaudio.org/) runs as the audio server, with [mod-host](https://github.com/mod-audio/mod-host/) on top taking care of plugin loading and management.  
Then on top we have our custom UI based on [LVGL](https://lvgl.io/), running on a separate process from the audio server and plugin host.

The only supported plugin format at the moment is [LV2](https://lv2plug.in/).  
We have a few LV2 Extensions at [github.com/Darkglass-Electronics/LV2-Extensions](https://github.com/Darkglass-Electronics/LV2-Extensions/).

## JUCE

A special note needs to be given about JUCE.  
While it does officially support exporting LV2 plugins since version 7, the official implementation uses "Patch Parameters" which is not supported in Anagram at the moment.

We have developed a custom, alternative LV2 wrapper for JUCE that uses old-style Control Ports plus:

 - disables regular desktop UI (we do not use it)
 - only enables the specific features in use by the plugin (e.g. Atom port only enabled if plugin needs time events)
 - has custom Anagram-specific plugin and parameter meta-data
 - is able to get updates on a regular basis without having to update the entire JUCE codebase

This custom LV2 wrapper is available at [github.com/Darkglass-Electronics/juce-anagram-lv2](https://github.com/Darkglass-Electronics/juce-anagram-lv2/).  
There is a JUCE example using this wrapper plugin under [examples](examples/).
