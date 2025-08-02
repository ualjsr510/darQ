/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleEQAudioProcessorEditor::SimpleEQAudioProcessorEditor(SimpleEQAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p),
	peakFreqSliderAttachment(audioProcessor.apvts, "Peak Freq", peakFreqSlider),
	spectrumAnalyzer(audioProcessor),
	peakGainSliderAttachment(audioProcessor.apvts, "Peak Gain", peakGainSlider),
	peakQualitySliderAttachment(audioProcessor.apvts, "Peak Quality", peakQualitySlider),
	lowCutFreqSliderAttachment(audioProcessor.apvts, "LowCut Freq", lowCutFreqSlider),
	highCutFreqSliderAttachment(audioProcessor.apvts, "HighCut Freq", highCutFreqSlider),
	lowCutSlopeSliderAttachment(audioProcessor.apvts, "LowCut Slope", lowCutSlopeSlider),
	highCutSlopeSliderAttachment(audioProcessor.apvts, "HighCut Slope", highCutSlopeSlider)
{

	addAndMakeVisible(spectrumAnalyzer);


	for (auto* comp : getComps())
	{
		addAndMakeVisible(comp);

		if (auto* s = dynamic_cast<juce::Slider*>(comp))
		{
			s->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
			s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
			s->setRotaryParameters(
				juce::MathConstants<float>::pi * 0.75f,  
				juce::MathConstants<float>::pi * 2.25f,
				true);
		}
	}

	// Aplica el LookAndFeel personalizado
	peakFreqSlider.setLookAndFeel(&customLookAndFeel);
	peakGainSlider.setLookAndFeel(&customLookAndFeel);
	peakQualitySlider.setLookAndFeel(&customLookAndFeel);
	lowCutFreqSlider.setLookAndFeel(&customLookAndFeel);
	highCutFreqSlider.setLookAndFeel(&customLookAndFeel);
	lowCutSlopeSlider.setLookAndFeel(&customLookAndFeel);
	highCutSlopeSlider.setLookAndFeel(&customLookAndFeel);

	peakFreqSlider.setName("Peak Freq");
	peakGainSlider.setName("Gain");
	peakQualitySlider.setName("Q");
	lowCutFreqSlider.setName("Low Cut Freq");
	lowCutSlopeSlider.setName("Low Slope");
	highCutFreqSlider.setName("High Cut Freq");
	highCutSlopeSlider.setName("High Slope");

	//backgroundImage = juce::ImageCache::getFromMemory(BinaryData::bg_png, BinaryData::bg_pngSize);

	setResizable(true, true);
	setResizeLimits(300, 200, 1200, 800);
	setSize(900, 600); // tamaño inicial
	centreWithSize(getWidth(), getHeight());

}

SimpleEQAudioProcessorEditor::~SimpleEQAudioProcessorEditor()
{
	// Limpia el LookAndFeel para evitar problemas al destruir
	peakFreqSlider.setLookAndFeel(nullptr);
	peakGainSlider.setLookAndFeel(nullptr);
	peakQualitySlider.setLookAndFeel(nullptr);
	lowCutFreqSlider.setLookAndFeel(nullptr);
	highCutFreqSlider.setLookAndFeel(nullptr);
	lowCutSlopeSlider.setLookAndFeel(nullptr);
	highCutSlopeSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void SimpleEQAudioProcessorEditor::paint(juce::Graphics& g)
{
	//g.drawImage(backgroundImage, getLocalBounds().toFloat());
	g.fillAll(juce::Colours::black);
}

void SimpleEQAudioProcessorEditor::resized()
{
	auto bounds = getLocalBounds().reduced(20); // margen general

	// Dividir en tres columnas iguales: izquierda, centro, derecha
	auto columnWidth = bounds.getWidth() / 3;

	auto lowCutArea = bounds.removeFromLeft(columnWidth).reduced(10);
	auto highCutArea = bounds.removeFromRight(columnWidth).reduced(10);
	auto peakArea = bounds.reduced(10); // lo que queda es la zona central

	// Distribución vertical: dividir cada zona en 2 (Low/HighCut) o 3 (Peak)
	auto lowCutHalf = lowCutArea.getHeight() / 2;
	lowCutFreqSlider.setBounds(lowCutArea.removeFromTop(lowCutHalf).reduced(0, 5));
	lowCutSlopeSlider.setBounds(lowCutArea.reduced(0, 5));

	auto highCutHalf = highCutArea.getHeight() / 2;
	highCutFreqSlider.setBounds(highCutArea.removeFromTop(highCutHalf).reduced(0, 5));
	highCutSlopeSlider.setBounds(highCutArea.reduced(0, 5));

	auto peakThird = peakArea.getHeight() / 3;
	peakFreqSlider.setBounds(peakArea.removeFromTop(peakThird).reduced(0, 5));
	peakGainSlider.setBounds(peakArea.removeFromTop(peakThird).reduced(0, 5));
	peakQualitySlider.setBounds(peakArea.reduced(0, 5));

	spectrumAnalyzer.setBounds(getLocalBounds());

}




std::vector<juce::Component*> SimpleEQAudioProcessorEditor::getComps()
{
	return
	{
		&peakFreqSlider,
		&peakGainSlider,
		&peakQualitySlider,
		&lowCutFreqSlider,
		&highCutFreqSlider,
		&lowCutSlopeSlider,
		&highCutSlopeSlider
	};
}
