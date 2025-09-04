// Example plugin for Anagram using JUCE
// Copyright (C) 2025 Filipe Coelho <falktx@darkglass.com>
// SPDX-License-Identifier: ISC

#include <juce_audio_processors/juce_audio_processors.h>

#include "juce_anagram.h"

//=====================================================================================================================
// definitions for this example plugin, matching the Darkglass Gain plugin

static constexpr float minGainDB = -40.f;
static constexpr float maxGainDB = 20.f;

//=====================================================================================================================
// reuse bypass and gain parameter handling logic for this simple example
// if bypassed, set smooth gain to 0dB
// if enabled, set smooth gain as the coefficient from dB

static float gainCoef(bool bypass, float gainDB)
{
    return bypass ? 1.f : juce::Decibels::decibelsToGain(std::clamp(gainDB, minGainDB, maxGainDB), minGainDB);
}

class ExampleBypassParameter : public juce::AudioParameterBool
{
public:
    ExampleBypassParameter(juce::AudioParameterFloat*& gainRef, juce::SmoothedValue<float>& gainValueRef)
        : juce::AudioParameterBool ({ "bypass", 1 }, "Bypass", false),
          gain(gainRef),
          gainValue(gainValueRef)
    {}

protected:
    void valueChanged (bool newValue) final
    {
        gainValue.setTargetValue(gainCoef(newValue, gain->get()));
    }

private:
    juce::AudioParameterFloat*& gain;
    juce::SmoothedValue<float>& gainValue;
};

// NOTE we subclass anagram::AudioParameterWithScalePoints for being able to give "-40" a dedicated string
class ExampleGainParameter : public juce::AudioParameterFloat,
                             public anagram::AudioParameterWithScalePoints
{
public:
    ExampleGainParameter(juce::AudioParameterBool*& bypassRef, juce::SmoothedValue<float>& gainValueRef)
        : juce::AudioParameterFloat ({ "gain", 2 },
                                     "Gain",
                                     { minGainDB, maxGainDB },
                                     0.f,
                                     juce::AudioParameterFloatAttributes().withLabel ("dB")),
          bypass(bypassRef),
          gainValue(gainValueRef)
    {}

protected:
    void valueChanged (float newValue) final
    {
        gainValue.setTargetValue(gainCoef(bypass->get(), newValue));
    }

    // tell the host to show "-Inf" for -40.f (minGainDB)
    juce::Array<anagram::AudioParameterScalePoint> getAllScalePoints() const final
    {
        return { { "-Inf", minGainDB } };
    }

private:
    juce::AudioParameterBool*& bypass;
    juce::SmoothedValue<float>& gainValue;
};

//=====================================================================================================================

class ExampleAudioProcessor : public juce::AudioProcessor
{
    // JUCE parameters
    juce::AudioParameterBool* bypass = new ExampleBypassParameter (gain, gainValue);
    juce::AudioParameterFloat* gain = new ExampleGainParameter (bypass, gainValue);
    juce::AudioParameterBool* invertPolarity = new juce::AudioParameterBool({ "inv-polarity", 3 },
                                                                            "Invert Polarity",
                                                                            false);

