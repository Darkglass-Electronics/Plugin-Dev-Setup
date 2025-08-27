// Example plugin for Anagram using JUCE
// Copyright (C) 2025 Filipe Coelho <falktx@darkglass.com>
// SPDX-License-Identifier: ISC

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

//==================================================================================

class ExampleAudioProcessor : public juce::AudioProcessor {
public:
    //==============================================================================
    ExampleAudioProcessor();
    ~ExampleAudioProcessor() override;

    //==============================================================================
    /** Returns the name of this processor. */
    const juce::String getName() const override;

    //==============================================================================
    /** Called before playback starts, to let the processor prepare itself.

        The sample rate is the target sample rate, and will remain constant until
        playback stops.

        You can call getTotalNumInputChannels and getTotalNumOutputChannels
        or query the busLayout member variable to find out the number of
        channels your processBlock callback must process.

        The maximumExpectedSamplesPerBlock value is a strong hint about the maximum
        number of samples that will be provided in each block. You may want to use
        this value to resize internal buffers. You should program defensively in
        case a buggy host exceeds this value. The actual block sizes that the host
        uses may be different each time the callback happens: completely variable
        block sizes can be expected from some hosts.

       @see busLayout, getTotalNumInputChannels, getTotalNumOutputChannels
    */
    void prepareToPlay (double sampleRate,
                        int maximumExpectedSamplesPerBlock) override;

    /** Called after playback has stopped, to let the object free up any resources it
        no longer needs.
    */
    void releaseResources() override;

    /** Renders the next block.

        When this method is called, the buffer contains a number of channels which is
        at least as great as the maximum number of input and output channels that
        this processor is using. It will be filled with the processor's input data and
        should be replaced with the processor's output.

        So for example if your processor has a total of 2 input channels and 4 output
        channels, then the buffer will contain 4 channels, the first two being filled
        with the input data. Your processor should read these, do its processing, and
        replace the contents of all 4 channels with its output.

        Or if your processor has a total of 5 inputs and 2 outputs, the buffer will have 5
        channels, all filled with data, and your processor should overwrite the first 2 of
        these with its output. But be VERY careful not to write anything to the last 3
        channels, as these might be mapped to memory that the host assumes is read-only!

        If your plug-in has more than one input or output buses then the buffer passed
        to the processBlock methods will contain a bundle of all channels of each bus.
        Use getBusBuffer to obtain an audio buffer for a particular bus.

        Note that if you have more outputs than inputs, then only those channels that
        correspond to an input channel are guaranteed to contain sensible data - e.g.
        in the case of 2 inputs and 4 outputs, the first two channels contain the input,
        but the last two channels may contain garbage, so you should be careful not to
        let this pass through without being overwritten or cleared.

        Also note that the buffer may have more channels than are strictly necessary,
        but you should only read/write from the ones that your processor is supposed to
        be using.

        The number of samples in these buffers is NOT guaranteed to be the same for every
        callback, and may be more or less than the estimated value given to prepareToPlay().
        Your code must be able to cope with variable-sized blocks, or you're going to get
        clicks and crashes!

        Also note that some hosts will occasionally decide to pass a buffer containing
        zero samples, so make sure that your algorithm can deal with that!

        If the processor is receiving a MIDI input, then the midiMessages array will be filled
        with the MIDI messages for this block. Each message's timestamp will indicate the
        message's time, as a number of samples from the start of the block.

        Any messages left in the MIDI buffer when this method has finished are assumed to
        be the processor's MIDI output. This means that your processor should be careful to
        clear any incoming messages from the array if it doesn't want them to be passed-on.

        If you have implemented the getBypassParameter method, then you need to check the
        value of this parameter in this callback and bypass your processing if the parameter
        has a non-zero value.

        Note that when calling this method as a host, the result may still be bypassed as
        the parameter that controls the bypass may be non-zero.

        Be very careful about what you do in this callback - it's going to be called by
        the audio thread, so any kind of interaction with the UI is absolutely
        out of the question. If you change a parameter in here and need to tell your UI to
        update itself, the best way is probably to inherit from a ChangeBroadcaster, let
        the UI components register as listeners, and then call sendChangeMessage() inside the
        processBlock() method to send out an asynchronous message. You could also use
        the AsyncUpdater class in a similar way.

        @see getBusBuffer
    */
    void processBlock (juce::AudioBuffer<float>& buffer,
                       juce::MidiBuffer& midiMessages) override;

