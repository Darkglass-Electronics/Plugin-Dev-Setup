# Audio Plugin Documentation for Darkglass Anagram

This document contains information on what LV2 host features are implemented on the [Darkglass Anagram](https://www.darkglass.com/products/anagram/).  
Most of official extensions are implemented, with a few custom ones on top as well.

## Host features

The following host features (passed during plugin instantiation) are supported:

> NOTE: these are feature URIs, not always valid URLs.

#### [bufsz:boundedBlockLength](http://lv2plug.in/ns/ext/buf-size#boundedBlockLength)

Host provides block length bounds information.

#### [bufsz:fixedBlockLength](http://lv2plug.in/ns/ext/buf-size#fixedBlockLength)

Host uses fixed-size buffers for audio processing.

#### [bufsz:powerOf2BlockLength](http://lv2plug.in/ns/ext/buf-size#powerOf2BlockLength)

Host uses a power-of-2 block size.

#### [log:log](http://lv2plug.in/ns/ext/log#log)

Host provides custom logging mechanism.  
Not implemented yet, but later on plugin logs will appear on a dedicated screen.

#### [options:options](http://lv2plug.in/ns/ext/options#options)

Host provides some LV2 information to the plugin through the use of "options".

The following options are supported:

###### [bufsz:minBlockLength](http://lv2plug.in/ns/ext/buf-size#minBlockLength)

Host buffers minimum block length, fixed to 16 frames in Anagram.

###### [bufsz:maxBlockLength](http://lv2plug.in/ns/ext/buf-size#maxBlockLength)

Host buffers maximum block length, fixed to 16 frames in Anagram.

###### [bufsz:nomimalBlockLength](http://lv2plug.in/ns/ext/buf-size#nomimalBlockLength)

Host buffers nomimal block length, fixed to 16 frames in Anagram.

###### [threads:schedPolicy](http://ardour.org/lv2/threads/#schedPolicy)

Scheduler policy for audio-related plugin threads, fixed to `SCHED_FIFO` in Anagram.

###### [threads:schedPriority](http://ardour.org/lv2/threads/#schedPriority)

Scheduler priority for audio-related plugin threads, fixed to 56 in Anagram.

###### [jack:client](http://jackaudio.org/metadata/client)

Direct access to the underlying JACK client used by the host for a specific plugin.  
For very advanced usecases only!

<!--
#### http://lv2plug.in/ns/ext/state#freePath

Host provides a host-side function to clear memory created during `state:makePath`.

#### http://lv2plug.in/ns/ext/state#makePath

unused in Anagram so far
-->

#### [urid:map](http://lv2plug.in/ns/ext/urid#map)

Host provides a way to map a URI string into a URID identifier.

#### [urid:unmap](http://lv2plug.in/ns/ext/urid#unmap)

Host provides a way to "unmap" a URID identifier back to its URI string.

#### [worker:interface](http://lv2plug.in/ns/ext/worker#interface)

Host provides worker threads that can be used by plugins in a thread-safe way.

#### http://www.darkglass.com/lv2/ns/lv2ext/control-port-state-update

Custom LV2 extension from Darkglass to deactivate and/or block certain parameters from being used.  
This allows, for example, for one parameter to turn off another, creating a link between them.

See [examples/lv2-filter-cmake/](examples/lv2-filter-cmake) for an example plugin that uses this feature.

#### http://kx.studio/ns/lv2ext/control-input-port-change-request

Custom LV2 extension from the KXStudio project that allows a plugin to request changes to the value of a control input port.

#### http://moddevices.com/ns/ext/license#feature

Custom LV2 extension from the MOD Devices / MOD Audio project for fetching a commercial plugin license file.

## Parameter designations

The following control port designations are supported:

> NOTE: the href target is the designation URI, not always a valid URL.

#### [lv2:enabled](http://lv2plug.in/ns/lv2core#enabled)

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

<!--
#### [lv2:freeWheeling](http://lv2plug.in/ns/lv2core#freeWheeling)

Boolean for offline rendering state; 1 means offline, 0 means realtime; unused in Anagram so far

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
-->

#### [kx:Reset](http://kxstudio.sf.net/ns/lv2ext/props#Reset)

Trigger for clearing any state memory (buffers, lfo phase etc) and setting any smoothed parameters directly to their target values.

Triggered with value 1 in these cases:
 - during preset change within bank (block already loaded and possibly run earlier)
 - when a second synced lv2 instance is added to serve as a dual mono pair

Special rules:
 - Must be trigger
 - Must have default and minimum 0

Recommendations:
 - During lv2_run apply reset after applying parameter changes so that parameter smoothing is actually skipped
 - Apply reset in plugin internally during activate()
 - activate() may be followed by reset on the first lv2_run. Keep track of cleared buffers especially to avoid unnecessary re-clearing of big buffers in these situations.

```ttl
@prefix lv2:    <http://lv2plug.in/ns/lv2core#> .
@prefix pprops: <http://lv2plug.in/ns/ext/port-props#> .
@prefix kx:     <http://kxstudio.sf.net/ns/lv2ext/props#> .
# ...
lv2:port [
  a lv2:InputPort, lv2:ControlPort ;
  lv2:index 3 ;
  lv2:symbol "reset" ;
  lv2:name "Reset" ;
  lv2:designation kx:Reset ;
  lv2:portProperty lv2:integer , pprops:trigger ;
  lv2:default 0 ;
  lv2:minimum 0 ;
  lv2:maximum 1 ;
] ;
```

<!--
#### [time:beatsPerBar](http://lv2plug.in/ns/ext/time#beatsPerBar)

Integer value in 1-16 range for beats per bar or time signature "numerator"; unused in Anagram so far

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
-->

#### [time:beatsPerMinute](http://lv2plug.in/ns/ext/time#beatsPerMinute)

Regular float value in 40-300 range for beats per bar

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

<!--
#### [time:speed](http://lv2plug.in/ns/ext/time#speed)

Regular float for playback speed but always 0 or 1; 0 means "stopped", 1 means "rolling"; unused in Anagram so far

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
-->

## Parameter properties

The following control port and/or parameter properties are supported:

> NOTE: the href target is the property URI, not always a valid URL.

#### [lv2:toggled](http://lv2plug.in/ns/lv2core#toggled)

Makes the parameter appear as a on/off toggle

#### [lv2:integer](http://lv2plug.in/ns/lv2core#integer)

Makes the parameter always use real numbers, so advance/decrease in whole steps.

#### [lv2:enumeration](http://lv2plug.in/ns/lv2core#enumeration)

Makes the parameter use a list for selecting the value, restricting it only to the exposed scale points.

#### [pprops:expensive](http://lv2plug.in/ns/ext/port-props#expensive)

Prevents the parameter from being used in scenes.

#### [pprops:logarithmic](http://lv2plug.in/ns/ext/port-props#logarithmic)

Makes the parameter advance/decrease in logarithmic scale.

The parameter MUST NOT have 0 as a valid value in its range.

#### [pprops:notOnGUI](http://lv2plug.in/ns/ext/port-props#notOnGUI)

Hides the parameter from the user and does not save it as part of the preset.

#### [dg:savedToPreset](http://www.darkglass.com/lv2/ns#savedToPreset)

Ensures the parameter is always saved to preset, even if hidden.

## Parameter units

The following control port and/or parameter units are supported:

> NOTE: the href target is the unit URI, not always a valid URL.

| URI                                                                        | Name              | printf     | Notes                                 |
|----------------------------------------------------------------------------|-------------------|------------|---------------------------------------|
| [units:s](http://lv2plug.in/ns/extensions/units#s)                         | Seconds           | `%f s`     |                                       |
| [units:ms](http://lv2plug.in/ns/extensions/units#ms)                       | Milliseconds      | `%f ms`    |                                       |
| [units:db](http://lv2plug.in/ns/extensions/units#db)                       | Decibels          | `%.1f dB`  |                                       |
| [units:pc](http://lv2plug.in/ns/extensions/units#pc)                       | Percentage        | `%f %%`    |                                       |
| [units:hz](http://lv2plug.in/ns/extensions/units#hz)                       | Hertz             | (variable) | Uses `Hz` if < 1000, otherwise `kHz`  |
| [units:khz](http://lv2plug.in/ns/extensions/units#khz)                     | Kilohertz         | `%f kHz`   |                                       |
| [units:mhz](http://lv2plug.in/ns/extensions/units#mhz)                     | Megahertz         | `%f MHz`   |                                       |
| [units:cent](http://lv2plug.in/ns/extensions/units#cent)                   | Cent              | `%f ct`    |                                       |
| [units:semitone12TET](http://lv2plug.in/ns/extensions/units#semitone12TET) | Semitone          | `%f semi`  |                                       |
| [dg:oneDecimalPoint](http://www.darkglass.com/lv2/ns#oneDecimalPoint)      | One Decimal Point | `%.1f`     | Special unit to force 1 decimal point |
