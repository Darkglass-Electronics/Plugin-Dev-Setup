// Example plugin for Anagram using JUCE
// Copyright (C) 2025 Filipe Coelho <falktx@darkglass.com>
// SPDX-License-Identifier: ISC

#include "PluginProcessor.h"

/* TODO
 * - add bypass parameter
 * - use parameter smoothing
 * - bring back regular juce code for non-anagram builds
 * - add "-Inf" enumerated value (is it possible with juce??)
 */

//=====================================================================================================================

static auto dbParam()
{
    return juce::AudioParameterFloatAttributes().withLabel ("dB");
}

//=====================================================================================================================

ExampleAudioProcessor::ExampleAudioProcessor()
    : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::mono())
                                       .withOutput ("Output", juce::AudioChannelSet::mono()))
{
    addParameter (gain = new juce::AudioParameterFloat ({ "gain", 1 }, "Gain", { -40.0f, 20.0f }, 0.f, dbParam()));
}

ExampleAudioProcessor::~ExampleAudioProcessor() {}

//=====================================================================================================================

const juce::String ExampleAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

//=====================================================================================================================

void ExampleAudioProcessor::prepareToPlay (double sampleRate,
                                           int maximumExpectedSamplesPerBlock)
{
}

void ExampleAudioProcessor::releaseResources()
{
}

void ExampleAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                          juce::MidiBuffer& midiMessages)
{
    const float gainCoef = juce::Decibels::decibelsToGain(gain->get());
    buffer.applyGain(gainCoef);

    midiMessages.clear();
}

//=====================================================================================================================

bool ExampleAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ExampleAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ExampleAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ExampleAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//=====================================================================================================================

bool ExampleAudioProcessor::hasEditor() const
{
    return false;
}

juce::AudioProcessorEditor* ExampleAudioProcessor::createEditor()
{
    return nullptr;
}

//=====================================================================================================================

int ExampleAudioProcessor::getNumPrograms()
{
    return 0;
}

int ExampleAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ExampleAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ExampleAudioProcessor::getProgramName (int index)
{
    return {};
}

void ExampleAudioProcessor::changeProgramName (int index,
                                               const juce::String& newName)
{
}

//=====================================================================================================================

void ExampleAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void ExampleAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//=====================================================================================================================

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ExampleAudioProcessor();
}

//=====================================================================================================================
