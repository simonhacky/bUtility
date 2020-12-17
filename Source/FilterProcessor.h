/*
  ==============================================================================

    FilterProcessor.h
    Created: 29 Oct 2020 1:45:41pm
    Author:  Simon

  ==============================================================================
*/

#pragma once

#include "Filter.h"

class FilterProcessor {
public:
    FilterProcessor() {
        wetPercentage = 1.0f;
        makeUp = 1.0f;
    }

    void processSample(float* sampleLeft, float* sampleRight, float frequency, float resonance, float sampleRate, FilterType type) {
        float sampleLeftDry = *sampleLeft;
        float sampleRightDry = *sampleRight;
        leftFilter.processSample(sampleLeft, frequency, resonance, sampleRate, type);
        rightFilter.processSample(sampleRight, frequency, resonance, sampleRate, type);

        *sampleLeft = *sampleLeft * wetPercentage * makeUp + sampleLeftDry * (1.0f - wetPercentage);
        *sampleRight = *sampleRight * wetPercentage * makeUp + sampleRightDry * (1.0f - wetPercentage);
    }

    void updateFilter(float frequency, float resonance, float sampleRate, float filterMinFrequency, float filterMaxFreqeuncy, float proportional, FilterType type) {
        if (type == FilterType::lowPass || type == FilterType::highPass) {
            if (type == FilterType::lowPass && resonance < 1.0f) {
                wetPercentage = calculateWetAmount(frequency, filterMaxFreqeuncy, 0.8, type);
                wetPercentage += (1 - wetPercentage) * resonance;
                resonance += 1 - wetPercentage;
            }
            resonance = convertLinearResonanceToExponential(resonance);
            if (proportional > 0.5)
                resonance += addResonance(frequency, resonance, filterMinFrequency, filterMaxFreqeuncy, type);
            makeUp = calculateMakeUp(frequency, resonance, filterMaxFreqeuncy, type);
        }
        leftFilter.updateCoefficients(frequency, resonance, sampleRate, type);
        rightFilter.updateCoefficients(frequency, resonance, sampleRate, type);
    }

    void switchState() {
        leftFilter.switchState();
        rightFilter.switchState();
    }

    void filterSnapToZero() {
        leftFilter.snapToZero();
        rightFilter.snapToZero();
    }

    float convertLinearResonanceToExponential(float resonance) {
        return pow((juce::MathConstants<double>::euler / 2.29), resonance) - 1 + 0.707;
    }

    float addResonance(float frequency, float resonance, float minFrequency, float maxFrequency, FilterType type) {
        if (type == FilterType::lowPass)
            return sqrt((maxFrequency - frequency) / maxFrequency) * 0.25;
        if (type == FilterType::highPass)
            return sqrt((maxFrequency - frequency) / maxFrequency) * 0.25 * ((sqrt(frequency - minFrequency) * 0.25) > 1 ? 1.0 : (sqrt(frequency - minFrequency) * 0.25)) + pow(frequency / maxFrequency, 2);
    }

    float calculateMakeUp(float frequency, float resonance, float maxFrequency, FilterType type) {
        if (type == FilterType::highPass)
            return (1 - resonance * 0.1) * pow(frequency / maxFrequency, 2) * 3 + 1;
        return 1;
    }

    float calculateWetAmount(float frequency, float maxFrequency, float percentageOfAffectedFrequencies, FilterType type) {
        if (frequency < maxFrequency * percentageOfAffectedFrequencies)
            return 1;
        return sqrt((maxFrequency - frequency) / (maxFrequency * (1 - percentageOfAffectedFrequencies)));
    }

private:
    Filter leftFilter;
    Filter rightFilter;

    float wetPercentage;
    float makeUp;
};
