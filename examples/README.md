# Audio Plugin Development Documentation for Darkglass Anagram

This folder contains audio plugin examples with Anagram-specific integration and in different plugin frameworks.

## Gain plugin examples

We have 3 examples that work exactly the same way as our internal "Gain" plugin in the Utilities category, but each using their own framework.

- [dpf-gain-cmake](dpf-gain-cmake) - using [DPF](https://github.com/DISTRHO/DPF/)
- [juce-gain-cmake](juce-gain-cmake) - using [JUCE](https://juce.com/)
- [rust-lv2-gain](rust-lv2-gain) - using [rust-lv2](https://rustaudio.github.io/rust-lv2/)

These examples all have the following controls:
 - Enable/Bypass (with smoothing)
 - Gain (-40 to 20 dB range, with smoothing)
 - Invert Polarity (without smoothing)

These serve as the most simple possible plugins that meet Anagram requirements:
 - have enable/bypass control
 - have parameter smoothing
 - use mono IO (dual-mono version would produce the same sound)
 - contain name abbreviation
 - contain block images

## Custom Extension examples

These examples use custom Darkglass LV2 extensions, written in raw LV2 instead of a higher-level framework.

- [lv2-filter-cmake](lv2-filter-cmake)

This plugin implements our custom [Control Port State Update](https://github.com/Darkglass-Electronics/LV2-Extensions/blob/main/dg-control-port-state-update.lv2/control-port-state-update.h) extension to indicate that one parameter is inactive when a different one is set to "Off".
