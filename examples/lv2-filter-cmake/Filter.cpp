/*
 * Example plugin for Anagram using raw LV2
 * Copyright (C) 2025 Filipe Coelho <falktx@falktx.com>
 * SPDX-License-Identifier: ISC
 */

#include "Korg35Filters.hpp"

#if defined(__has_include) && __has_include("lv2/core/lv2.h")
#include "lv2/core/lv2.h"
#else
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#endif

#include "dg-control-port-state-update.lv2/control-port-state-update.h"

class ExampleFilter
{
    Korg35Filter lp, hp;

    enum FreqType {
        kFreqNone,
        kFreqLP,
        kFreqHP,
    };

    union {
        struct {
            const float* in;
            float* out;
            const float* paramEnabled;
            const float* paramReset;
            const float* paramType;
            const float* paramFreq;
        };
        void** ports;
    };

    struct

    void reset()
    {
        lp.reset();
        hp.reset();
    }

public:
    ExampleFilter(double sampleRate)
        : lp(sampleRate),
          hp(sampleRate)
    {
    }

    void connectPort(uint32_t port, void* data)
    {
        ports[port] = data;
    }

    void activate()
    {
        reset();
    }

    void run(uint32_t numSamples)
    {
        const float freq = *paramFreq;
        lp.setFrequency(freq);
        hp.setFrequency(freq);

        lp.processLowPass(in, out, numSamples);
    }
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    static const LV2_Descriptor descriptor {
        .URI = "https://github.com/Darkglass-Electronics/Plugin-Dev-Setup#example-filter-lv2",
        .instantiate = [](const struct LV2_Descriptor*,
                          double sampleRate,
                          const char*,
                          const LV2_Feature* const* features) -> LV2_Handle
        {
            return new ExampleFilter(sampleRate);
        },
        .connect_port = [](LV2_Handle instance, uint32_t port, void* data_location)
        {
            static_cast<ExampleFilter*>(instance)->connectPort(port, data_location);
        },
        .activate = [](LV2_Handle instance)
        {
            static_cast<ExampleFilter*>(instance)->activate();
        },
        .run = [](LV2_Handle instance, uint32_t sample_count)
        {
            static_cast<ExampleFilter*>(instance)->run(sample_count);
        },
        .deactivate = nullptr,
        .extension_data = [](const char* uri) -> const void*
        {
            return nullptr;
        }
    };
    return index == 0 ? &descriptor : nullptr;
}
