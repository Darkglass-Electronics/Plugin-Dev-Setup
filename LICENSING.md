# Audio Plugin Development Documentation for Darkglass Anagram

This repository contains documentation and examples related to developing audio plugins for [Darkglass Anagram](https://www.darkglass.com/products/anagram/) as a platform.

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

## IP protection

For dealing with IP protection and licensing, the Anagram has a built-in solution tied to its hardware components.  
This solution is available to developers as a static library, to be used during the build process.  
It uses a license-file mechanism protected by device-specific hardware-id and public/private key encryption.

Fetch the latest release of this library for Anagram [here](https://download.mod.audio/shared/libmodla-v1.3.1-pablito.tar.gz).

Inside you will find instructions and more details, including a minimal plugin example.

### Framework usage

If you are using DPF, you can simply add these lines to your `DistrhoPluginInfo.h` configuration file:

```C++
// enable MOD licensing for Anagram (codenamed "Pablito")
#ifdef _DARKGLASS_DEVICE_PABLITO
#define DISTRHO_PLUGIN_LICENSED_FOR_MOD 1
#endif
```

We will integrate this library API into our [custom JUCE LV2 wrapper](https://github.com/Darkglass-Electronics/juce-anagram-lv2.git) soon.

There is no way to use this licensing mechanism for Rust-based plugins for now.

## Incompatible open-source licenses

Plugins that use "viral" licenses such as GPL are not compatible with this library, as it is proprietary.

It is still possible to release open-source plugins for Anagram in any case.
