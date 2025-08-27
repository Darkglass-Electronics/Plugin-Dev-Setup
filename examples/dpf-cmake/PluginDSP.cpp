/*
 * Example plugin for Anagram using DPF
 * Copyright (C) 2021 Jean Pierre Cimalando <jp-dev@inbox.ru>
 * Copyright (C) 2021-2025 Filipe Coelho <falktx@falktx.com>
 * SPDX-License-Identifier: ISC
 */

#include "DistrhoPlugin.hpp"
#include "extra/ValueSmoother.hpp"

START_NAMESPACE_DISTRHO

// --------------------------------------------------------------------------------------------------------------------

static constexpr const float db2coef(float g)
{
    return g > -90.f ? std::pow(10.f, g * 0.05f) : 0.f;
}

// --------------------------------------------------------------------------------------------------------------------

class ExamplePlugin : public Plugin
{
    enum Parameters {
        kParamGain = 0,
        kParamCount
    };

    float fGainDB = 0.f;
    ExponentialValueSmoother fSmoothGain;

public:
   /**
      Plugin class constructor.@n
      You must set all parameter values to their defaults, matching ParameterRanges::def.
    */
    ExamplePlugin()
        : Plugin(kParamCount, 0, 0) // parameters, programs, states
    {
        fSmoothGain.setSampleRate(getSampleRate());
        fSmoothGain.setTargetValue(db2coef(0.f));
        fSmoothGain.setTimeConstant(0.02f); // 20ms
    }

protected:
    // ----------------------------------------------------------------------------------------------------------------
    // Information

   /**
      Get the plugin label.@n
      This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
    */
    const char* getLabel() const noexcept override
    {
        return "DarkglassExampleDPF";
    }

   /**
      Get an extensive comment/description about the plugin.@n
      Optional, returns nothing by default.
    */
    const char* getDescription() const override
    {
        return "Example plugin for Anagram using DPF";
    }

   /**
      Get the plugin author/maker.
    */
    const char* getMaker() const noexcept override
    {
        return "falkTX, Jean Pierre Cimalando";
    }

   /**
      Get the plugin license (a single line of text or a URL).@n
      For commercial plugins this should return some short copyright information.
    */
    const char* getLicense() const noexcept override
    {
        return "ISC";
    }

   /**
      Get the plugin version, in hexadecimal.
      @see d_version()
    */
    uint32_t getVersion() const noexcept override
    {
        return d_version(1, 0, 0);
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Init

   /**
      Initialize the parameter @a index.@n
      This function will be called once, shortly after the plugin is created.
    */
    void initParameter(uint32_t index, Parameter& parameter) override
    {
        DISTRHO_SAFE_ASSERT_RETURN(index == 0,);

        parameter.ranges.min = -90.f;
        parameter.ranges.max = 30.f;
        parameter.ranges.def = 0.f;
        parameter.hints = kParameterIsAutomatable;
        parameter.name = "Gain";
        parameter.shortName = "Gain";
        parameter.symbol = "gain";
        parameter.unit = "dB";
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Internal data

   /**
      Get the current value of a parameter.@n
      The host may call this function from any context, including realtime processing.
    */
    float getParameterValue(uint32_t index) const override
    {
        DISTRHO_SAFE_ASSERT_RETURN(index == 0, 0.f);

        return fGainDB;
    }

   /**
      Change a parameter value.@n
      The host may call this function from any context, including realtime processing.@n
      When a parameter is marked as automatable, you must ensure no non-realtime operations are performed.
      @note This function will only be called for parameter inputs.
    */
    void setParameterValue(uint32_t index, float value) override
    {
        DISTRHO_SAFE_ASSERT_RETURN(index == 0,);

        fGainDB = value;
        fSmoothGain.setTargetValue(db2coef(std::clamp(value, -90.f, 30.f)));
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Audio/MIDI Processing

   /**
      Activate this plugin.
    */
    void activate() override
    {
        fSmoothGain.clearToTargetValue();
    }

   /**
      Run/process function for plugins without MIDI input.
      @note Some parameters might be null if there are no audio inputs or outputs.
    */
    void run(const float** inputs, float** outputs, uint32_t frames) override
    {
        // get the left and right audio inputs
        const float* const inpL = inputs[0];
        const float* const inpR = inputs[1];

        // get the left and right audio outputs
        float* const outL = outputs[0];
        float* const outR = outputs[1];

        // apply gain against all samples
        for (uint32_t i = 0; i < frames; ++i)
        {
            const float gain = fSmoothGain.next();
            outL[i] = inpL[i] * gain;
            outR[i] = inpR[i] * gain;
        }
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Callbacks (optional)

   /**
      Optional callback to inform the plugin about a sample rate change.@n
      This function will only be called when the plugin is deactivated.
      @see getSampleRate()
    */
    void sampleRateChanged(double newSampleRate) override
    {
        fSmoothGain.setSampleRate(newSampleRate);
    }

    // ----------------------------------------------------------------------------------------------------------------

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExamplePlugin)
};

// --------------------------------------------------------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ExamplePlugin();
}

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
