/*
  ==============================================================================

    Filter.h
    Created: 2 Nov 2020 5:40:27pm
    Author:  Simon

  ==============================================================================
*/

#pragma once

enum class FilterType {
    highPass,
    lowPass,
    HPF1,
    LPF1,
    APF
};

class Filter {
public:
    Filter() {
        reset();
    }

    void processSample(float* sample, float frequency, float resonance, float sampleRate, FilterType type) {
        auto xn = *sample;
        auto yn = coeff0 * xn + state1;

        state1 = coeff1 * xn - coeff3 * yn + state2;
        state2 = coeff2 * xn - coeff4 * yn;

        *sample = yn;
    }

    void snapToZero() {
        JUCE_SNAP_TO_ZERO(state1);
        JUCE_SNAP_TO_ZERO(state2)
    }

    void updateCoefficients(float frequency, float resonance, float sampleRate, FilterType type) { 
        if (type == FilterType::lowPass) {
            double theta_c = 2.0 * juce::MathConstants<float>::pi * frequency / sampleRate;
            double q = 1.0 / (2.0 * resonance);
            double b_1 = 0.0;
            double b_2 = exp(-2.0 * q * theta_c);
            if (q <= 1.0)
                b_1 = -2.0 * exp(-q * theta_c) * cos(pow((1.0 - q * q), 0.5) * theta_c);
            else
                b_1 = -2.0 * exp(-q * theta_c) * cosh(pow((q * q - 1.0), 0.5) * theta_c);

            double B0 = (1.0 + b_1 + b_2) * (1.0 + b_1 + b_2);
            double B1 = (1.0 - b_1 + b_2) * (1.0 - b_1 + b_2);
            double B2 = -4.0 * b_2;
            double phi_0 = 1.0 - sin(theta_c / 2.0) * sin(theta_c / 2.0);
            double phi_1 = sin(theta_c / 2.0) * sin(theta_c / 2.0);
            double phi_2 = 4.0 * phi_0 * phi_1;
            double R1 = (B0 * phi_0 + B1 * phi_1 + B2 * phi_2) * (resonance * resonance);
            double A0 = B0;
            double A1 = (R1 - A0 * phi_0) / phi_1;
            if (A0 < 0.0)
                A0 = 0.0;
            if (A1 < 0.0)
                A1 = 0.0;
            double a_0 = 0.5 * (pow(A0, 0.5) + pow(A1, 0.5));
            double a_1 = pow(A0, 0.5) - a_0;
            double a_2 = 0.0;
            coeff0 = a_0;
            coeff1 = a_1;
            coeff2 = a_2;
            coeff3 = b_1;
            coeff4 = b_2;
        }
        else if (type == FilterType::highPass) {
            double theta_c = 2.0 * juce::MathConstants<float>::pi * frequency / sampleRate;
            double q = 1.0 / (2.0 * resonance);
            double b_1 = 0.0;
            double b_2 = exp(-2.0 * q * theta_c);
            if (q <= 1.0)
                b_1 = -2.0 * exp(-q * theta_c) * cos(pow((1.0 - q * q), 0.5) * theta_c);
            else
                b_1 = -2.0 * exp(-q * theta_c) * cosh(pow((q * q - 1.0), 0.5) * theta_c);

            double B0 = (1.0 + b_1 + b_2) * (1.0 + b_1 + b_2);
            double B1 = (1.0 - b_1 + b_2) * (1.0 - b_1 + b_2);
            double B2 = -4.0 * b_2;
            double phi_0 = 1.0 - sin(theta_c / 2.0) * sin(theta_c / 2.0);
            double phi_1 = sin(theta_c / 2.0) * sin(theta_c / 2.0);
            double phi_2 = 4.0 * phi_0 * phi_1;
            double R1 = (B0 * phi_0 + B1 * phi_1 + B2 * phi_2) * (resonance * resonance);
            double A0 = B0;
            double A1 = (R1 - A0 * phi_0) / phi_1;
            if (A0 < 0.0)
                A0 = 0.0;
            if (A1 < 0.0)
                A1 = 0.0;
            double a_0 = (sqrt(B0 * phi_0 + B1 * phi_1 + B2 * phi_2) / (4.0 * phi_1)) * resonance;
            double a_1 = -2.0 * a_0;
            double a_2 = a_0;
            coeff0 = a_0;
            coeff1 = a_1;
            coeff2 = a_2;
            coeff3 = b_1;
            coeff4 = b_2;
        } else if (type == FilterType::LPF1) {
            double theta_c = 2.0 * juce::MathConstants<float>::pi * frequency / sampleRate;
            double gamma = cos(theta_c) / (1.0 + sin(theta_c));
            double alpha = (1 - gamma) / 2;
            coeff0 = alpha;
            coeff1 = alpha;
            coeff2 = 0;
            coeff3 = -gamma;
            coeff4 = 0;
        } else if (type == FilterType::HPF1) {
            double theta_c = 2.0 * juce::MathConstants<float>::pi * frequency / sampleRate;
            double gamma = cos(theta_c) / (1.0 + sin(theta_c));
            double alpha = (1 + gamma) / 2;
            coeff0 = alpha;
            coeff1 = -alpha;
            coeff2 = 0;
            coeff3 = -gamma;
            coeff4 = 0;
        } else if (type == FilterType::APF) {
            double alpha = (tan(juce::MathConstants<float>::pi * frequency / sampleRate) - 1) / (tan(juce::MathConstants<float>::pi * frequency / sampleRate) + 1);
            coeff0 = alpha;
            coeff1 = 1.0;
            coeff2 = 0.0;
            coeff3 = alpha;
            coeff4 = 0.0;
        }
    }

    void reset() {
        coeff0 = 1.0f;
        coeff1 = 0.0f;
        coeff2 = 0.0f;
        coeff3 = 0.0f;
        coeff4 = 0.0f;
        state1 = 0.0f;
        state2 = 0.0f;
        stateMemory1 = 0.0f;
        stateMemory2 = 0.0f;
    }

    void switchState() {
        auto temp1 = state1;
        auto temp2 = state2;
        state1 = stateMemory1;
        state2 = stateMemory2;
        stateMemory1 = temp1;
        stateMemory2 = temp2;
    }

private:
    float coeff0;
    float coeff1;
    float coeff2;
    float coeff3;
    float coeff4;
    float state1;
    float state2;

    float stateMemory1;
    float stateMemory2;
};
