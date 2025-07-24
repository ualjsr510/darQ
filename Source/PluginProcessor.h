/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct ChainSettings
{
	float lowCutFreq{ 0 }, highCutFreq{ 0 };
	float peakFreq{ 0 }, peakGainInDecibels{ 0 }, peakQuality{ 0 };
	float lowCutSlope{ 0 }, highCutSlope{ 0 };
};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvst) {
	ChainSettings settings;
	settings.lowCutFreq = apvst.getRawParameterValue("LowCut Freq")->load();
	settings.highCutFreq = apvst.getRawParameterValue("HighCut Freq")->load();
	settings.peakFreq = apvst.getRawParameterValue("Peak Freq")->load();
	settings.peakGainInDecibels = apvst.getRawParameterValue("Peak Gain")->load();
	settings.peakQuality = apvst.getRawParameterValue("Peak Quality")->load();
	settings.lowCutSlope = apvst.getRawParameterValue("LowCut Slope")->load();
	settings.highCutSlope = apvst.getRawParameterValue("HighCut Slope")->load();
	return settings;
} 

//==============================================================================
/**
*/
class SimpleEQAudioProcessor : public juce::AudioProcessor
{
public:
	//==============================================================================
	SimpleEQAudioProcessor();
	~SimpleEQAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
	juce::AudioProcessorValueTreeState apvst{ *this, nullptr, "Parameters", createParameterLayout() };

private:
	using Filter = juce::dsp::IIR::Filter<float>;

	using CutFilter = juce::dsp::ProcessorChain <Filter, Filter, Filter, Filter>;

	using MonoChain = juce::dsp::ProcessorChain <CutFilter,Filter,CutFilter>;

	MonoChain leftChain, rightChain;

	enum ChainPositions
	{
		LowCut,
		Peak,
		HighCut
	};

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleEQAudioProcessor)
};