    //==============================================================================

    /** Returns the length of the processor's tail, in seconds. */
    double getTailLengthSeconds() const override;

    /** Returns true if the processor wants MIDI messages.

        This must return the same value every time it is called.
        This may be called by the audio thread, so this should be fast.
        Ideally, just return a constant.
    */
    bool acceptsMidi() const override;

    /** Returns true if the processor produces MIDI messages.

        This must return the same value every time it is called.
        This may be called by the audio thread, so this should be fast.
        Ideally, just return a constant.
    */
    bool producesMidi() const override;

    /** Returns true if this is a MIDI effect plug-in and does no audio processing.

        This must return the same value every time it is called.
        This may be called by the audio thread, so this should be fast.
        Ideally, just return a constant.
    */
    bool isMidiEffect() const override;

    //==============================================================================
    /** Creates the processor's GUI.

        This can return nullptr if you want a GUI-less processor, in which case the host
        may create a generic UI that lets the user twiddle the parameters directly.

        If you do want to pass back a component, the component should be created and set to
        the correct size before returning it. If you implement this method, you must
        also implement the hasEditor() method and make it return true.

        Remember not to do anything silly like allowing your processor to keep a pointer to
        the component that gets created - it could be deleted later without any warning, which
        would make your pointer into a dangler. Use the getActiveEditor() method instead.

        The correct way to handle the connection between an editor component and its
        processor is to use something like a ChangeBroadcaster so that the editor can
        register itself as a listener, and be told when a change occurs. This lets them
        safely unregister themselves when they are deleted.

        Here are a few things to bear in mind when writing an editor:

        - Initially there won't be an editor, until the user opens one, or they might
          not open one at all. Your processor mustn't rely on it being there.
        - An editor object may be deleted and a replacement one created again at any time.
        - It's safe to assume that an editor will be deleted before its processor.

        @see hasEditor
    */
    juce::AudioProcessorEditor* createEditor() override;

    /** Your processor subclass must override this and return true if it can create an
        editor component.
        @see createEditor
    */
    bool hasEditor() const override;

    //==============================================================================
    /** Returns the number of preset programs the processor supports.

        The value returned must be valid as soon as this object is created, and
        must not change over its lifetime.

        This value shouldn't be less than 1.
    */
    int getNumPrograms() override;

    /** Returns the number of the currently active program. */
    int getCurrentProgram() override;

    /** Called by the host to change the current program. */
    void setCurrentProgram (int index) override;

    /** Must return the name of a given program. */
    const juce::String getProgramName (int index) override;

    /** Called by the host to rename a program. */
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    /** The host will call this method when it wants to save the processor's internal state.

        This must copy any info about the processor's state into the block of memory provided,
        so that the host can store this and later restore it using setStateInformation().

        Note that there's also a getCurrentProgramStateInformation() method, which only
        stores the current program, not the state of the entire processor.

        See also the helper function copyXmlToBinary() for storing settings as XML.

        @see getCurrentProgramStateInformation
    */
    void getStateInformation (juce::MemoryBlock& destData) override;

    /** This must restore the processor's state from a block of data previously created
        using getStateInformation().

        Note that there's also a setCurrentProgramStateInformation() method, which tries
        to restore just the current program, not the state of the entire processor.

        See also the helper function getXmlFromBinary() for loading settings as XML.

        In the case that this AudioProcessor is implementing a VST3 that has declared compatible
        plugins via VST3ClientExtensions::getCompatibleClasses(), the state passed to this
        function may have been created by one of these compatible plugins.

        If the parameter IDs of the current plugin differ from the IDs of the plugin whose state
        was passed to this function, you can use information from the plugin state
        to determine which parameter mapping to use if necessary.
        VST3ClientExtensions::getCompatibleParameterIds() will always be called after
        setStateInformation(), and that function should return the parameter mapping from the most
        recently-loaded state.

        @see setCurrentProgramStateInformation, VST3ClientExtensions::getCompatibleParameterIds
    */
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::AudioParameterFloat* gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExampleAudioProcessor)
};

//==================================================================================
