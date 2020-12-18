#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BUtilityAudioProcessor::BUtilityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
     ), parameters(*this, nullptr, juce::Identifier("bUtilityParameters"), {
         std::make_unique<juce::AudioParameterBool>("enabled", "bUtility", defaultEnabled),
         std::make_unique<juce::AudioParameterFloat>(
             "inputGain",
             "Gain",
             decibelRange(-100.0f, 12.0f),
             defaultGain,
             "dB",
             juce::AudioProcessorParameter::genericParameter,
             [this](float value, int maximumStringLength) { return value == -100.0f ? "-INF" : juce::String(value, 1); }),

         std::make_unique<juce::AudioParameterFloat>(
             "inputPanning",
             "Panning",
             juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f),
             defaultPan,
             "",      
             juce::AudioProcessorParameter::genericParameter,
             [this](float value, int maximumStringLength) {
             if (value < 0)
                return juce::String(value * 100, 0) + "% Left";
            if (value > 0)
                return juce::String(value * 100, 0) + "% Right";
            if (value == 0)
                return juce::String("Centred");}),

         std::make_unique<juce::AudioParameterFloat>("inputPhase", "Phase", juce::NormalisableRange<float>(0, 180.0f, 1.0f), defaultPhase, "deg"),

         std::make_unique<juce::AudioParameterFloat>(
             "midSideWidth",
             "Width",
             juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f),
             defaultMidSideWidth,
             "",
             juce::AudioProcessorParameter::genericParameter,
             [this](float value, int maximumStringLength) {
                 if (value < 0)
                    return juce::String(value * 100, 0) + "% Merged";
                if (value > 0)
                    return juce::String(value * 100, 0) + "% Seperated";
                if (value == 0)
                    return juce::String("Original");}),
         std::make_unique<juce::AudioParameterFloat>(
             "midSideFrequency",
             "Frequency",
             frequencyRange(filterMinFrequency, filterMaxFreqeuncy),
             defaultMidSideFrequency,
             "Hz",
             juce::AudioProcessorParameter::genericParameter,
             [this](float value, int maximumStringLength) { return juce::String(value, 0); }),

         std::make_unique<juce::AudioParameterBool>("midSideSolo", "Solo", defaultMidSideSolo),
         std::make_unique<juce::AudioParameterFloat>(
             "midSideAboveBelow",
             "Below",
             juce::NormalisableRange<float>(0.0f, 1.0f, 1.0f),
             0.0f,
             "",
             juce::AudioProcessorParameter::genericParameter,
             [this](float value, int maximumStringLength) { return value > 0.5 ? "Above" : "Below"; }),

         std::make_unique<juce::AudioParameterFloat>(
             "highPassFrequency",
             "Frequency",
             frequencyRange(filterMinFrequency, filterMaxFreqeuncy),
             defaultHPFrequency,
             "Hz",
             juce::AudioProcessorParameter::genericParameter,
             [this](float value, int maximumStringLength) { return juce::String(value, 0); }),
         std::make_unique<juce::AudioParameterFloat>("highPassResonance", "Resonance", juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), defaultResonance),

         std::make_unique<juce::AudioParameterBool>("highPassProportional", "Proportional", defaultProportional),
         std::make_unique<juce::AudioParameterBool>("highPassSecondFilter", "x2", defaultSecondFilter),

         std::make_unique<juce::AudioParameterFloat>(
             "lowPassFrequency",
             "Frequency",
             frequencyRange(filterMinFrequency, filterMaxFreqeuncy),
             defaultLPFrequency,
             "Hz",
             juce::AudioProcessorParameter::genericParameter,
             [this](float value, int maximumStringLength) { return juce::String(value, 0); }),
         std::make_unique<juce::AudioParameterFloat>("lowPassResonance", "Resonance", juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), defaultResonance),

         std::make_unique<juce::AudioParameterBool>("lowPassProportional", "Proportional", defaultProportional),
         std::make_unique<juce::AudioParameterBool>("lowPassSecondFilter", "x2", defaultSecondFilter),

         std::make_unique<juce::AudioParameterFloat>(
             "outputGain",
             "Gain",
             decibelRange(-100.0f, 12.0f),
             defaultGain,
             "dB",
             juce::AudioProcessorParameter::genericParameter,
             [this](float value, int maximumStringLength) { return value == -100.0f ? "-INF" : juce::String(value, 1); }),

         std::make_unique<juce::AudioParameterFloat>(
             "outputPanning",
             "Panning",
             juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f),
             defaultPan,
             "",
             juce::AudioProcessorParameter::genericParameter,
             [this](float value, int maximumStringLength) {
             if (value < 0)
                return juce::String(value * 100, 0) + "% Left";
            if (value > 0)
                return juce::String(value * 100, 0) + "% Right";
            if (value == 0)
                return juce::String("Centred");}),

         std::make_unique<juce::AudioParameterFloat>("outputPhase", "Phase", juce::NormalisableRange<float>(0, 180.0f, 1.0f), defaultPhase, "deg"),

         std::make_unique<juce::AudioParameterFloat>("EDITOR_X", "editor size x", juce::NormalisableRange<float>(0.0f, 1000000.0f, 1.0f), DEFAULT_SIZE_X),
         std::make_unique<juce::AudioParameterFloat>("EDITOR_Y", "editor size y", juce::NormalisableRange<float>(0.0f, 1000000.0f, 1.0f), DEFAULT_SIZE_Y),
      })
