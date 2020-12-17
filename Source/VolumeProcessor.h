/*
  ==============================================================================

    VolumeProcessor.h
    Created: 29 Oct 2020 1:45:48pm
    Author:  Simon

  ==============================================================================
*/

#pragma once

class VolumeProcessor {
public:
    VolumeProcessor() {}

    void volumeProcessSample(float* sampleLeft, float* sampleRight, float gain) {
        *sampleLeft *= gain;
        *sampleRight *= gain;
    }

    void phaseProcessSample(float* sampleLeft, float* sampleRight, float frequency, float sampleRate, float min, float max) {
        updatePhaseFilter(frequency, sampleRate, min, max);
        leftFilter.processSample(sampleLeft, frequency, 0.0f, sampleRate, FilterType::APF);
        rightFilter.processSample(sampleRight, frequency, 0.0f, sampleRate, FilterType::APF);
    }

    void pannerProcessSample(float* sampleLeft, float* sampleRight, float pan) {
        if (pan < 0.0f) {
            float weight = pan * -1.0f;
            *sampleLeft = *sampleLeft * (0.5f * (1.0f - weight) + 0.5f) + *sampleRight * (0.5f * weight);
            *sampleRight = *sampleRight * (1.0f - weight);
        }
        else {
            float weight = pan;
            *sampleRight = *sampleRight * (0.5f * (1.0f - weight) + 0.5f) + *sampleLeft * (0.5f * weight);
            *sampleLeft = *sampleLeft * (1.0f - weight);
        }
    }

    void updatePhaseFilter(float degree, float sampleRate, float min, float max) {
        float hz = convertPhaseDegreeToHz(degree, max, min);
        leftFilter.updateCoefficients(hz, 0.0f, sampleRate, FilterType::APF);
        rightFilter.updateCoefficients(hz, 0.0f, sampleRate, FilterType::APF);
    }

    float convertPhaseDegreeToHz(float degree, float maxFrequency, float minFrequency) {
        return (maxFrequency + minFrequency) - (minFrequency + (convertLinearDegreeToExponential(degree) / 180) * (maxFrequency - minFrequency));
    }

    float convertLinearDegreeToExponential(float degree) {
        return ((std::log2(((degree - 0) * (std::pow(2, 1 * 10) - 1) / (180 - 0)) + 1)) / 10) * 180;
    }

    void filterSnapToZero() {
        leftFilter.snapToZero();
        rightFilter.snapToZero();
    }

private:
    Filter leftFilter;
    Filter rightFilter;
};

