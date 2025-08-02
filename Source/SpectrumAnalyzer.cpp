#include "SpectrumAnalyzer.h"

SpectrumAnalyzer::SpectrumAnalyzer(SimpleEQAudioProcessor& p)
    : audioProcessor(p)
{
    startTimerHz(60); 
}

void SpectrumAnalyzer::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.fillAll();

    drawSpectrum(g);
}

void SpectrumAnalyzer::timerCallback()
{
    if (audioProcessor.isFFTReady())
    {
        audioProcessor.drawNextFrameOfSpectrum();
        audioProcessor.setFFTReady(false);
        repaint();
    }
}

void SpectrumAnalyzer::drawSpectrum(juce::Graphics& g)
{
    auto* scopeData = audioProcessor.getScopeData();
    auto bounds = getLocalBounds().toFloat();

    juce::Path spectrumPath;

    for (int i = 0; i < SimpleEQAudioProcessor::scopeSize; ++i)
    {
        auto x = juce::jmap((float)i, 0.0f, (float)(SimpleEQAudioProcessor::scopeSize - 1),
            bounds.getX(), bounds.getRight());

        auto y = juce::jmap(scopeData[i], 0.0f, 1.0f, bounds.getBottom(), bounds.getY());

        if (i == 0)
            spectrumPath.startNewSubPath(x, y);
        else
            spectrumPath.lineTo(x, y);
    }

    g.setColour(juce::Colours::aqua.withAlpha(0.75f));
    g.strokePath(spectrumPath, juce::PathStrokeType(2.0f));
}
