#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "EditorElements.h"

const int DEFAULT_SIZE_X = 850;
const int DEFAULT_SIZE_Y = 360;

//==============================================================================
/**
*/
class BUtilityAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BUtilityAudioProcessorEditor (BUtilityAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~BUtilityAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void paintValueIfMouseHover();
    
    void setAllToCurrentValue();

    void setSizeAndResizeLimit(float panelWidth, float panelHeight, float toolbarHeight);

    void setVolumeSectionBounds(BUtilityPanel panel, BUtilityHeaderLabel label, BUtilityDecibelSlider gainSlider, BUtilityLabel gainLabel, BUtilitySlider balanceSlider, BUtilityLabel balanceLabel, BUtilitySlider phaseSlider, BUtilityLabel phaseLabel, float fontSize, float headerFontSize);
    void setInputSectionBounds(float fontSize, float headerFontSize);
    void setMidSideSectionBounds(float fontSize, float headerFontSize);
    void setHighPassSectionBounds(float fontSize, float headerFontSize);
    void setLowPassSectionBounds(float fontSize, float headerFontSize);
    void setOutputSectionBounds(float fontSize, float headerFontSize);
    void setToolbarSectionBounds(float fontSize, float headerFontSize);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BUtilityAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    BUtilityLookAndFeel bUtilityLookAndFeel;

    float guiScale = 1.0f;

    // TOOL BAR
    BUtilityTextButton toolBarResetAllParametersButton;

    BUtilityEnabledButton toolBarEnabledButton{ "bUtility" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> toolBarEnabledAttachment;

    juce::ComboBox toolBarGuiScaleComboBox;

    // INPUT SECTION
    BUtilityHeaderLabel inputLabel;

    BUtilityDecibelSlider inputGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;
    BUtilityLabel inputGainLabel;

    BUtilityBalanceSlider inputBalanceSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputBalanceAttachment;
    BUtilityLabel inputBalanceLabel;

    BUtilityPhaseSlider inputPhaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputPhaseAttachment;
    BUtilityLabel inputPhaseLabel;

    // MID SIDE SECTION
    BUtilityHeaderLabel midSideLabel;

    BUtilityWidthSlider midSideWitdthSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midSideWitdthAttachment;
    BUtilityLabel midSideWitdthLabel;

    BUtilityFrequencySlider midSideFrequencySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midSideFrequencyAttachment;
    BUtilityLabel midSideFrequencyLabel;

    BUtilityToggleButton midSideSoloButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midSideSoloButtonAttachment;

    BUtilitySwitchSlider midSideFlipSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midSideFlipAttachment;
    BUtilityLabel midSideFlipLabel;

    // HIGH PASS SECTION
    BUtilityHeaderLabel highPassLabel;

    BUtilityFrequencySlider highPassFrequencySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highPassFrequencyAttachment;
    BUtilityLabel highPassFrequencyLabel;

    BUtilityResonanceSlider highPassResonanceSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highPassResonanceAttachment;
    BUtilityLabel highPassResonanceLabel;

    BUtilityToggleButton highPassProportionalButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highPassProportionalAttachment;

    BUtilityToggleButton highPassSecondFilterButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highPassSecondFilterAttachment;

    // LOW PASS SECTION
    BUtilityHeaderLabel lowPassLabel;

    BUtilityFrequencySlider lowPassFrequencySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowPassFrequencyAttachment;
    BUtilityLabel lowPassFrequencyLabel;

    BUtilityResonanceSlider lowPassResonanceSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowPassResonanceAttachment;
    BUtilityLabel lowPassResonanceLabel;

    BUtilityToggleButton lowPassProportionalButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowPassProportionalAttachment;

    BUtilityToggleButton lowPassSecondFilterButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowPassSecondFilterAttachment;

    // OUTPUT SECTION
    BUtilityHeaderLabel outputLabel;

    BUtilityDecibelSlider outputGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;
    BUtilityLabel outputGainLabel;

    BUtilityBalanceSlider outputBalanceSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputBalanceAttachment;
    BUtilityLabel outputBalanceLabel;

    BUtilityPhaseSlider outputPhaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputPhaseAttachment;
    BUtilityLabel outputPhaseLabel;

    BUtilityPanel inputPanel;
    BUtilityPanel midSidePanel;
    BUtilityPanel highPassPanel;
    BUtilityPanel lowPassPanel;
    BUtilityPanel outputPanel;
    BUtilityPanel toolbarPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BUtilityAudioProcessorEditor)
};
