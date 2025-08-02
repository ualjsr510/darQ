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

    constexpr float silenceThreshold = 0.001f; // Umbral bajo para evitar ruido residual

    for (int i = 1; i < SimpleEQAudioProcessor::scopeSize; ++i)
    {
        // Omitir segmentos si alguno de los extremos está por debajo del umbral
        if (scopeData[i - 1] < silenceThreshold && scopeData[i] < silenceThreshold)
            continue;

        float x1 = juce::jmap((float)(i - 1), 0.0f, (float)(SimpleEQAudioProcessor::scopeSize - 1),
            bounds.getX(), bounds.getRight());
        float y1 = juce::jmap(scopeData[i - 1], 0.0f, 1.0f, bounds.getBottom(), bounds.getY());

        float x2 = juce::jmap((float)i, 0.0f, (float)(SimpleEQAudioProcessor::scopeSize - 1),
            bounds.getX(), bounds.getRight());
        float y2 = juce::jmap(scopeData[i], 0.0f, 1.0f, bounds.getBottom(), bounds.getY());

        float t = (float)i / (SimpleEQAudioProcessor::scopeSize - 1);

        juce::Colour lowFreqColour = juce::Colour(0, 0, 255);     // Azul
        juce::Colour highFreqColour = juce::Colour(255, 0, 0);    // Rojo
        juce::Colour currentColour = lowFreqColour.interpolatedWith(highFreqColour, t);

        g.setColour(currentColour.withAlpha(0.9f));
        g.drawLine(x1, y1, x2, y2, 1.5f);
    }
}



