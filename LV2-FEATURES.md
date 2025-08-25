# Audio Plugin Development Documentation for Darkglass Anagram

This document contains information on what LV2 host features are implemented on the [Darkglass Anagram](https://www.darkglass.com/products/anagram/).  
Most of official extensions are implemented, with a few custom ones on top as well.

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

## Parameter designations

The following control port designations are supported:

> NOTE: the href target is the designation URI, not always a valid URL.

- [lv2:enabled](http://lv2plug.in/ns/lv2core#enabled)

Boolean for bypass/enabled state; 1 means enabled, 0 means bypassed

Special rules:
 - Must have minimum 0
 - Must have default and maximum 1

```ttl
@prefix lv2: <http://lv2plug.in/ns/lv2core#> .
# ...
lv2:port [
  a lv2:InputPort, lv2:ControlPort ;
  lv2:index 3 ;
  lv2:symbol "enabled" ;
  lv2:name "Enabled" ;
  lv2:designation lv2:enabled ;
  lv2:portProperty lv2:integer , lv2:toggled ;
  lv2:default 1 ;
  lv2:minimum 0 ;
  lv2:maximum 1 ;
] ;
```

- [lv2:freeWheeling](http://lv2plug.in/ns/lv2core#freeWheeling)

boolean for offline rendering state; 1 means offline, 0 means realtime; unused in Anagram so far

Special rules:
 - Must have default and minimum 0
 - Must have maximum 1

```ttl
@prefix lv2: <http://lv2plug.in/ns/lv2core#> .
# ...
lv2:port [
  a lv2:InputPort, lv2:ControlPort ;
  lv2:index 3 ;
  lv2:symbol "freeWheeling" ;
  lv2:name "Free Wheeling" ;
  lv2:designation lv2:freeWheeling ;
  lv2:portProperty lv2:integer , lv2:toggled ;
  lv2:default 0 ;
  lv2:minimum 0 ;
  lv2:maximum 1 ;
] ;
```

- [kxprops:Reset](http://kxstudio.sf.net/ns/lv2ext/props#Reset) trigger for clearing old buffers when changing presets

Special rules:
 - Must be trigger
 - Must have default and minimum 0

```ttl
@prefix lv2:     <http://lv2plug.in/ns/lv2core#> .
@prefix pprops:  <http://lv2plug.in/ns/ext/port-props#> .
@prefix kxprops: <http://kxstudio.sf.net/ns/lv2ext/props#> .
# ...
lv2:port [
  a lv2:InputPort, lv2:ControlPort ;
  lv2:index 3 ;
  lv2:symbol "reset" ;
  lv2:name "Reset" ;
  lv2:designation kxprops:Reset ;
  lv2:portProperty lv2:integer , pprops:trigger ;
  lv2:default 0 ;
  lv2:minimum 0 ;
  lv2:maximum 1 ;
] ;
```

- [time:beatsPerBar](http://lv2plug.in/ns/ext/time#beatsPerBar)

integer value in 1-16 range for beats per bar or time signature "numerator"; unused in Anagram so far

```ttl
@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .
@prefix time: <http://lv2plug.in/ns/ext/time#> .
# ...
lv2:port [
  a lv2:InputPort, lv2:ControlPort ;
  lv2:index 3 ;
  lv2:symbol "beatsPerBar" ;
  lv2:name "Beats Per Bar" ;
  lv2:designation time:beatsPerBar ;
  lv2:portProperty lv2:integer ;
  lv2:default 4 ;
  lv2:minimum 1 ;
  lv2:maximum 16 ;
] ;
```

- [time:beatsPerMinute](http://lv2plug.in/ns/ext/time#beatsPerMinute)

regular float value in 40-300 range for beats per bar

```ttl
@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .
@prefix time: <http://lv2plug.in/ns/ext/time#> .
# ...
lv2:port [
  a lv2:InputPort, lv2:ControlPort ;
  lv2:index 3 ;
  lv2:symbol "beatsPerMinute" ;
  lv2:name "Beats Per Minute" ;
  lv2:designation time:beatsPerMinute ;
  lv2:default 40 ;
  lv2:minimum 120 ;
  lv2:maximum 300 ;
] ;
```

- [time:speed](http://lv2plug.in/ns/ext/time#speed)

regular float for playback speed but always 0 or 1; 0 means "stopped", 1 means "rolling"; unused in Anagram so far

Special rules:
 - Must have default 0

```ttl
@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .
@prefix time: <http://lv2plug.in/ns/ext/time#> .
# ...
lv2:port [
  a lv2:InputPort, lv2:ControlPort ;
  lv2:index 3 ;
  lv2:symbol "speed" ;
  lv2:name "Speed" ;
  lv2:designation time:speed ;
  lv2:default 0 ;
  lv2:minimum 0 ;
  lv2:maximum 1 ;
] ;
```

## Parameter properties

The following control port and/or parameter properties are supported:

> NOTE: the href target is the designation URI, not always a valid URL.

- [lv2:toggled](http://lv2plug.in/ns/lv2core#toggled)

Makes the parameter appear as a on/off toggle

- [lv2:integer](http://lv2plug.in/ns/lv2core#integer)

Makes the parameter always use real numbers, so advance/decrease in whole steps.

- [lv2:enumeration](http://lv2plug.in/ns/lv2core#enumeration)

Makes the parameter use a list for selecting the value, restricting it only to the exposed scale points.

- [pprops:logarithmic](http://lv2plug.in/ns/ext/port-props#logarithmic)

Makes the parameter advance/decrease in logarithmic scale.

The parameter MUST NOT have 0 as a valid value in its range.

- [pprops:notOnGUI](http://lv2plug.in/ns/ext/port-props#notOnGUI)

Hides the parameter from the user and does not save it as part of the preset.

## Parameter units

The following control port and/or parameter units are supported:

> NOTE: the href target is the designation URI, not always a valid URL.

|----------------------------------------------------------------------------|---------------------|------------|--------------------------------------|
| URI                                                                        | Name                | printf     | Notes                                |
| [units:s](http://lv2plug.in/ns/extensions/units#s)                         | Seconds             | `%f s`     |                                      |
| [units:ms](http://lv2plug.in/ns/extensions/units#ms)                       | Milliseconds        | `%f ms`    |                                      |
| [units:db](http://lv2plug.in/ns/extensions/units#db)                       | Decibels            | `%.1f dB`  |                                      |
| [units:pc](http://lv2plug.in/ns/extensions/units#pc)                       | Percentage          | `%f %%`    |                                      |
| [units:hz](http://lv2plug.in/ns/extensions/units#hz)                       | Hertz               | (variable) | uses `Hz` if < 1000, otherwise `kHz` |
| [units:khz](http://lv2plug.in/ns/extensions/units#khz)                     | Kilohertz           | `%f MHz`   |                                      |
| [units:mhz](http://lv2plug.in/ns/extensions/units#mhz)                     | Megahertz           | `%f MHz`   |                                      |
| [units:cent](http://lv2plug.in/ns/extensions/units#cent)                   | Cent                | `%f ct`    |                                      |
| [units:semitone12TET](http://lv2plug.in/ns/extensions/units#semitone12TET) | Semitone            | `%f semi`  |                                      |
| [dgprops:oneDecimalPoint](http://www.darkglass.com/lv2/ns#oneDecimalPoint) | "One Decimal Point" | `%.1f`     |                                      |