#endif
{
    enabledParameter = parameters.getRawParameterValue("enabled");

    inputGainParameter = parameters.getRawParameterValue("inputGain");
    inputPanParameter = parameters.getRawParameterValue("inputPanning");
    inputPhaseParameter = parameters.getRawParameterValue("inputPhase");

    midSideWidthParameter = parameters.getRawParameterValue("midSideWidth");
    midSideFrequencyParameter = parameters.getRawParameterValue("midSideFrequency");
    midSideSoloParameter = parameters.getRawParameterValue("midSideSolo");
    midSideAboveOrBelowParameter = parameters.getRawParameterValue("midSideAboveBelow");

    highPassFrequencyParameter = parameters.getRawParameterValue("highPassFrequency");
    highPassResonanceParameter = parameters.getRawParameterValue("highPassResonance");
    highPassProportionalParameter = parameters.getRawParameterValue("highPassProportional");
    highPassSecondFilterParameter = parameters.getRawParameterValue("highPassSecondFilter");

    lowPassFrequencyParameter = parameters.getRawParameterValue("lowPassFrequency");
    lowPassResonanceParameter = parameters.getRawParameterValue("lowPassResonance");
    lowPassProportionalParameter = parameters.getRawParameterValue("lowPassProportional");
    lowPassSecondFilterParameter = parameters.getRawParameterValue("lowPassSecondFilter");

    outputGainParameter = parameters.getRawParameterValue("outputGain");
    outputPanParameter = parameters.getRawParameterValue("outputPanning");
    outputPhaseParameter = parameters.getRawParameterValue("outputPhase");

    editorSizeXParameter = parameters.getRawParameterValue("EDITOR_X");
    editorSizeYParameter = parameters.getRawParameterValue("EDITOR_Y");
    
}

BUtilityAudioProcessor::~BUtilityAudioProcessor() {
}


void BUtilityAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    setPreviousParametersToCurrentOnes();
}

void BUtilityAudioProcessor::releaseResources() {
    //This call crashes FL Studio as soon as the plugin is bypassed. Why? No one knows!
    //clearFilters();
}

void BUtilityAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto numSamples = buffer.getNumSamples();
    auto sampleRate = getSampleRate();

    float smoothInputGain = previousInputGain;
    float inputGainStep = calculateSmoothParameterStep(*inputGainParameter, previousInputGain, numSamples);

    float smoothInputPan = previousInputPan;
    float inputPanStep = calculateSmoothParameterStep(*inputPanParameter, previousInputPan, numSamples);

    float smoothInputPhase = previousInputPhase;
    float inputPhaseStep = calculateSmoothParameterStep(*inputPhaseParameter, previousInputPhase, numSamples);

    float smoothMidSideWidth = previousMidSideWidth;
    float midSideWidthStep = calculateSmoothParameterStep(*midSideWidthParameter, previousMidSideWidth, numSamples);

    float smoothMidSideFrequency = previousMidSideFrequency;
    float midSideFrequencyStep = calculateSmoothParameterStep(*midSideFrequencyParameter, previousMidSideFrequency, numSamples);

    float smoothLPFrequency = previousLPFrequency;
    float LPFrequencyStep = calculateSmoothParameterStep(*lowPassFrequencyParameter, previousLPFrequency, numSamples);

    float smoothLPResonance = previousLPResonance;
    float LPResonanceStep = calculateSmoothParameterStep(*lowPassResonanceParameter, previousLPResonance, numSamples);

    float smoothHPFrequency = previousHPFrequency;
    float HPFrequencyStep = calculateSmoothParameterStep(*highPassFrequencyParameter, previousHPFrequency, numSamples);

    float smoothHPResonance = previousHPResonance;
    float HPResonanceStep = calculateSmoothParameterStep(*highPassResonanceParameter, previousHPResonance, numSamples);

    float smoothOutputGain = previousOutputGain;
    float outputGainStep = calculateSmoothParameterStep(*outputGainParameter, previousOutputGain, numSamples);

    float smoothOutputPan = previousOutputPan;
    float outputPanStep = calculateSmoothParameterStep(*outputPanParameter, previousOutputPan, numSamples);

    float smoothOutputPhase = previousOutputPhase;
    float outputPhaseStep = calculateSmoothParameterStep(*outputPhaseParameter, previousOutputPhase, numSamples);

    for (int sample = 0; sample < numSamples; ++sample) {
        float* sampleLeft = &buffer.getWritePointer(0)[sample];
        float* sampleRight = &buffer.getWritePointer(1)[sample];

        if (*enabledParameter) {
            processVolume(sampleLeft, sampleRight, &inputProcessor, smoothInputGain, smoothInputPan, smoothInputPhase, sampleRate);
            processMidSide(sampleLeft, sampleRight, &midSideProcessor, smoothMidSideWidth, smoothMidSideFrequency, *midSideAboveOrBelowParameter, *midSideSoloParameter, sampleRate);
            processFilter(sampleLeft, sampleRight, &highPassProcessor, FilterType::highPass, smoothHPFrequency, smoothHPResonance, *highPassSecondFilterParameter, *highPassProportionalParameter, sampleRate);
            processFilter(sampleLeft, sampleRight, &lowPassProcessor, FilterType::lowPass, smoothLPFrequency, smoothLPResonance, *lowPassSecondFilterParameter, *lowPassProportionalParameter, sampleRate);
            processVolume(sampleLeft, sampleRight, &outputProcessor, smoothOutputGain, smoothOutputPan, smoothOutputPhase, sampleRate);
        }

        smoothInputGain += inputGainStep;
        smoothInputPan += inputPanStep;
        smoothInputPhase += inputPhaseStep;
        smoothMidSideWidth += midSideWidthStep;
        smoothMidSideFrequency += midSideFrequencyStep;
        smoothLPFrequency += LPFrequencyStep;
        smoothLPResonance += LPResonanceStep;
        smoothHPFrequency += HPFrequencyStep;
        smoothHPResonance += HPResonanceStep;
        smoothOutputGain += outputGainStep;
        smoothOutputPan += outputPanStep;
        smoothOutputPhase += outputPhaseStep;
    }
    inputProcessor.filterSnapToZero();
    midSideProcessor.filterSnapToZero();
    highPassProcessor.filterSnapToZero();
    lowPassProcessor.filterSnapToZero();
    outputProcessor.filterSnapToZero();

    setPreviousParametersToCurrentOnes();
}

//==============================================================================


void BUtilityAudioProcessor::setPreviousParametersToCurrentOnes() {
    previousInputGain = *inputGainParameter;
    previousInputPan = *inputPanParameter;
    previousInputPhase = *inputPhaseParameter;

    previousMidSideWidth = *midSideWidthParameter;
    previousMidSideFrequency = *midSideFrequencyParameter;
    
    previousHPFrequency = *highPassFrequencyParameter;
    previousHPResonance = *highPassResonanceParameter;
    previousHPProportional = *highPassProportionalParameter;

    previousLPFrequency = *lowPassFrequencyParameter;
    previousLPResonance = *lowPassResonanceParameter;
    previousLPProportional = *lowPassProportionalParameter;

    previousOutputGain = *outputGainParameter;
    previousOutputPan = *outputPanParameter;
    previousOutputPhase = *outputPhaseParameter;
}

