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
    float* meter;
    float* value;
    float meterValue;
    uint32_t samplesPerStep;
    uint32_t sampleCounter;
} ExamplePlugin;

static LV2_Handle lv2_instantiate(const struct LV2_Descriptor* descriptor,
                                  double sampleRate,
                                  const char* bundlepath,
                                  const LV2_Feature* const* features)
{
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
}

static void lv2_cleanup(LV2_Handle handle)
{
    free(handle);
}

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    static const LV2_Descriptor descriptor = {
        .URI = "https://github.com/Darkglass-Electronics/Plugin-Dev-Setup#example-styling",
        .instantiate = lv2_instantiate,
        .connect_port = lv2_connect_port,
        .run = lv2_run,
        .cleanup = lv2_cleanup,
    };
    return index == 0 ? &descriptor : NULL;
}
