# Audio Plugin Development Documentation for Darkglass Anagram

This repository contains documentation and examples related to developing audio plugins for [Darkglass Anagram](https://www.darkglass.com/products/anagram/) as a platform.

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

## IP protection

For dealing with IP protection and licensing, the Anagram has a built-in solution tied to its hardware components.  
This solution is available to developers as a static library, to be used during the build process.  
It uses a license-file mechanism protected by device-specific hardware-id and public/private key encryption.

Fetch the latest release of this library for Anagram [here](https://github.com/Darkglass-Electronics/plugin-builder/tree/main/plugins-dep/package/libnickel).  
The static library to use depends on the compiler, just pick the one that matches. (gcc9 version is the default)

If you are using a plugin framework these details will be taken care of for you automatically,
otherwise you need to implement the API and instructions from `libnickel.h` manually.

## Freeware usage

We ask developers to always use this library, even for free plugins.  
This is so that the plugins can be protected against running outside of their target environment (say an Anagram binary running on a regular arm64 PC).

For free plugins the libnickel library must be setup in such a way so that it checks for not only a regular license file but also the Anagram device key.  
This makes it possible to run the plugins inside the Anagram "for free" (as it is intended) but prevent usage outside of Anagram (will run as trial/demo).

**The setup for this is already in place for DPF and JUCE frameworks**.  
If you roll your own framework or are coding directly against LV2, your plugin instantiation function should contain something like this:

```C++
#if defined(_DARKGLASS_DEVICE_PABLITO)
// check for license file against this plugin URI
if (! nickel_init(sampleRate, features, PLUGIN_URI))
{
    // if that fails, accept the Anagram device key (codenamed "pablito")
    // NOTE: remove this line if your plugin is commercial!
    nickel_init(sampleRate, features, "urn:darkglass:pablito");
}
#endif
```

Additionally your plugin ttl file should mention the license it accepts, matching the libnickel setup above, like so:

```ttl
@prefix licns: <http://www.darkglass.com/lv2/ns/lv2ext/license#> .
@prefix lv2:   <http://lv2plug.in/ns/lv2core#> .
# etc etc

<https://my-example-company.com/products/my-plugin-1>
    a lv2:UtilityPlugin, lv2:Plugin, doap:Project ;

    # always accept licenses for the plugin itself
    licns:uri <https://my-example-company.com/products/my-plugin-1> ;

    # if freeware also accept the Anagram device key
    licns:uri <urn:darkglass:pablito> ;

    # confirm to LV2 standards by defining these here
    lv2:extensionData licns:interface ;
    lv2:requiredFeature licns:feature ;

    # everything else as expected from an LV2 plugin
    doap:name "Example 1" ;
    # etc etc
```

### Framework usage

If you are using DPF make sure to checkout the `develop` branch instead of `main`.  
Then just set if your plugin is commercial or not in your `DistrhoPluginInfo.h` configuration file:

```C++
// 1 for commercial, 0 for freeware
#define DISTRHO_PLUGIN_IS_COMMERCIAL 1
```

For JUCE the setup is similar, but instead of a compiler macro it is an option in the `juce_anagram_lv2_setup` call.  
See [juce-anagram-lv2/CMakeLists.txt](https://github.com/Darkglass-Electronics/juce-anagram-lv2/blob/main/CMakeLists.txt#L27) for the possible options, `IS_FREEWARE` as boolean defines if freeware (true) or commercial (false).

There is no way to use this licensing mechanism for Rust-based plugins for now.

## Incompatible open-source licenses

Plugins that use "viral" licenses such as GPL are not compatible with this library, as it is proprietary.

It is still possible to release open-source plugins for Anagram in any case, they will simply go without any licensing protection.