void BUtilityAudioProcessor::setAllParametersToDefault() {
    *enabledParameter = defaultEnabled;

    *inputGainParameter = defaultGain;
    *inputPanParameter = defaultPan;
    *inputPhaseParameter = defaultPhase;

    *midSideWidthParameter = defaultMidSideWidth;
    *midSideFrequencyParameter = defaultMidSideFrequency;
    *midSideSoloParameter = defaultMidSideSolo;
    *midSideAboveOrBelowParameter = defaultMidSideAboveBelow;

    *highPassFrequencyParameter = defaultHPFrequency;
    *highPassResonanceParameter = defaultResonance;
    *highPassProportionalParameter = defaultProportional;
    *highPassSecondFilterParameter = defaultSecondFilter;

    *lowPassFrequencyParameter = defaultLPFrequency;
    *lowPassResonanceParameter = defaultResonance;
    *lowPassProportionalParameter = defaultProportional;
    *lowPassSecondFilterParameter = defaultSecondFilter;

    *outputGainParameter = defaultGain;
    *outputPanParameter = defaultPan;
    *outputPhaseParameter = defaultPhase;
}

float BUtilityAudioProcessor::calculateSmoothParameterStep(float parameter, float previousParameter, float numSamples) {
    return (parameter - previousParameter) / numSamples;
}

void BUtilityAudioProcessor::processVolume(float* sampleLeft, float* sampleRight, VolumeProcessor* volume, float gain, float pan, float phase, float sampleRate) {
    if (gain != defaultGain)
        volume->volumeProcessSample(sampleLeft, sampleRight, juce::Decibels::decibelsToGain(gain));
    if (phase != defaultPhase)
        volume->phaseProcessSample(sampleLeft, sampleRight, phase, sampleRate, filterMinFrequency, filterMaxFreqeuncy);
    if (pan != defaultPan)
        volume->pannerProcessSample(sampleLeft, sampleRight, pan);
}

void BUtilityAudioProcessor::processMidSide(float* sampleLeft, float* sampleRight, MidSideProcessor* midSide, float width, float frequency, float aboveBelow, float solo, float sampleRate) {
    // what makes more sense? panner first or width first?
    // todo wird eh in meherer funktionen gesplittet
    // lol ne wirds nicht u suck 
    if (width != defaultMidSideWidth || frequency != defaultMidSideFrequency || solo != defaultMidSideSolo)
        midSide->midSideProcessSample(sampleLeft, sampleRight, frequency, sampleRate, aboveBelow, solo, width);
}

void BUtilityAudioProcessor::processFilter(float* sampleLeft, float* sampleRight, FilterProcessor* filter, FilterType type, float frequency, float resonance, float secondFilter, float proportional, float sampleRate) {
    if (type == FilterType::lowPass && (frequency != previousLPFrequency || resonance != previousLPResonance || proportional != previousLPProportional) ||
        type == FilterType::highPass && (frequency != previousHPFrequency || resonance != previousHPResonance || proportional != previousHPProportional))

        filter->updateFilter(frequency, resonance, sampleRate, filterMinFrequency, filterMaxFreqeuncy, proportional, type);

    if (type == FilterType::lowPass && (frequency != defaultLPFrequency) || type == FilterType::highPass && (frequency != defaultHPFrequency) || resonance != defaultResonance) {
        filter->processSample(sampleLeft, sampleRight, frequency, resonance, sampleRate, type);
        if (secondFilter != defaultSecondFilter) {
            filter->switchState();
            filter->processSample(sampleLeft, sampleRight, frequency, resonance, sampleRate, type);
            filter->switchState();
        }
    }
}

juce::Point<int> BUtilityAudioProcessor::getSavedSize() const {
    return { (int)*editorSizeXParameter, (int)*editorSizeYParameter };
}

void BUtilityAudioProcessor::setSavedState(const juce::Point<int>& size) {
    *editorSizeXParameter = size.x;
    *editorSizeYParameter = size.y;
}

//==============================================================================
const juce::String BUtilityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BUtilityAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool BUtilityAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool BUtilityAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double BUtilityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BUtilityAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BUtilityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BUtilityAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String BUtilityAudioProcessor::getProgramName(int index)
{
    return {};
}

void BUtilityAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
#ifndef JucePlugin_PreferredChannelConfigurations
bool BUtilityAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

//==============================================================================
bool BUtilityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BUtilityAudioProcessor::createEditor()
{
    return new BUtilityAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void BUtilityAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BUtilityAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BUtilityAudioProcessor();
}