    // Gain coefficient as a smooth value, directly referenced by both bypass and gain parameters
    juce::SmoothedValue<float> gainValue;

public:
    // Constructor with mono IO
    ExampleAudioProcessor()
        : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::mono())
                                           .withOutput ("Output", juce::AudioChannelSet::mono()))
    {
        addParameter (bypass);
        addParameter (gain);
        addParameter (invertPolarity);

        gainValue.setCurrentAndTargetValue (1.f);
    }

    // Destructor
    ~ExampleAudioProcessor() override {}

    //=================================================================================================================

    // Returns the name of this processor
    const juce::String getName() const override
    {
        return JucePlugin_Name;
    }

    // Returns a list of alternative names to use for this processor
    // On Anagram we use this method to fetch the "dg:abbreviation" property
    // See https://github.com/Darkglass-Electronics/Plugin-Dev-Setup/blob/main/REQUIREMENTS.md
    juce::StringArray getAlternateDisplayNames() const override
    {
        return { "GAI" };
    }

    //=================================================================================================================

    // Called before playback starts, to let the processor prepare itself
    void prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock) override
    {
        constexpr float smoothTimeMs = 0.02f;
        gainValue.reset (sampleRate, smoothTimeMs);

        juce::ignoreUnused (maximumExpectedSamplesPerBlock);
    }

    // Called after playback has stopped, to let the object free up any resources it no longer needs
    void releaseResources() override
    {
    }

    // Renders the next block
    // Anagram will always use 32-bit float format
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        gainValue.applyGain(buffer.getWritePointer(0), buffer.getNumSamples());

        if (invertPolarity->get())
            buffer.applyGain(-1.f);

        juce::ignoreUnused (midiMessages);
    }

    // indicate to the compiler we only use the float variant of processBlock, which silences a warning
    using AudioProcessor::processBlock;

    //=================================================================================================================

    // Returns the length of the processor's tail, in seconds
    // Unused in Anagram but required by JUCE, just return 0
    double getTailLengthSeconds() const override
    {
        return 0.0;
    }

    // Returns true if the processor wants MIDI messages
    // Unused in Anagram for now
    bool acceptsMidi() const override
    {
       #if JucePlugin_WantsMidiInput
        return true;
       #else
        return false;
       #endif
    }

    // Returns true if the processor produces MIDI messages
    // Unused in Anagram for now
    bool producesMidi() const override
    {
       #if JucePlugin_ProducesMidiOutput
        return true;
       #else
        return false;
       #endif
    }

    //=================================================================================================================

    // Returns the parameter that controls the AudioProcessor's bypass state
    // Required in Anagram, must be implemented and not return null
    juce::AudioProcessorParameter* getBypassParameter() const override
    {
        return bypass;
    }

    //=================================================================================================================

    // Creates the processor's GUI
    // Unused in Anagram but required by JUCE, just return null
    juce::AudioProcessorEditor* createEditor() override
    {
        return nullptr;
    }

    // Your processor subclass must override this and return true if it can create an editor component
    // Unused in Anagram but required by JUCE, just return false
    bool hasEditor() const override
    {
        return false;
    }

    //=================================================================================================================

    // Returns the number of preset programs the processor supports
    // Unused in Anagram for now
    int getNumPrograms() override
    {
        return 0;
    }

    // Returns the number of the currently active program
    // Unused in Anagram but required by JUCE, just return 0
    int getCurrentProgram() override
    {
        return 0;
    }

    // Called by the host to change the current program
    // Unused in Anagram but required by JUCE
    void setCurrentProgram (int index) override
    {
        juce::ignoreUnused (index);
    }

    // Must return the name of a given program
    // Unused in Anagram for now
    const juce::String getProgramName (int index) override
    {
        juce::ignoreUnused (index);
        return {};
    }

    // Called by the host to rename a program
    // Unused in Anagram but required by JUCE
    void changeProgramName (int index, const juce::String& newName) override
    {
        juce::ignoreUnused (index, newName);
    }

    //=================================================================================================================

    // Unused in Anagram but required by JUCE, do nothing
    // In Anagram the host will be the one saving the state, not the plugin
    void getStateInformation (juce::MemoryBlock& destData) override
    {
        juce::ignoreUnused (destData);
    }

    // Unused in Anagram but required by JUCE, do nothing
    // In Anagram the host will be the one saving the state, not the plugin
    void setStateInformation (const void* data, int sizeInBytes) override
    {
        juce::ignoreUnused (data, sizeInBytes);
    }

    //=================================================================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExampleAudioProcessor)
};

//=====================================================================================================================
// This creates new instances of the plugin..

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ExampleAudioProcessor();
}

//=====================================================================================================================
