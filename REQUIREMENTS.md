# Audio Plugin Development Documentation for Darkglass Anagram

This document contains information on how plugins should behave when on the [Darkglass Anagram](https://www.darkglass.com/products/anagram/).  
A basic set of rules to follow in order to ensure consistency within the platform.

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

## IO

- Plugins MUST always have 1in-1out (mono) or 2in-2out (stereo).
  NOTE: Anagram will automatically load 2 instances of mono plugins if there are stereo outputs in preceding signal chain.

- Plugins CAN have both mono and stereo variants exposed IFF the stereo version sounds different than dual-mono.
  LV2 does not provide direct support for this, so we have our internal solution for this. Please contact us if you have such a plugin.

- 

## Parameters

- Parameter MUST have smoothing when relevant (filters, gain, etc)

- LV2 "enabled" designation MUST be implemented, for smooth bypass on/off

- LV2 "reset" designation MUST be implemented for the plugins where it makes sense (like delays, so that changing presets clears old buffers)

- Parameters SHOULD be grouped into sets of 6
  Anagram "block settings" screen shows 6 parameters at a time in a paginated fashion

