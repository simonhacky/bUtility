#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
BUtilityAudioProcessorEditor::BUtilityAudioProcessorEditor(BUtilityAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), audioProcessor(p), valueTreeState(vts),
    inputPanel(juce::Colour::fromRGB(70, 81, 86)),
    midSidePanel(juce::Colour::fromRGB(90, 100, 105)),
    highPassPanel(juce::Colour::fromRGB(70, 81, 86)),
    lowPassPanel(juce::Colour::fromRGB(90, 100, 105)),
    outputPanel(juce::Colour::fromRGB(70, 81, 86)),
    toolbarPanel(juce::Colour::fromRGB(30, 37, 40))
        
{
    addAndMakeVisible(inputPanel);
    addAndMakeVisible(midSidePanel);
    addAndMakeVisible(highPassPanel);
    addAndMakeVisible(lowPassPanel);
    addAndMakeVisible(outputPanel);
    addAndMakeVisible(toolbarPanel);

    // INPUT SECTION
    inputLabel.setText("Input", juce::NotificationType::dontSendNotification);
    inputPanel.addAndMakeVisible(inputLabel);

    inputPanel.addAndMakeVisible(inputGainSlider);
    inputGainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "inputGain", inputGainSlider));
    inputPanel.addAndMakeVisible(inputGainLabel);

    inputPanel.addAndMakeVisible(inputBalanceSlider);
    inputBalanceAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "inputPanning", inputBalanceSlider));
    inputPanel.addAndMakeVisible(inputBalanceLabel);

    inputPanel.addAndMakeVisible(inputPhaseSlider);
    inputPhaseAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "inputPhase", inputPhaseSlider));
    inputPanel.addAndMakeVisible(inputPhaseLabel);

    // MID SIDE SECTION
    midSideLabel.setText("Mid Side", juce::NotificationType::dontSendNotification);
    midSidePanel.addAndMakeVisible(midSideLabel);

    midSidePanel.addAndMakeVisible(midSideWitdthSlider);
    midSideWitdthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "midSideWidth", midSideWitdthSlider));
    midSidePanel.addAndMakeVisible(midSideWitdthLabel);

    midSidePanel.addAndMakeVisible(midSideFrequencySlider);
    midSideFrequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "midSideFrequency", midSideFrequencySlider));
    midSidePanel.addAndMakeVisible(midSideFrequencyLabel);

    midSideSoloButton.setButtonText("Solo");
    midSidePanel.addAndMakeVisible(midSideSoloButton);
    midSideSoloButtonAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "midSideSolo", midSideSoloButton));

    midSidePanel.addAndMakeVisible(midSideFlipSlider);
    midSideFlipAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "midSideAboveBelow", midSideFlipSlider));
    midSidePanel.addAndMakeVisible(midSideFlipLabel);

    // HIGH PASS SECTION
    highPassLabel.setText("High Pass", juce::NotificationType::dontSendNotification);
    highPassPanel.addAndMakeVisible(highPassLabel);

    highPassPanel.addAndMakeVisible(highPassFrequencySlider);
    highPassFrequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "highPassFrequency", highPassFrequencySlider));
    highPassPanel.addAndMakeVisible(highPassFrequencyLabel);

    highPassPanel.addAndMakeVisible(highPassResonanceSlider);
    highPassResonanceAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "highPassResonance", highPassResonanceSlider));
    highPassPanel.addAndMakeVisible(highPassResonanceLabel);

    highPassSecondFilterButton.setButtonText("x2");
    highPassPanel.addAndMakeVisible(highPassSecondFilterButton);
    highPassSecondFilterAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "highPassSecondFilter", highPassSecondFilterButton));

    highPassProportionalButton.setButtonText("Proportional");
    highPassPanel.addAndMakeVisible(highPassProportionalButton);
    highPassProportionalAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "highPassProportional", highPassProportionalButton));

    // LOW PASS SECTION
    lowPassLabel.setText("Low Pass", juce::NotificationType::dontSendNotification);
    lowPassPanel.addAndMakeVisible(lowPassLabel);

    lowPassPanel.addAndMakeVisible(lowPassFrequencySlider);
    lowPassFrequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lowPassFrequency", lowPassFrequencySlider));
    lowPassPanel.addAndMakeVisible(lowPassFrequencyLabel);

    lowPassPanel.addAndMakeVisible(lowPassResonanceSlider);
    lowPassResonanceAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lowPassResonance", lowPassResonanceSlider));
    lowPassPanel.addAndMakeVisible(lowPassResonanceLabel);

    lowPassSecondFilterButton.setButtonText("x2");
    lowPassPanel.addAndMakeVisible(lowPassSecondFilterButton);
    lowPassSecondFilterAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lowPassSecondFilter", lowPassSecondFilterButton));

    lowPassProportionalButton.setButtonText("Proportional");
    lowPassPanel.addAndMakeVisible(lowPassProportionalButton);
    lowPassProportionalAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "lowPassProportional", lowPassProportionalButton));

    // OUTPUT SECTION
    outputLabel.setText("Output", juce::NotificationType::dontSendNotification);
    outputPanel.addAndMakeVisible(outputLabel);

    outputPanel.addAndMakeVisible(outputGainSlider);
    outputGainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "outputGain", outputGainSlider));
    outputPanel.addAndMakeVisible(outputGainLabel);

    outputPanel.addAndMakeVisible(outputBalanceSlider);
    outputBalanceAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "outputPanning", outputBalanceSlider));
    outputPanel.addAndMakeVisible(outputBalanceLabel);

    outputPanel.addAndMakeVisible(outputPhaseSlider);
    outputPhaseAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "outputPhase", outputPhaseSlider));
    outputPanel.addAndMakeVisible(outputPhaseLabel);

    // TOOL BAR
    toolBarResetAllParametersButton.setButtonText("Reset Parameters");
    toolbarPanel.addAndMakeVisible(toolBarResetAllParametersButton);
    toolBarResetAllParametersButton.onClick = [this]() {
        audioProcessor.setAllParametersToDefault();
        setAllToCurrentValue();
    };

    toolBarGuiScaleComboBox.addItem("50 %", 1);
    toolBarGuiScaleComboBox.addItem("75 %", 2);
    toolBarGuiScaleComboBox.addItem("100 %", 3);
    toolBarGuiScaleComboBox.addItem("150 %", 4);
    toolBarGuiScaleComboBox.addItem("200 %", 5);
    toolBarGuiScaleComboBox.addItem("400 %", 6);
    toolBarGuiScaleComboBox.onChange = [this]() {
        if (toolBarGuiScaleComboBox.getSelectedId() == 1)
            guiScale = 0.5;
        else if (toolBarGuiScaleComboBox.getSelectedId() == 2)
            guiScale = 0.75;
        else if (toolBarGuiScaleComboBox.getSelectedId() == 3)
            guiScale = 1;
        else if (toolBarGuiScaleComboBox.getSelectedId() == 4)
            guiScale = 1.5;
        else if (toolBarGuiScaleComboBox.getSelectedId() == 5)
            guiScale = 2;
        else if (toolBarGuiScaleComboBox.getSelectedId() == 6)
            guiScale = 4;
        resized();
        setSize(ceil(DEFAULT_SIZE_X * guiScale), ceil(DEFAULT_SIZE_Y * guiScale));};
    toolBarGuiScaleComboBox.setSelectedId(3);
    toolBarGuiScaleComboBox.setJustificationType(juce::Justification::left);
    toolbarPanel.addAndMakeVisible(toolBarGuiScaleComboBox);

    toolbarPanel.addAndMakeVisible(toolBarEnabledButton);
    toolBarEnabledAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, "enabled", toolBarEnabledButton));

    setLookAndFeel(&bUtilityLookAndFeel);

    auto size = audioProcessor.getSavedSize();
    setResizable(true, true);
    setSize(size.x, size.y);
}

