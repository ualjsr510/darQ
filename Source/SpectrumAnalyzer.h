#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SpectrumAnalyzer : public juce::Component,
    private juce::Timer
{
public:
    SpectrumAnalyzer(SimpleEQAudioProcessor&);
    ~SpectrumAnalyzer() override = default;

    void paint(juce::Graphics&) override;
    void resized() override {}
    void timerCallback() override;

private:
    void drawSpectrum(juce::Graphics&);

    SimpleEQAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzer)
};
