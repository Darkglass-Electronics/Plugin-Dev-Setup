# Audio Plugin Documentation for Darkglass Anagram

This document contains information on how plugins should behave when on the [Darkglass Anagram](https://www.darkglass.com/products/anagram/).  
A basic set of rules to follow in order to ensure consistency within the platform.

## Plugin meta-data

- Each Plugin MUST have a [dg:abbreviation](http://www.darkglass.com/lv2/ns#abbreviation) - a simple string consisting of 2 or 3 characters in uppercase.  
  This is used when referring to a plugin + parameter name in the bindings screen.  
  The abbreviation does not have to be globally unique.  
  Example:
```ttl
@prefix dg: <http://www.darkglass.com/lv2/ns#> .
<urn:darkglass:example>
  a lv2:Plugin, doap:Project ;
  doap:name "Gain" ;
  dg:abbreviation "GAI" ;
```

- Each Plugin MUST have [dg:blockImageOff](http://www.darkglass.com/lv2/ns#blockImageOff) and [dg:blockImageOn](http://www.darkglass.com/lv2/ns#blockImageOn) - 2 paths for 200x200 PNG image files stored inside the LV2 plugin bundle.  
  These images are shown on the signal chain and other screens and as the plugin "block" image.  
  Typically the "off" image is darkened compared to the "on" image.  
  Example:
```ttl
@prefix dg: <http://www.darkglass.com/lv2/ns#> .
<urn:darkglass:example>
  dg:blockImageOff <gain-off.png> ;
  dg:blockImageOn <gain-on.png> ;
```

## IO

- Plugins MUST always have 1in-1out (mono) or 2in-2out (stereo).  
  Anagram will automatically load 2 instances of mono plugins if there are stereo outputs in preceding signal chain.

- Plugins CAN have both mono and stereo variants exposed IFF the stereo version sounds different than dual-mono.  
  LV2 does not provide support for dynamic IO, we have our internal solution for this by exposing mono and stereo variants of the same plugin.  
  Please contact us if you have such a plugin.

## Parameters

- Parameters MUST have smoothing when relevant (filters, gain, etc)

- [lv2:enabled](http://lv2plug.in/ns/lv2core#enabled) designation MUST be implemented as an [lv2:ControlPort](http://lv2plug.in/ns/lv2core#ControlPort), for smooth bypass on/off

- [lv2:enabled](http://lv2plug.in/ns/lv2core#enabled) designated control port MUST have its default value as 1 for "on"

- [kx:Reset](http://kxstudio.sf.net/ns/lv2ext/props#Reset) designation SHOULD be implemented as a control port for the plugins where it makes sense (like delays, so that changing presets clears old buffers)

- Parameters SHOULD be grouped into sets of 6  
  Anagram "block settings" screen shows 6 parameters at a time in a paginated fashion

See [LV2-FEATURES.md#parameter-designations](LV2-FEATURES.md#parameter-designations) for more information about these special designations.

## Parameter changes

- Plugins MUST NOT change global CPU registers (like "flush to zero"), the host already has them set

- Plugins MUST NOT change the "enabled"/"bypass" control on their own

- Plugins SHOULD try to avoid changing parameters on their own, as it is likely to mess with Scenes
