/*
 * Example plugin for Anagram using raw LV2
 * Copyright (C) 2025-2026 Filipe Coelho <falktx@falktx.com>
 * SPDX-License-Identifier: ISC
 */

#include "Korg35Filters.hpp"
#include "ValueSmoother.hpp"

#if defined(__has_include) && __has_include("lv2/core/lv2.h")
#include "lv2/core/lv2_util.h"
#include "lv2/options/options.h"
#else
#include "lv2/lv2plug.in/ns/lv2core/lv2_util.h"
#include "lv2/lv2plug.in/ns/extensions/options/options.h"
#endif

#include "dg-control-port-state-update.lv2/control-port-state-update.h"

// use libnickel licensing mechanism when building for Anagram (codenamed pablito)
#if _DARKGLASS_DEVICE_PABLITO
#include <libnickel.h>
#endif

#include <array>

#define EXAMPLE_PLUGIN_URI "https://github.com/Darkglass-Electronics/Plugin-Dev-Setup#example-filter-lv2"

class ExampleFilter
{
    // types of filter exposed in this plugin, matching the ttl side
    enum FreqType {
        kFreqNone,
        kFreqLP,
        kFreqHP,
        kFreqCount
    };

    // all ports of this plugin, matching the ttl side
    enum Ports {
        kPortInput,
        kPortOutput,
        kPortEnabled,
        kPortReset,
        kPortType,
        kPortFreq,
        kPortCount
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
        void* ports[kPortCount];
    };

    // filter objects
    Korg35Filter lp, hp;

    // report state updates to host as needed
    FreqType oldType = kFreqLP;
    LV2_Control_Port_State_Update* portStateUpdate = nullptr;

    // smoothing values
    LinearValueSmoother smoothEnabled;
    LinearValueSmoother smoothFilters[kFreqCount];

    // process audio in chunks of 16 frames
    static constexpr uint32_t bufferSize = 16;
    std::array<float, bufferSize> bufDry;
    std::array<float, bufferSize> bufLP;
    std::array<float, bufferSize> bufHP;

   #if _DARKGLASS_DEVICE_PABLITO
    // counter for the licensing mechanism
    uint32_t runFrameCount = 0;
   #endif

    // reset filters and flush smooth values
    void reset()
    {
        lp.reset();
        hp.reset();

        smoothEnabled.clearToTargetValue();
        for (uint8_t i = 0; i < kFreqCount; ++i)
            smoothFilters[i].clearToTargetValue();

       #if _DARKGLASS_DEVICE_PABLITO
        // also reset the licensing run counter
        runFrameCount = 0;
       #endif
    }

public:
    ExampleFilter(double sampleRate, const LV2_Feature* const* features)
        : lp(sampleRate),
          hp(sampleRate)
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

        portStateUpdate = static_cast<LV2_Control_Port_State_Update*>(
            lv2_features_data(features, LV2_CONTROL_PORT_STATE_UPDATE_URI));

        static constexpr const float t60 = 0.05f;
        smoothEnabled.setSampleRate(sampleRate);
        smoothEnabled.setTimeConstant(t60);
        smoothEnabled.setTargetValue(1.f);

        for (uint8_t i = 0; i < kFreqCount; ++i)
        {
            smoothFilters[i].setSampleRate(sampleRate);
            smoothFilters[i].setTimeConstant(t60);
            smoothFilters[i].setTargetValue(0.f);
        }

        // Low Pass is default
        smoothFilters[kFreqLP].setTargetValue(1.f);
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
        // handle parameters first
        smoothEnabled.setTargetValue(d_isNotZero(*paramEnabled) ? 1.f : 0.f);

        // check if filter type changed, report to host as needed
        const FreqType type = static_cast<FreqType>(std::clamp<int>(*paramType + 0.5f, kFreqNone, kFreqHP));
        if (oldType != type)
        {
            oldType = type;

            if (portStateUpdate != nullptr)
                portStateUpdate->update_state(portStateUpdate->handle,
                                              kPortFreq,
                                              type == kFreqNone ? LV2_CONTROL_PORT_STATE_INACTIVE
                                                                : LV2_CONTROL_PORT_STATE_NONE);

            switch (type)
            {
            case kFreqNone:
                smoothFilters[kFreqNone].setTargetValue(1.f);
                smoothFilters[kFreqLP].setTargetValue(0.f);
                smoothFilters[kFreqHP].setTargetValue(0.f);
                break;
            case kFreqLP:
                smoothFilters[kFreqNone].setTargetValue(0.f);
                smoothFilters[kFreqLP].setTargetValue(1.f);
                smoothFilters[kFreqHP].setTargetValue(0.f);
                break;
            case kFreqHP:
                smoothFilters[kFreqNone].setTargetValue(0.f);
                smoothFilters[kFreqLP].setTargetValue(0.f);
                smoothFilters[kFreqHP].setTargetValue(1.f);
                break;
            case kFreqCount:
                break;
            }
        }

        const float freq = *paramFreq;
        lp.setFrequency(freq);
        hp.setFrequency(freq);

        if (d_isNotZero(*paramReset))
            reset();

        #if _DARKGLASS_DEVICE_PABLITO
         runFrameCount = nickel_license_run_begin(runFrameCount, numSamples);
        #endif

        // process audio in chunks
        for (uint32_t offset = 0; offset < numSamples; offset += bufferSize)
        {
            const uint32_t numSamplesInChunk = std::min(numSamples - offset, bufferSize);

            lp.processLowPass(in + offset, bufLP.data(), numSamplesInChunk);
            hp.processHighPass(in + offset, bufHP.data(), numSamplesInChunk);

            float mix, proc;
            for (uint32_t i = 0; i < numSamplesInChunk; ++i)
            {
                // tell the compiler we only deal with finite numbers
                if (!std::isfinite(in[offset + i]))
                    __builtin_unreachable();
                if (!std::isfinite(out[offset + i]))
                    __builtin_unreachable();
                if (!std::isfinite(bufLP[i]))
                    __builtin_unreachable();
                if (!std::isfinite(bufHP[i]))
                    __builtin_unreachable();

                // processed value with smoothing between filter types
                proc = in[offset + i] * smoothFilters[kFreqNone].next()
                     + bufLP[i] * smoothFilters[kFreqLP].next()
                     + bufHP[i] * smoothFilters[kFreqHP].next();

                // enabled/bypass handling
                mix = smoothEnabled.next();
                out[offset + i] = proc * mix + in[offset + i] * (1.f - mix);
            }
        }

        #if _DARKGLASS_DEVICE_PABLITO
         nickel_license_run_silence(runFrameCount, out, numSamples, 0);
        #endif
    }
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    static const LV2_Descriptor descriptor {
        .URI = EXAMPLE_PLUGIN_URI,
        .instantiate = [](const struct LV2_Descriptor*,
                          double sampleRate,
                          const char*,
                          const LV2_Feature* const* features) -> LV2_Handle
        {
            return new ExampleFilter(sampleRate, features);
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
        .cleanup = [](LV2_Handle instance)
        {
            delete static_cast<ExampleFilter*>(instance);
        },
        .extension_data = [](const char* uri) -> const void*
        {
           #if _DARKGLASS_DEVICE_PABLITO
            return nickel_license_interface(uri);
           #else
            return nullptr;
           #endif
        }
    };
    return index == 0 ? &descriptor : nullptr;
}
