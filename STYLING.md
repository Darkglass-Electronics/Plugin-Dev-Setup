# Audio Plugin Development Documentation for Darkglass Anagram

This repository contains documentation and examples related to developing audio plugins for [Darkglass Anagram](https://www.darkglass.com/products/anagram/) as a platform.

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

NOTE: None of the information in document is present on a public release as the system implementation is still on-going.

## Styling

It is possible to stylize the block and block settings of a plugin through the use of a Darkglass LV2 extension.  
This extension defines properties related to alignment, fonts, images and positioning.

For a high-level overview of all the possible properties and their relationship see [custom-styling.hpp](https://github.com/Darkglass-Electronics/mod-connector/blob/custom-styling/src/custom-styling.hpp) which is a C++ representation used by our LV2 host.

Because many plugin frameworks automatically generate LV2 ttl, we recommend to store all the styling related information in a separate file and only reference it through "seeAlso" on the `manifest.ttl` entry point.  
For example:

```ttl
@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .
@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .

<https://mycompany.com/products/MyPlugin>
    a lv2:Plugin ;
    lv2:binary <MyPlugin.so> ;
    rdfs:seeAlso <MyPlugin.ttl> , <Styling.ttl> .
```

In the example above we manually add `<Styling.ttl>` to the `rdfs:seeAlso` property, so if the plugin framework (or any other generator) replaces the main plugin ttl file we won't lose our changes.

If your ttl files are hand-crafted you might prefer to store everything together. Both choices are valid.

### Reference

All properties exist under the LV2 extension prefix `http://www.darkglass.com/lv2/ns/lv2ext/custom-styling#`.  
In this document we assume this prefix has been aliased to `dgcs` using syntax like so:

```ttl
@prefix dgcs: <http://www.darkglass.com/lv2/ns/lv2ext/custom-styling#> .
```

NOTE: Work in progress! for now here is a list of all properties:

```ttl
###############################################################################
# enums

# possible alignment types
dgcs:alignBottomLeft
dgcs:alignBottomMid
dgcs:alignBottomRight
dgcs:alignCenter
dgcs:alignLeftMid
dgcs:alignRightMid
dgcs:alignTopLeft
dgcs:alignTopMid
dgcs:alignTopRight

###############################################################################
# single-value properties

# part of an image; sets the alignment type
dgcs:alignment

# part of a font, image or overlay; sets a bundle-related path
dgcs:path

# part of a font; sets the size
dgcs:size

# part of a block parameter; sets the image position
# NOTE only applies to dgcs:block parameters, not dgcs:blockSettings
dgcs:x
dgcs:y

###############################################################################
# combined properties

# possible overlays
dgcs:blocked
dgcs:inUse
dgcs:inactive
dgcs:unavailable

# possible block settings top-bar buttons
dgcs:back
dgcs:close
dgcs:more
dgcs:remove
dgcs:swap

# refers to an image, used in multiple properties
dgcs:background
dgcs:backgroundScenes
dgcs:control

# refers to a font
dgcs:font

# refers to the bypass widget
dgcs:bypass

# refers to custom parameters (specified by control port symbol)
dgcs:parameters

###############################################################################
# properties specific to the block settings

# the block name in the block setttings
dgcs:blockName

# the block settings pagination dots
dgcs:paginationDots

# padding of empty slots in the block settings until parameters begin
dgcs:parameterStartPadding

# the block settings top bar buttons
dgcs:topBarButtons

# possible default parameter widgets
dgcs:knob
dgcs:list
dgcs:meter
dgcs:toggle

###############################################################################
# high-level properties

# the styling for a block
dgcs:block

# the styling for a block's settings
dgcs:blockSettings
```

### Block styling

Not implemented yet.

### Block settings styling

Latest internal builds support styling of a few details in the block settings.

For brievety here is an example file for all properties supported so far:

```ttl
@prefix dgcs: <http://www.darkglass.com/lv2/ns/lv2ext/custom-styling#> .
@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .

<https://mycompany.com/products/MyPlugin>
    # styling the block settings
    dgcs:blockSettings [

        # custom background, can specify path and alignment type
        dgcs:background [
            dgcs:alignment dgcs:alignBottomRight ;
            dgcs:path <background.png> ;
        ] ;

        # default knob widget, only "control" is required but we can provide extra background
        dgcs:knob [
            dgcs:background [
                dgcs:alignment dgcs:alignCenter ;
                dgcs:path <knob-background.png> ;
            ] ;
            dgcs:backgroundScenes [
                dgcs:alignment dgcs:alignCenter ;
                dgcs:path <knob-background-scenes.png> ;
            ] ;
            dgcs:control [
                dgcs:alignment dgcs:alignCenter ;
                dgcs:path <knob.png> ;
            ] ;
        ] ;

        # default toggle widget
        dgcs:toggle [
            dgcs:control [
                dgcs:alignment dgcs:alignCenter ;
                dgcs:path <switch.png> ;
            ] ;
        ] ;

        # arbitrary padding for branding is possible
        dgcs:parameterStartPadding 4;

        # custom parameter widgets that override the defaults above
        dgcs:parameters [
            lv2:symbol "gain" ;
            dgcs:control [
                dgcs:alignment dgcs:alignCenter ;
                dgcs:path <knob.png> ;
            ] ;
        ] , [
            lv2:symbol "inv_polarity" ;
            dgcs:control [
                dgcs:alignment dgcs:alignCenter ;
                dgcs:path <knob.png> ;
            ] ;
        ] ;
    ] .
```
