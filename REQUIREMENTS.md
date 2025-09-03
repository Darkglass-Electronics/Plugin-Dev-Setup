# Audio Plugin Documentation for Darkglass Anagram

This document contains information on how plugins should behave when on the [Darkglass Anagram](https://www.darkglass.com/products/anagram/).  
A basic set of rules to follow in order to ensure consistency within the platform.

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

## IO

- Plugins MUST always have 1in-1out (mono) or 2in-2out (stereo) or 1in-2out.  
  Anagram will automatically load 2 instances of mono plugins if there are stereo outputs in preceding signal chain.

- Plugins CAN have both mono and stereo variants exposed IFF the stereo version sounds different than dual-mono.  
  LV2 does not provide support for dynamic IO, we have our internal solution for this by exposing mono and stereo variants of the same plugin.  
  Please contact us if you have such a plugin.

## Parameters

- Parameters MUST have smoothing when relevant (filters, gain, etc)

- LV2 "enabled" designation MUST be implemented as a control port, for smooth bypass on/off

- LV2 "enabled" designated control port MUST have its default value as 1 for "on"

- LV2 "reset" designation SHOULD be implemented as a control port for the plugins where it makes sense (like delays, so that changing presets clears old buffers)

- Parameters SHOULD be grouped into sets of 6  
  Anagram "block settings" screen shows 6 parameters at a time in a paginated fashion

## Parameter changes

- Plugins MUST NOT change global CPU registers (like "flush to zero"), the host already has them set

- Plugins MUST NOT change the "enabled"/"bypass" control on their own

- Plugins SHOULD try to avoid changing parameters on their own, as it is likely to mess with Scenes