BUtilityAudioProcessorEditor::~BUtilityAudioProcessorEditor()
{
}

//==============================================================================
void BUtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    paintValueIfMouseHover();
}

void BUtilityAudioProcessorEditor::paintValueIfMouseHover() {
    if (inputGainSlider.isMouseOverOrDragging())
        inputGainLabel.setText(inputGainSlider.getTextFromValue(inputGainSlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        inputGainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
    if (inputBalanceSlider.isMouseOverOrDragging())
        inputBalanceLabel.setText(inputBalanceSlider.getTextFromValue(inputBalanceSlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        inputBalanceLabel.setText("Balance", juce::NotificationType::dontSendNotification);
    if (inputPhaseSlider.isMouseOverOrDragging())
        inputPhaseLabel.setText(inputPhaseSlider.getTextFromValue(inputPhaseSlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        inputPhaseLabel.setText("Phase", juce::NotificationType::dontSendNotification);
    if (midSideWitdthSlider.isMouseOverOrDragging())
        midSideWitdthLabel.setText(midSideWitdthSlider.getTextFromValue(midSideWitdthSlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        midSideWitdthLabel.setText("Width", juce::NotificationType::dontSendNotification);
    if (midSideFrequencySlider.isMouseOverOrDragging())
        midSideFrequencyLabel.setText(midSideFrequencySlider.getTextFromValue(midSideFrequencySlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        midSideFrequencyLabel.setText("Frequency", juce::NotificationType::dontSendNotification);
    if (highPassFrequencySlider.isMouseOverOrDragging())
        highPassFrequencyLabel.setText(highPassFrequencySlider.getTextFromValue(highPassFrequencySlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        highPassFrequencyLabel.setText("Frequency", juce::NotificationType::dontSendNotification);
    if (highPassResonanceSlider.isMouseOverOrDragging())
        highPassResonanceLabel.setText(highPassResonanceSlider.getTextFromValue(highPassResonanceSlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        highPassResonanceLabel.setText("Reso", juce::NotificationType::dontSendNotification);
    if (lowPassFrequencySlider.isMouseOverOrDragging())
        lowPassFrequencyLabel.setText(lowPassFrequencySlider.getTextFromValue(lowPassFrequencySlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        lowPassFrequencyLabel.setText("Frequency", juce::NotificationType::dontSendNotification);
    if (lowPassResonanceSlider.isMouseOverOrDragging())
        lowPassResonanceLabel.setText(lowPassResonanceSlider.getTextFromValue(lowPassResonanceSlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        lowPassResonanceLabel.setText("Reso", juce::NotificationType::dontSendNotification);
    if (outputGainSlider.isMouseOverOrDragging())
        outputGainLabel.setText(outputGainSlider.getTextFromValue(outputGainSlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        outputGainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
    if (outputBalanceSlider.isMouseOverOrDragging())
        outputBalanceLabel.setText(outputBalanceSlider.getTextFromValue(outputBalanceSlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        outputBalanceLabel.setText("Balance", juce::NotificationType::dontSendNotification);
    if (outputPhaseSlider.isMouseOverOrDragging())
        outputPhaseLabel.setText(outputPhaseSlider.getTextFromValue(outputPhaseSlider.getValue()), juce::NotificationType::dontSendNotification);
    else
        outputPhaseLabel.setText("Phase", juce::NotificationType::dontSendNotification);
    midSideFlipLabel.setText(midSideFlipSlider.getTextFromValue(midSideFlipSlider.getValue()), juce::NotificationType::dontSendNotification);
}

void BUtilityAudioProcessorEditor::setAllToCurrentValue() {
    inputGainSlider.setValue(*valueTreeState.getRawParameterValue("inputGain"));
    inputBalanceSlider.setValue(*valueTreeState.getRawParameterValue("inputPanning"));
    inputPhaseSlider.setValue(*valueTreeState.getRawParameterValue("inputPhase"));
    midSideWitdthSlider.setValue(*valueTreeState.getRawParameterValue("midSideWidth"));
    midSideFrequencySlider.setValue(*valueTreeState.getRawParameterValue("midSideFrequency"));
    midSideSoloButton.setToggleState(*valueTreeState.getRawParameterValue("midSideSolo") > 0.5 ? true : false, true);
    midSideFlipSlider.setValue(*valueTreeState.getRawParameterValue("midSideAboveBelow"));
    highPassFrequencySlider.setValue(*valueTreeState.getRawParameterValue("highPassFrequency"));
    highPassResonanceSlider.setValue(*valueTreeState.getRawParameterValue("highPassResonance"));
    highPassProportionalButton.setToggleState(*valueTreeState.getRawParameterValue("highPassProportional") > 0.5 ? true : false, true);
    highPassSecondFilterButton.setToggleState(*valueTreeState.getRawParameterValue("highPassSecondFilter") > 0.5 ? true : false, true);
    lowPassFrequencySlider.setValue(*valueTreeState.getRawParameterValue("lowPassFrequency"));
    lowPassResonanceSlider.setValue(*valueTreeState.getRawParameterValue("lowPassResonance"));
    lowPassProportionalButton.setToggleState(*valueTreeState.getRawParameterValue("lowPassProportional") > 0.5 ? true : false, true);
    lowPassSecondFilterButton.setToggleState(*valueTreeState.getRawParameterValue("lowPassSecondFilter") > 0.5 ? true : false, true);
    outputGainSlider.setValue(*valueTreeState.getRawParameterValue("outputGain"));
    outputBalanceSlider.setValue(*valueTreeState.getRawParameterValue("outputPanning"));
    outputPhaseSlider.setValue(*valueTreeState.getRawParameterValue("outputPhase"));
    toolBarEnabledButton.setToggleState(*valueTreeState.getRawParameterValue("enabled") > 0.5 ? true : false, true);
}

void BUtilityAudioProcessorEditor::resized()
{  
    auto toolbarHeight = (DEFAULT_SIZE_Y / 9) * guiScale;
    auto panelWidth = floor(DEFAULT_SIZE_X  * guiScale / 5);
    auto panelHeight = DEFAULT_SIZE_Y  * guiScale - toolbarHeight;
    auto fontSize = panelHeight / 20;
    auto headerFontSize = panelHeight / 10;

    audioProcessor.setSavedState({ getWidth(), (int)(DEFAULT_SIZE_Y * guiScale + toolbarHeight) });
    setSizeAndResizeLimit(panelWidth, panelHeight, toolbarHeight);

    auto area = getLocalBounds();
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    fb.alignContent = juce::FlexBox::AlignContent::flexStart;

    juce::FlexItem flex1(panelWidth, panelHeight, inputPanel);
    juce::FlexItem flex2(panelWidth, panelHeight, midSidePanel);
    juce::FlexItem flex3(panelWidth, panelHeight, highPassPanel);
    juce::FlexItem flex4(panelWidth, panelHeight, lowPassPanel);
    juce::FlexItem flex5(panelWidth, panelHeight, outputPanel);

    fb.items.addArray ( { flex1, flex2, flex3, flex4, flex5 } );

    fb.performLayout(area);

    toolbarPanel.setBounds(area.removeFromBottom(toolbarHeight));

    setInputSectionBounds(fontSize, headerFontSize);
    setMidSideSectionBounds(fontSize, headerFontSize);
    setHighPassSectionBounds(fontSize, headerFontSize);
    setLowPassSectionBounds(fontSize, headerFontSize);
    setOutputSectionBounds(fontSize, headerFontSize);
    setToolbarSectionBounds(fontSize, headerFontSize);
}

void BUtilityAudioProcessorEditor::setSizeAndResizeLimit(float panelWidth, float panelHeight, float toolbarHeight) {
    if (getWidth() >= panelWidth * 5) {
        setResizeLimits(panelWidth, panelHeight + toolbarHeight, panelWidth * 5, panelHeight + toolbarHeight);
    }
    if (getWidth() < panelWidth * 5 && getWidth() > panelWidth * 4) {
        if (getHeight() > panelHeight + toolbarHeight && getHeight() < panelHeight * 2 + toolbarHeight)
            setSize(getWidth(), panelHeight * 2 + toolbarHeight);
        setSize(panelWidth * 4, getHeight());
        setResizeLimits(panelWidth, panelHeight + toolbarHeight, panelWidth * 5, panelHeight * 2 + toolbarHeight);
    }
    if (getWidth() < panelWidth * 4 && getWidth() > panelWidth * 3) {
        if (getHeight() > panelHeight + toolbarHeight && getHeight() < panelHeight * 2 + toolbarHeight)
            setSize(getWidth(), panelHeight * 2 + toolbarHeight);
        setSize(panelWidth * 3, getHeight());
        setResizeLimits(panelWidth, panelHeight + toolbarHeight, panelWidth * 5, panelHeight * 2 + toolbarHeight);
    }
    if (getWidth() < panelWidth * 3 && getWidth() > panelWidth * 2) {
        if (getHeight() > panelHeight + toolbarHeight && getHeight() < panelHeight * 2 + toolbarHeight)
            setSize(getWidth(), panelHeight * 2 + toolbarHeight);
        if (getHeight() > panelHeight * 2 + toolbarHeight && getHeight() < panelHeight * 3 + toolbarHeight)
            setSize(getWidth(), panelHeight * 3 + toolbarHeight);
        setSize(panelWidth * 2, getHeight());
        setResizeLimits(panelWidth, panelHeight + toolbarHeight, panelWidth * 5, panelHeight * 3 + toolbarHeight);
    }
    if (getWidth() < panelWidth * 2) {
        if (getHeight() > panelHeight + toolbarHeight && getHeight() < panelHeight * 2 + toolbarHeight)
            setSize(getWidth(), panelHeight * 2 + toolbarHeight);
        if (getHeight() > panelHeight * 2 + toolbarHeight && getHeight() < panelHeight * 3 + toolbarHeight)
            setSize(getWidth(), panelHeight * 3 + toolbarHeight);
        if (getHeight() > panelHeight * 3 + toolbarHeight && getHeight() < panelHeight * 4 + toolbarHeight)
            setSize(getWidth(), panelHeight * 4 + toolbarHeight);
        if (getHeight() > panelHeight * 4 + toolbarHeight)
            setSize(getWidth(), panelHeight * 5 + toolbarHeight);
        setSize(panelWidth * 1, getHeight());
        setResizeLimits(panelWidth, panelHeight + toolbarHeight, panelWidth * 5, panelHeight * 5 + toolbarHeight);
    }
}

void BUtilityAudioProcessorEditor::setInputSectionBounds(float fontSize, float headerFontSize) {
    auto inputArea = inputPanel.getLocalBounds();
    inputLabel.setBounds(inputArea.removeFromTop(inputArea.getHeight() / 5));
    inputLabel.setFont(headerFontSize);
    auto inputGainArea = inputArea.removeFromTop(inputArea.getHeight() / 3 * 2);
    inputGainLabel.setBounds(inputGainArea.removeFromBottom(inputGainArea.getHeight() / 5));
    inputGainLabel.setFont(fontSize);
    inputGainSlider.setBounds(inputGainArea);
    auto inputBalanceArea = inputArea.removeFromLeft(inputArea.getWidth() / 2);
    inputBalanceLabel.setBounds(inputBalanceArea.removeFromBottom(inputBalanceArea.getHeight() / 5));
    inputBalanceLabel.setFont(fontSize);
    inputBalanceSlider.setBounds(inputBalanceArea);
    inputPhaseLabel.setBounds(inputArea.removeFromBottom(inputArea.getHeight() / 5));
    inputPhaseLabel.setFont(fontSize);
    inputPhaseSlider.setBounds(inputArea);
}

void BUtilityAudioProcessorEditor::setMidSideSectionBounds(float fontSize, float headerFontSize) {
    auto midSideArea = midSidePanel.getLocalBounds();
    midSideLabel.setBounds(midSideArea.removeFromTop(midSideArea.getHeight() / 5));
    midSideLabel.setFont(headerFontSize);
    auto widthArea = midSideArea.removeFromTop(midSideArea.getHeight() / 3);
    midSideWitdthLabel.setBounds(widthArea.removeFromBottom(widthArea.getHeight() / 5));
    midSideWitdthLabel.setFont(fontSize);
    midSideWitdthSlider.setBounds(widthArea);
    auto frequencyArea = midSideArea.removeFromTop(midSideArea.getHeight() / 2);
    midSideFrequencyLabel.setBounds(frequencyArea.removeFromBottom(frequencyArea.getHeight() / 5));
    midSideFrequencyLabel.setFont(fontSize);
    midSideFrequencySlider.setBounds(frequencyArea);
    midSideSoloButton.setBounds(midSideArea.removeFromLeft(midSideArea.getWidth() / 2));
    midSideFlipLabel.setBounds(midSideArea.removeFromBottom(midSideArea.getHeight() / 5));
    midSideFlipLabel.setFont(fontSize);
    midSideFlipSlider.setBounds(midSideArea);
}

void BUtilityAudioProcessorEditor::setHighPassSectionBounds(float fontSize, float headerFontSize) {
    auto highPassArea = highPassPanel.getLocalBounds();
    highPassLabel.setBounds(highPassArea.removeFromTop(highPassArea.getHeight() / 5));
    highPassLabel.setFont(headerFontSize);
    auto resonanceArea = highPassArea.removeFromRight(highPassArea.getWidth() / 3);
    highPassResonanceLabel.setBounds(resonanceArea.removeFromBottom(resonanceArea.getHeight() / 5));
    highPassResonanceLabel.setFont(fontSize);
    highPassResonanceSlider.setBounds(resonanceArea);
    auto frequencyArea = highPassArea.removeFromTop(highPassArea.getHeight() / 2);
    highPassFrequencyLabel.setBounds(frequencyArea.removeFromBottom(frequencyArea.getHeight() / 5));
    highPassFrequencyLabel.setFont(fontSize);
    highPassFrequencySlider.setBounds(frequencyArea);
    highPassProportionalButton.setBounds(highPassArea.removeFromTop(highPassArea.getHeight() / 2));
    highPassSecondFilterButton.setBounds(highPassArea);
}

void BUtilityAudioProcessorEditor::setLowPassSectionBounds(float fontSize, float headerFontSize) {
    auto lowPassArea = lowPassPanel.getLocalBounds();
    lowPassLabel.setBounds(lowPassArea.removeFromTop(lowPassArea.getHeight() / 5));
    lowPassLabel.setFont(headerFontSize);
    auto resonanceArea = lowPassArea.removeFromRight(lowPassArea.getWidth() / 3);
    lowPassResonanceLabel.setBounds(resonanceArea.removeFromBottom(resonanceArea.getHeight() / 5));
    lowPassResonanceLabel.setFont(fontSize);
    lowPassResonanceSlider.setBounds(resonanceArea);
    auto frequencyArea = lowPassArea.removeFromTop(lowPassArea.getHeight() / 2);
    lowPassFrequencyLabel.setBounds(frequencyArea.removeFromBottom(frequencyArea.getHeight() / 5));
    lowPassFrequencyLabel.setFont(fontSize);
    lowPassFrequencySlider.setBounds(frequencyArea);
    lowPassProportionalButton.setBounds(lowPassArea.removeFromTop(lowPassArea.getHeight() / 2));
    lowPassSecondFilterButton.setBounds(lowPassArea);
}

void BUtilityAudioProcessorEditor::setOutputSectionBounds(float fontSize, float headerFontSize) {
    auto outputArea = inputPanel.getLocalBounds();
    outputLabel.setBounds(outputArea.removeFromTop(outputArea.getHeight() / 5));
    outputLabel.setFont(headerFontSize);
    auto inputGainArea = outputArea.removeFromTop(outputArea.getHeight() / 3 * 2);
    outputGainLabel.setBounds(inputGainArea.removeFromBottom(inputGainArea.getHeight() / 5));
    outputGainLabel.setFont(fontSize);
    outputGainSlider.setBounds(inputGainArea);
    auto inputBalanceArea = outputArea.removeFromLeft(outputArea.getWidth() / 2);
    outputBalanceLabel.setBounds(inputBalanceArea.removeFromBottom(inputBalanceArea.getHeight() / 5));
    outputBalanceLabel.setFont(fontSize);
    outputBalanceSlider.setBounds(inputBalanceArea);
    outputPhaseLabel.setBounds(outputArea.removeFromBottom(outputArea.getHeight() / 5));
    outputPhaseLabel.setFont(fontSize);
    outputPhaseSlider.setBounds(outputArea);
}

void BUtilityAudioProcessorEditor::setToolbarSectionBounds(float fontSize, float headerFontSize) {
    auto elementWidth = 170 * guiScale;
    auto toolbarArea = toolbarPanel.getLocalBounds();
    toolBarGuiScaleComboBox.setBounds(toolbarArea.removeFromRight(elementWidth));
    toolBarResetAllParametersButton.setBounds(toolbarArea.removeFromLeft(elementWidth));
    toolBarEnabledButton.setBounds(toolbarArea.removeFromLeft(elementWidth));
}
