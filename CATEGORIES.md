# Audio Plugin Documentation for Darkglass Anagram

This document contains information on plugin categories implemented on the [Darkglass Anagram](https://www.darkglass.com/products/anagram/).

The Anagram has the following plugin categories:

 - Amp & Cabs
 - Overdrive & Distortion
 - EQs, Filters & Dynamics
 - Modulation & Pitch
 - Delays & Reverbs
 - Utilities

Anagram does not provide a way for plugins to specifically set one of these,
instead it translates/converts the LV2 plugin category into one of the 6 above.

The conversion is as follows:

 - [lv2:AmplifierPlugin](http://lv2plug.in/ns/lv2core#AmplifierPlugin) -> Amp & Cabs
 - [lv2:SimulatorPlugin](http://lv2plug.in/ns/lv2core#SimulatorPlugin) -> Amp & Cabs
 - [lv2:DistortionPlugin](http://lv2plug.in/ns/lv2core#DistortionPlugin) -> Overdrive & Distortion
 - [lv2:WaveshaperPlugin](http://lv2plug.in/ns/lv2core#WaveshaperPlugin) -> Overdrive & Distortion
 - [lv2:DynamicsPlugin](http://lv2plug.in/ns/lv2core#DynamicsPlugin) -> EQs, Filters & Dynamics
 - [lv2:CompressorPlugin](http://lv2plug.in/ns/lv2core#CompressorPlugin) -> EQs, Filters & Dynamics
 - [lv2:EnvelopePlugin](http://lv2plug.in/ns/lv2core#EnvelopePlugin) -> EQs, Filters & Dynamics
 - [lv2:ExpanderPlugin](http://lv2plug.in/ns/lv2core#ExpanderPlugin) -> EQs, Filters & Dynamics
 - [lv2:GatePlugin](http://lv2plug.in/ns/lv2core#GatePlugin) -> EQs, Filters & Dynamics
 - [lv2:LimiterPlugin](http://lv2plug.in/ns/lv2core#LimiterPlugin) -> EQs, Filters & Dynamics
 - [lv2:FilterPlugin](http://lv2plug.in/ns/lv2core#FilterPlugin) -> EQs, Filters & Dynamics
 - [lv2:AllpassPlugin](http://lv2plug.in/ns/lv2core#AllpassPlugin) -> EQs, Filters & Dynamics
 - [lv2:BandpassPlugin](http://lv2plug.in/ns/lv2core#BandpassPlugin) -> EQs, Filters & Dynamics
 - [lv2:CombPlugin](http://lv2plug.in/ns/lv2core#CombPlugin) -> EQs, Filters & Dynamics
 - [lv2:EQPlugin](http://lv2plug.in/ns/lv2core#EQPlugin) -> EQs, Filters & Dynamics
 - [lv2:MultiEQPlugin](http://lv2plug.in/ns/lv2core#MultiEQPlugin) -> EQs, Filters & Dynamics
 - [lv2:ParaEQPlugin](http://lv2plug.in/ns/lv2core#ParaEQPlugin) -> EQs, Filters & Dynamics
 - [lv2:HighpassPlugin](http://lv2plug.in/ns/lv2core#HighpassPlugin) -> EQs, Filters & Dynamics
 - [lv2:LowpassPlugin](http://lv2plug.in/ns/lv2core#LowpassPlugin) -> EQs, Filters & Dynamics
 - [lv2:ModulatorPlugin](http://lv2plug.in/ns/lv2core#ModulatorPlugin) -> Modulation & Pitch
 - [lv2:ChorusPlugin](http://lv2plug.in/ns/lv2core#ChorusPlugin) -> Modulation & Pitch
 - [lv2:FlangerPlugin](http://lv2plug.in/ns/lv2core#FlangerPlugin) -> Modulation & Pitch
 - [lv2:PhaserPlugin](http://lv2plug.in/ns/lv2core#PhaserPlugin) -> Modulation & Pitch
 - [lv2:SpectralPlugin](http://lv2plug.in/ns/lv2core#SpectralPlugin) -> Modulation & Pitch
 - [lv2:PitchPlugin](http://lv2plug.in/ns/lv2core#PitchPlugin) -> Modulation & Pitch
 - [lv2:DelayPlugin](http://lv2plug.in/ns/lv2core#DelayPlugin) -> Delays & Reverbs
 - [lv2:ReverbPlugin](http://lv2plug.in/ns/lv2core#ReverbPlugin) -> Delays & Reverbs

Everything else will appear under Utilities.
