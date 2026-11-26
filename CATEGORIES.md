# Audio Plugin Documentation for Darkglass Anagram

This document contains information on plugin categories implemented on the [Darkglass Anagram](https://www.darkglass.com/products/anagram/).

The Anagram has the following plugin categories:

 - Amp & Cabs
 - Overdrive & Distortion
 - EQs, Filters & Dynamics
 - Modulation & Pitch
 - Delays & Reverbs
 - Utilities

The Anagram plugin catagories differ from the standard LV2 categories. See this list bellow for all the catagories on the Anagram:

```
dg:AmpPlugin
dg:CabPlugin
dg:DelayPlugin
dg:DistortionPlugin
dg:DynamicPlugin
dg:EQPlugin
dg:FilterPlugin
dg:FuzzPlugin
dg:LoaderPlugin
dg:ModulationPlugin
dg:OverdrivePlugin
dg:PitchPlugin
dg:ReverbPlugin
dg:SynthPlugin
dg:UtilityPlugin
```

Every plugin should always include a standard lv2 category.
If no Dargkglass catogary is provided, Anagram translates/converts the LV2 plugin category into one of the 6 bellow.

 - [lv2:AmplifierPlugin](http://lv2plug.in/ns/lv2core#AmplifierPlugin) -> Amp & Cabs -> Amp
 - [lv2:SimulatorPlugin](http://lv2plug.in/ns/lv2core#SimulatorPlugin) -> Amp & Cabs -> Amp
 - [lv2:DistortionPlugin](http://lv2plug.in/ns/lv2core#DistortionPlugin) -> Overdrive & Distortion -> Distortion
 - [lv2:WaveshaperPlugin](http://lv2plug.in/ns/lv2core#WaveshaperPlugin) -> Overdrive & Distortion -> Distortion
 - [lv2:DynamicsPlugin](http://lv2plug.in/ns/lv2core#DynamicsPlugin) -> EQs, Filters & Dynamics -> Dynamic
 - [lv2:CompressorPlugin](http://lv2plug.in/ns/lv2core#CompressorPlugin) -> EQs, Filters & Dynamics -> Dynamic
 - [lv2:EnvelopePlugin](http://lv2plug.in/ns/lv2core#EnvelopePlugin) -> EQs, Filters & Dynamics -> Dynamic
 - [lv2:ExpanderPlugin](http://lv2plug.in/ns/lv2core#ExpanderPlugin) -> EQs, Filters & Dynamics -> Dynamic
 - [lv2:GatePlugin](http://lv2plug.in/ns/lv2core#GatePlugin) -> EQs, Filters & Dynamics -> Dynamic
 - [lv2:LimiterPlugin](http://lv2plug.in/ns/lv2core#LimiterPlugin) -> EQs, Filters & Dynamics -> Dynamic
 - [lv2:FilterPlugin](http://lv2plug.in/ns/lv2core#FilterPlugin) -> EQs, Filters & Dynamics -> Filter
 - [lv2:AllpassPlugin](http://lv2plug.in/ns/lv2core#AllpassPlugin) -> EQs, Filters & Dynamics -> Filter
 - [lv2:BandpassPlugin](http://lv2plug.in/ns/lv2core#BandpassPlugin) -> EQs, Filters & Dynamics -> Filter
 - [lv2:CombPlugin](http://lv2plug.in/ns/lv2core#CombPlugin) -> EQs, Filters & Dynamics -> Filter
 - [lv2:EQPlugin](http://lv2plug.in/ns/lv2core#EQPlugin) -> EQs, Filters & Dynamics -> Filter
 - [lv2:MultiEQPlugin](http://lv2plug.in/ns/lv2core#MultiEQPlugin) -> EQs, Filters & Dynamics -> Filter
 - [lv2:ParaEQPlugin](http://lv2plug.in/ns/lv2core#ParaEQPlugin) -> EQs, Filters & Dynamics -> Filter
 - [lv2:HighpassPlugin](http://lv2plug.in/ns/lv2core#HighpassPlugin) -> EQs, Filters & Dynamics -> Filter
 - [lv2:LowpassPlugin](http://lv2plug.in/ns/lv2core#LowpassPlugin) -> EQs, Filters & Dynamics -> Filter
 - [lv2:ModulatorPlugin](http://lv2plug.in/ns/lv2core#ModulatorPlugin) -> Modulation & Pitch -> Modulation
 - [lv2:ChorusPlugin](http://lv2plug.in/ns/lv2core#ChorusPlugin) -> Modulation & Pitch -> Modulation
 - [lv2:FlangerPlugin](http://lv2plug.in/ns/lv2core#FlangerPlugin) -> Modulation & Pitch -> Modulation
 - [lv2:PhaserPlugin](http://lv2plug.in/ns/lv2core#PhaserPlugin) -> Modulation & Pitch & Synth -> Modulation
 - [lv2:SpectralPlugin](http://lv2plug.in/ns/lv2core#SpectralPlugin) -> Modulation & Pitch -> Modulation
 - [lv2:PitchPlugin](http://lv2plug.in/ns/lv2core#PitchPlugin) -> Modulation & Pitch -> Pitch
 - [lv2:DelayPlugin](http://lv2plug.in/ns/lv2core#DelayPlugin) -> Delays & Reverbs -> Delay
 - [lv2:ReverbPlugin](http://lv2plug.in/ns/lv2core#ReverbPlugin) -> Delays & Reverbs -> Reverb
 - [lv2:InstrumentPlugin](http://lv2plug.in/ns/lv2core#InstrumentPlugin) -> Modulation & Pitch & Synth -> Synth
 - [lv2:UtilityPlugin](http://lv2plug.in/ns/lv2core#UtilityPlugin) ->  Utilities -> Utility

Everything else will appear under Utilities.
