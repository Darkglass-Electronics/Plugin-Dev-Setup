/*
 * Example plugin for Anagram using DPF
 * Copyright (C) 2025 Filipe Coelho <falktx@falktx.com>
 * SPDX-License-Identifier: ISC
 */

#include "DistrhoPlugin.hpp"
#include "extra/ValueSmoother.hpp"

START_NAMESPACE_DISTRHO

// --------------------------------------------------------------------------------------------------------------------
// definitions for this example plugin, matching the Darkglass Gain plugin

static constexpr float minGainDB = -40.f;
static constexpr float maxGainDB = 20.f;

// --------------------------------------------------------------------------------------------------------------------
// dB to coefficient

static constexpr const float db2coef(float g)
{
    return g > minGainDB ? std::pow(10.f, g * 0.05f) : 0.f;
}

// --------------------------------------------------------------------------------------------------------------------

class ExamplePlugin : public Plugin
{
    // list of parameters
    enum Parameters {
        kParamBypass,
        kParamGainDB,
        kParamInvertPolarity,
        kParamCount
    };

    // list of programs
    enum Programs {
        kProgramAtZero,
        kProgramMinus8,
        kProgramMinus12,
        kProgramMinus16,
        kProgramCount
    };

    // store raw parameter values, so we can return them during getParameterValue()
    float fParameters[kParamCount] = {};

    // gain coefficient with smoothing
    ExponentialValueSmoother fSmoothGain;

    // reuse bypass and gain parameter handling for this simple example
    // if bypassed, set smooth gain to 0dB
    // if enabled, set smooth gain as the coefficient from dB
    void updateSmoothGain()
    {
        fSmoothGain.setTargetValue(fParameters[kParamBypass] > 0.5f
            ? db2coef(0.f)
            : db2coef(std::clamp(fParameters[kParamGainDB], minGainDB, maxGainDB)));
    }

public:
   /**
      Plugin class constructor.
      You must set all parameter values to their defaults, matching ParameterRanges::def.
    */
    ExamplePlugin()
        : Plugin(kParamCount, kProgramCount, 0) // parameters, programs, states
    {
        constexpr float smoothTimeMs = 0.02f;
        fSmoothGain.setSampleRate(getSampleRate());
        fSmoothGain.setTargetValue(db2coef(0.f));
        fSmoothGain.setTimeConstant(smoothTimeMs);
    }

protected:
    // ----------------------------------------------------------------------------------------------------------------
    // Information

   /**
      Get the plugin label.
      This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
    */
    const char* getLabel() const noexcept override
    {
        return "DarkglassExampleGainDPF";
    }

   /**
      Get an extensive comment/description about the plugin.
      Optional, returns nothing by default.
    */
    const char* getDescription() const override
    {
        return "Example gain plugin for Anagram using DPF";
    }

   /**
      Get the plugin author/maker.
    */
    const char* getMaker() const noexcept override
    {
        return "falkTX";
    }

   /**
      Get the plugin license (a single line of text or a URL).
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
      Initialize the parameter @a index.
      This function will be called once, shortly after the plugin is created.
    */
    void initParameter(uint32_t index, Parameter& parameter) override
    {
        switch (static_cast<Parameters>(index))
        {
        case kParamBypass:
            parameter.initDesignation(kParameterDesignationBypass);
            break;
        case kParamGainDB:
            parameter.ranges.min = minGainDB;
            parameter.ranges.max = maxGainDB;
            parameter.ranges.def = 0.f;
            parameter.hints = kParameterIsAutomatable;
            parameter.name = "Gain";
            parameter.symbol = "gain";
            parameter.unit = "dB";
            {
                parameter.enumValues.count = 1;
                parameter.enumValues.values = new ParameterEnumerationValue[1];
                parameter.enumValues.values[0].label = "-Inf";
                parameter.enumValues.values[0].value = minGainDB;
            }
            break;
        case kParamInvertPolarity:
            parameter.ranges.min = 0.f;
            parameter.ranges.max = 1.f;
            parameter.ranges.def = 0.f;
            parameter.hints = kParameterIsAutomatable | kParameterIsInteger | kParameterIsBoolean;
            parameter.name = "Invert Polarity";
            parameter.symbol = "inv_polarity";
            break;
        case kParamCount:
            break;
        }
    }

   /**
      Set the name of the program @a index.
      This function will be called once, shortly after the plugin is created.
      Must be implemented by your plugin class only if DISTRHO_PLUGIN_WANT_PROGRAMS is enabled.
    */
    void initProgramName(uint32_t index, String& programName) override
    {
        switch (static_cast<Programs>(index))
        {
        case kProgramAtZero:
            programName = "At Zero";
            break;
        case kProgramMinus8:
            programName = "Minus 8";
            break;
        case kProgramMinus12:
            programName = "Minus 12";
            break;
        case kProgramMinus16:
            programName = "Minus 16";
            break;
        case kProgramCount:
            break;
        }
    };

    // ----------------------------------------------------------------------------------------------------------------
    // Internal data

   /**
      Get the current value of a parameter.
      The host may call this function from any context, including realtime processing.
    */
    float getParameterValue(uint32_t index) const override
    {
        return fParameters[index];
    }

   /**
      Change a parameter value.
      The host may call this function from any context, including realtime processing.
      When a parameter is marked as automatable, you must ensure no non-realtime operations are performed.
      @note This function will only be called for parameter inputs.
    */
    void setParameterValue(uint32_t index, float value) override
    {
        fParameters[index] = value;

        switch (index)
        {
        case kParamBypass:
        case kParamGainDB:
            updateSmoothGain();
            break;
        }
    }

   /**
      Load a program.
      The host may call this function from any context, including realtime processing.
      Must be implemented by your plugin class only if DISTRHO_PLUGIN_WANT_PROGRAMS is enabled.
    */
    void loadProgram(uint32_t index) override
    {
        fParameters[kParamInvertPolarity] = 0.f;

        switch (static_cast<Programs>(index))
        {
        case kProgramAtZero:
            fParameters[kParamGainDB] = 0.f;
            break;
        case kProgramMinus8:
            fParameters[kParamGainDB] = -8.f;
            break;
        case kProgramMinus12:
            fParameters[kParamGainDB] = -12.f;
            break;
        case kProgramMinus16:
            fParameters[kParamGainDB] = -16.f;
            break;
        case kProgramCount:
            return;
        }

        updateSmoothGain();
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
        // get the mono audio input
        const float* const in = inputs[0];

        // get the mono audio output
        float* const out = outputs[0];

        // polarity
        const float polarity = fParameters[kParamInvertPolarity] > 0.5f ? -1.f : 1.f;

        // apply gain and polarity
        for (uint32_t i = 0; i < frames; ++i)
            out[i] = in[i] * fSmoothGain.next() * polarity;
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Callbacks (optional)

   /**
      Optional callback to inform the plugin about a sample rate change.
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
