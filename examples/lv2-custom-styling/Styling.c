/*
 * Example plugin for Anagram
 * Copyright (C) 2026 Filipe Coelho <falktx@falktx.com>
 * SPDX-License-Identifier: ISC
 */

#if defined(__has_include) && __has_include("lv2/core/lv2.h")
#include "lv2/core/lv2.h"
#else
#include "lv2.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// use libnickel licensing mechanism when building for Anagram (codenamed pablito)
#if _DARKGLASS_DEVICE_PABLITO
#include <libnickel.h>
#endif

#define EXAMPLE_PLUGIN_URI "https://github.com/Darkglass-Electronics/Plugin-Dev-Setup#example-styling"

// all ports of this plugin, matching the ttl side
typedef enum {
    kPortInput,
    kPortOutput,
    kPortEnabled,
    kPortReset,
    kPortList,
    kPortToggle,
    kPortInteger,
    kPortFloat1,
    kPortFloat2,
    kPortFloat3,
    kPortMeter,
    kPortValue,
    kPortCount
} Ports;

typedef struct {
    const float* in;
    float* out;
    const float* reset;
    float* meter;
    float* value;
    float meterValue;
    uint32_t samplesPerStep;
    uint32_t sampleCounter;
   #if _DARKGLASS_DEVICE_PABLITO
    // counter for the licensing mechanism
    uint32_t runFrameCount;
   #endif
} ExamplePlugin;

static LV2_Handle lv2_instantiate(const struct LV2_Descriptor* descriptor,
                                  double sampleRate,
                                  const char* bundlepath,
                                  const LV2_Feature* const* features)
{
   #if _DARKGLASS_DEVICE_PABLITO
    // check for the license of this plugin
    if (! nickel_init(sampleRate, features, EXAMPLE_PLUGIN_URI))
    {
        // if no license (or unsupported host), try the Anagram (codenamed pablito) device key too
        // since this is a free plugin, so we want to accept the device key
        // NOTE for commercial plugins make sure to remove the next line!
        nickel_init(sampleRate, features, "urn:darkglass:pablito");
    }
   #endif
    ExamplePlugin* plugin = calloc(1, sizeof(ExamplePlugin));
    if (plugin != NULL)
        plugin->samplesPerStep = (uint32_t)sampleRate;
    return plugin;
}

static void lv2_connect_port(LV2_Handle handle, uint32_t port, void* data)
{
    ExamplePlugin* plugin = handle;

    switch (port) {
    case kPortInput:
        plugin->in = data;
        break;
    case kPortOutput:
        plugin->out = data;
        break;
    case kPortReset:
        plugin->reset = data;
        break;
    case kPortMeter:
        plugin->meter = data;
        break;
    case kPortValue:
        plugin->value = data;
        break;
    }
}

static void lv2_run(LV2_Handle handle, uint32_t numSamples)
{
    ExamplePlugin* plugin = handle;

   #if _DARKGLASS_DEVICE_PABLITO
    if (*plugin->reset > 0.f)
        plugin->runFrameCount = 0;

    plugin->runFrameCount = nickel_license_run_begin(plugin->runFrameCount, numSamples);
   #endif

    if (plugin->out != plugin->in)
        memcpy(plugin->out, plugin->in, sizeof(float) * numSamples);

    // increase once per second, to show each strip frame
    plugin->sampleCounter += numSamples;

    while (plugin->sampleCounter >= plugin->samplesPerStep) {
        plugin->sampleCounter -= plugin->samplesPerStep;
        plugin->meterValue += 1.f;

        if (plugin->meterValue > 299.f)
            plugin->meterValue = 0.f;
    }

    *plugin->meter = plugin->meterValue;
    *plugin->value = plugin->meterValue;

   #if _DARKGLASS_DEVICE_PABLITO
    nickel_license_run_silence(plugin->runFrameCount, plugin->out, numSamples, 0);
   #endif
}

static void lv2_cleanup(LV2_Handle handle)
{
    free(handle);
}

static const void* lv2_extension_data(const char* uri)
{
   #if _DARKGLASS_DEVICE_PABLITO
    return nickel_license_interface(uri);
   #else
    return NULL;
   #endif
}

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    static const LV2_Descriptor descriptor = {
        .URI = EXAMPLE_PLUGIN_URI,
        .instantiate = lv2_instantiate,
        .connect_port = lv2_connect_port,
        .run = lv2_run,
        .cleanup = lv2_cleanup,
        .extension_data = lv2_extension_data,
    };
    return index == 0 ? &descriptor : NULL;
}
