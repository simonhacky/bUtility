#pragma once

#include "Filter.h"

class MidSideProcessor {
public:
    MidSideProcessor() {}

    void midSideProcessSample(float* sampleLeft, float* sampleRight, float frequency, float sampleRate, float aboveBelow, float solo, float width) {
        float sampleLeftAbove = *sampleLeft;
        float sampleLeftBelow = *sampleLeft;
        float sampleRightAbove = *sampleRight;
        float sampleRightBelow = *sampleRight;
        updateFilters(frequency, sampleRate);
        filterLeftAbove.processSample(&sampleLeftAbove, frequency, 0.0, sampleRate, FilterType::HPF1);
        filterLeftBelow.processSample(&sampleLeftBelow, frequency, 0.0, sampleRate, FilterType::LPF1);
        filterRightAbove.processSample(&sampleRightAbove, frequency, 0.0, sampleRate, FilterType::HPF1);
        filterRightBelow.processSample(&sampleRightBelow, frequency, 0.0, sampleRate, FilterType::LPF1);
        if (aboveBelow > 0.5)
            widthProcessSample(&sampleLeftAbove, &sampleRightAbove, width);
        else
            widthProcessSample(&sampleLeftBelow, &sampleRightBelow, width);
        if (solo > 0.5) {
            *sampleLeft = aboveBelow > 0.5 ? sampleLeftAbove : sampleLeftBelow;
            *sampleRight = aboveBelow > 0.0 ? sampleRightAbove : sampleRightBelow;
        } else {
            *sampleLeft = sampleLeftAbove + sampleLeftBelow;
            *sampleRight = sampleRightAbove + sampleRightBelow;
        }
    }

    void updateFilters(float frequency, float sampleRate) {
        filterLeftAbove.updateCoefficients(frequency, 0.0, sampleRate, FilterType::HPF1);
        filterLeftBelow.updateCoefficients(frequency, 0.0, sampleRate, FilterType::LPF1);
        filterRightAbove.updateCoefficients(frequency, 0.0, sampleRate, FilterType::HPF1);
        filterRightBelow.updateCoefficients(frequency, 0.0, sampleRate, FilterType::LPF1);
    }

    void widthProcessSample(float* sampleLeft, float* sampleRight, float width) {
        float sampleMid = (*sampleLeft + *sampleRight) / sqrt(2);
        float sampleSide = (*sampleLeft - *sampleRight) / sqrt(2);
        if (width < 0) {
            sampleSide *= (1 - width * -1);
        }
        else {
            sampleMid *= 1 - width;
        }
        *sampleLeft = (sampleMid + sampleSide) / sqrt(2);
        *sampleRight = (sampleMid - sampleSide) / sqrt(2);
    }

    void filterSnapToZero() {
        filterLeftAbove.snapToZero();
        filterLeftBelow.snapToZero();
        filterRightAbove.snapToZero();
        filterRightBelow.snapToZero();
    }

private:
    Filter filterLeftAbove;
    Filter filterLeftBelow;
    Filter filterRightAbove;
    Filter filterRightBelow;
};
