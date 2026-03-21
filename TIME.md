# Audio Plugin Documentation for Darkglass Anagram

This document contains information on using time or tempo on the [Darkglass Anagram](https://www.darkglass.com/products/anagram/).

## Supported time values

The Anagram does contain a "timeline" like a typical plugin host or DAW, and so the possible time-related values given to plugins is quite limited.

There is no real "play" or "rolling" state, in LV2 this is typically denoted by [time:speed](http://lv2plug.in/ns/ext/time#speed) which for Anagram will always be 0 meaning "stopped".

The supported time-related values are:

- [time:beatsPerMinute](http://lv2plug.in/ns/ext/time#beatsPerMinute) in 40 to 300 range

And the following values are fixed:

- [time:bar](http://lv2plug.in/ns/ext/time#bar) is always 0
- [time:barBeat](http://lv2plug.in/ns/ext/time#barBeat) is always 0
- [time:beat](http://lv2plug.in/ns/ext/time#beat) is always 0
- [time:beatUnit](http://lv2plug.in/ns/ext/time#beatUnit) is always 4
- [time:beatsPerBar](http://lv2plug.in/ns/ext/time#beatsPerBar) is always 4
- [time:frame](http://lv2plug.in/ns/ext/time#frame) is always 0
- [time:speed](http://lv2plug.in/ns/ext/time#speed) is always 0
- [kx:TimePositionTicksPerBeat](http://kxstudio.sf.net/ns/lv2ext/props#TimePositionTicksPerBeat) is always 0

## Control port designations

The simplest way to access time-related information is through control port designations.  
This is a regular control port used for parameters but hidden from the user and automatically in sync with host values.

The supported time-related control port designations are:

- [time:beatsPerBar](http://lv2plug.in/ns/ext/time#beatsPerBar) in 1 to 16 range, always 4
- [time:beatsPerMinute](http://lv2plug.in/ns/ext/time#beatsPerMinute) in 40 to 300 range
- [time:speed](http://lv2plug.in/ns/ext/time#speed) boolean, always 0

## Time position in atom ports

The more complete (and also more standard) way of receiving time-related values for plugins is through [time:Position](http://lv2plug.in/ns/ext/time#Position) messages inside an [Atom port](http://lv2plug.in/ns/ext/atom#AtomPort).

Because Anagram only supports BPM for now, going this route might be unnecessary work.  
That said, most plugin frameworks (like DPF or JUCE) implement this method by default instead of relying on control port designations.

Documentation for this feature is outside the scope of this document.  
Refer to [github.com/lv2/lv2-examples/eg-metro.lv2](https://github.com/lv2/lv2-examples/tree/main/plugins/eg-metro.lv2) for an official LV2 example plugin that implements it.
