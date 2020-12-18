#pragma once

#include <JuceHeader.h>
#include "FilterProcessor.h"
#include "MidSideProcessor.h"
#include "VolumeProcessor.h"

//==============================================================================
/**
*/
class BUtilityAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BUtilityAudioProcessor();
    ~BUtilityAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    void setPreviousParametersToCurrentOnes();
    void setAllParametersToDefault();
    float calculateSmoothParameterStep(float parameter, float previousParameter, float numSamples);
    void processVolume(float* sampleLeft, float* sampleRight, VolumeProcessor* volume, float gain, float pan, float phase, float sampleRate);
    void processFilter(float* sampleLeft, float* sampleRight, FilterProcessor* filter, FilterType type, float frequency, float resonance, float secondFilter, float proportional, float sampleRate);
    void processMidSide(float* sampleLeft, float* sampleRight, MidSideProcessor* midSide, float width, float frequency, float aboveBelow, float solo, float sampleRate);
    juce::Point<int> getSavedSize() const;
    void setSavedState(const juce::Point<int>& size);

private:
    const float filterMinFrequency = 20.0f;
    const float filterMaxFreqeuncy = 20000.0f;

    const bool defaultEnabled = true;
    const float defaultGain = 0.0f;
    const float defaultPan = 0.0f;
    const float defaultPhase = 0.0f;
    const float defaultMidSideWidth = 0.0f;
    const float defaultMidSideFrequency = filterMaxFreqeuncy;
    const bool defaultMidSideSolo = false;
    const float defaultMidSideAboveBelow = 0.0f;
    const float defaultHPFrequency = filterMinFrequency;
    const float defaultLPFrequency = filterMaxFreqeuncy;
    const float defaultResonance = 0.0f;
    const bool defaultSecondFilter = false;
    const bool defaultProportional = true;

    juce::AudioProcessorValueTreeState parameters;

    std::atomic<float>* enabledParameter = nullptr;

    std::atomic<float>* inputGainParameter = nullptr;
    std::atomic<float>* inputPanParameter = nullptr;
    std::atomic<float>* inputPhaseParameter = nullptr;

    std::atomic<float>* midSideWidthParameter = nullptr;
    std::atomic<float>* midSideFrequencyParameter = nullptr;
    std::atomic<float>* midSideSoloParameter = nullptr;
    std::atomic<float>* midSideAboveOrBelowParameter = nullptr;

    std::atomic<float>* highPassFrequencyParameter = nullptr;
    std::atomic<float>* highPassResonanceParameter = nullptr;
    std::atomic<float>* highPassProportionalParameter = nullptr;
    std::atomic<float>* highPassSecondFilterParameter = nullptr;

    std::atomic<float>* lowPassFrequencyParameter = nullptr;
    std::atomic<float>* lowPassResonanceParameter = nullptr;
    std::atomic<float>* lowPassProportionalParameter = nullptr;
    std::atomic<float>* lowPassSecondFilterParameter = nullptr;

    std::atomic<float>* outputGainParameter = nullptr;
    std::atomic<float>* outputPanParameter = nullptr;
    std::atomic<float>* outputPhaseParameter = nullptr;

    float previousInputGain;
    float previousInputPan;
    float previousInputPhase;

    float previousMidSideWidth;
    float previousMidSideFrequency;

    float previousHPFrequency;
    float previousHPResonance;
    float previousHPProportional;

    float previousLPFrequency;
    float previousLPResonance;
    float previousLPProportional;

    float previousOutputGain;
    float previousOutputPan;
    float previousOutputPhase;

    VolumeProcessor inputProcessor;
    MidSideProcessor midSideProcessor;
    FilterProcessor highPassProcessor;
    FilterProcessor lowPassProcessor;
    VolumeProcessor outputProcessor;

    std::atomic<float>* editorSizeXParameter = nullptr;
    std::atomic<float>* editorSizeYParameter = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BUtilityAudioProcessor)
};
