#pragma once

struct BUtilityPanel : public juce::Component {
    BUtilityPanel(juce::Colour c) : backgroundColour(c) {
        
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(backgroundColour);
    }

    void resized() override {

    }

    juce::Colour backgroundColour;
};

class BUtilityLookAndFeel : public juce::LookAndFeel_V4 {
public:
    BUtilityLookAndFeel() {
        //setColour(juce::Label::outlineColourId, juce::Colours::red);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityLookAndFeel);
};

class BUtilitySlider : public juce::Slider {
public:
    BUtilitySlider() {
        setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        setSliderStyle(juce::Slider::RotaryVerticalDrag);
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilitySlider);
};

class BUtilityLabel : public juce::Label {
public:
    BUtilityLabel() {
        setJustificationType(juce::Justification::centred);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityLabel);
};

class BUtilityToggleButton : public juce::ToggleButton {
public:
    BUtilityToggleButton() {
        
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityToggleButton);
};

class BUtilityTextButton : public juce::TextButton {
public:
    BUtilityTextButton() {

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityTextButton);
};

class BUtilityDecibelSlider : public BUtilitySlider {
public:
    BUtilityDecibelSlider() {}

    double getValueFromText(const juce::String& text) override {
        auto minusInfinitydB = -100.0;
        auto decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();

        return decibelText.equalsIgnoreCase("-INF") ? minusInfinitydB : decibelText.getDoubleValue();
    }

    juce::String getTextFromValue(double value) override {
        return juce::Decibels::toString(value, 1);
    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityDecibelSlider);
};

class BUtilityFrequencySlider : public BUtilitySlider {
public:
    BUtilityFrequencySlider() {}

    juce::String getTextFromValue(double value) override {
        return juce::String(value) + " Hz";
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityFrequencySlider);
};

class BUtilityResonanceSlider : public BUtilitySlider {
public:
    BUtilityResonanceSlider() {
        setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityResonanceSlider);
};

class BUtilitySwitchSlider : public BUtilitySlider {
public:
    BUtilitySwitchSlider() {
        setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        setColour(juce::Slider::backgroundColourId, getLookAndFeel().findColour(juce::Slider::trackColourId));
    }

    juce::String getTextFromValue(double value) override {
        return value > 0.5 ? "Above" : "Below";
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilitySwitchSlider);
};


class BUtilityBalanceSlider : public BUtilitySlider {
public:
    BUtilityBalanceSlider() {

    }

    juce::String getTextFromValue(double value) override {
        auto balanceValue = juce::String(juce::roundToInt(value * 100));
        if (value < 0)
            return balanceValue + "% Left";
        if (value > 0)
            return balanceValue + "% Right";
        if (value == 0)
            return "Centred";
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityBalanceSlider);
};

class BUtilityPhaseSlider : public BUtilitySlider {
public:
    BUtilityPhaseSlider() {

    }

    juce::String getTextFromValue(double value) override {
        return juce::String(value) + " deg";
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityPhaseSlider);
};

class BUtilityWidthSlider : public BUtilitySlider {
public:
    BUtilityWidthSlider() {

    }

    juce::String getTextFromValue(double value) override {
        auto balanceValue = juce::String(juce::roundToInt(value * 100));
        if (value < 0)
            return balanceValue + "% Merged";
        if (value > 0)
            return balanceValue + "% Seperated";
        if (value == 0)
            return "Original";
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityWidthSlider);
};

class BUtilityEnabledButton : public juce::TextButton {
public:
    BUtilityEnabledButton(const juce::String& buttonText) {
        setButtonText(buttonText);
        setClickingTogglesState(true);
        setColour(juce::TextButton::textColourOnId, juce::Colours::darkgreen);
        setColour(juce::TextButton::textColourOffId, juce::Colours::darkred);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityEnabledButton);
};

class BUtilityHeaderLabel : public BUtilityLabel {
public:
    BUtilityHeaderLabel() {
        
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BUtilityHeaderLabel);
};

const double FREQUENCY_EXP_FACTOR = 10;
static inline juce::NormalisableRange<float> frequencyRange(float min, float max) {
    return juce::NormalisableRange<float>(min, max,
        [](float start, float end, float normalised) { return start + (std::pow(2, normalised * FREQUENCY_EXP_FACTOR) - 1) * (end - start) / (std::pow(2, 1 * FREQUENCY_EXP_FACTOR) - 1); },
        [](float start, float end, float value) { return (std::log2(((value - start) * (std::pow(2, 1 * FREQUENCY_EXP_FACTOR) - 1) / (end - start)) + 1)) / FREQUENCY_EXP_FACTOR; },
        [](float start, float end, float value) { return juce::jlimit((float)start, (float)end, (float)juce::roundToInt(value));
        });
}

const double DECIBEL_LOG_FACTOR = 634.684832012561;
static inline juce::NormalisableRange<float> decibelRange(float min, float max) {
    return juce::NormalisableRange<float>(min, max,
        [](float start, float end, float normalised) { return start + std::log2(normalised * DECIBEL_LOG_FACTOR + 1) * (end - start) / std::log2(1 * DECIBEL_LOG_FACTOR + 1); },
        [](float start, float end, float value) { return (std::pow(2, (std::log2(1 * DECIBEL_LOG_FACTOR + 1) * value - start * std::log2(1 * DECIBEL_LOG_FACTOR + 1)) / (end - start)) - 1) / DECIBEL_LOG_FACTOR; },
        [](float start, float end, float value) { return juce::jlimit((float)start, (float)end, (float)value);
        });
}
