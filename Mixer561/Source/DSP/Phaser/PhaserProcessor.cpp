/*
  ==============================================================================

    PhaserProcessor.cpp
	Created: 6 Nov 2023 2:32:15pm
    Author:  KP31

  ==============================================================================
*/

#include "PhaserProcessor.h"

PhaserProcessor::PhaserProcessor()
{
}

PhaserProcessor::~PhaserProcessor()
{
}

void PhaserProcessor::process(juce::AudioBuffer<float>& buffer, double sampleRate)
{
	if(bypass)
	{
		return;
	}
	int numChannel = buffer.getNumChannels();
	int numSamples = buffer.getNumSamples();

	float phase;
	float phaseMain;
	unsigned int sampleCount;

	

	for (int channel = 0; channel < numChannel; ++channel) {
		float* channelData = buffer.getWritePointer(channel);
		sampleCount = sampleCountToUpdateFilters;
		phase = lfoPhase;

		for (int sample = 0; sample < numSamples; ++sample) {
			float in = channelData[sample];

			float centreFrequency = LFO::lfo(phase, SineWave);
			centreFrequency *= sweepWidth;
			centreFrequency += lfoFrequency;

			phase += lfoFrequency * inverseSampleRate;  // LFO advance Time
			if (phase >= 1.0f)
				phase -= 1.0f;

			if (sampleCount++ % updateFiltersInterval == 0)
				updateFilters(centreFrequency);

			// Run through cascade all-pass filters with an oscillating freq
			float filtered = in + feedback * filteredOutputs[channel];
			for (int i = 0; i < numFiltersPerChannel; ++i)
				filtered = filters[channel *  numFiltersPerChannel + i]->processSingleSampleRaw(filtered);

			filteredOutputs.set(channel, filtered);
			float out = in + depth * (filtered - in) * 0.5f;
			channelData[sample] = out;
		}

		if (channel == 0)
			phaseMain = phase;
	}

	sampleCountToUpdateFilters = sampleCount;
	lfoPhase = phaseMain;
}


void PhaserProcessor::setupFilters(int channel, double sampleRate)
{
	twoPi = juce::MathConstants<double>::twoPi;
	lfoPhase = 0.0f;
	inverseSampleRate = 1.0f / (float)sampleRate;

	sampleCountToUpdateFilters = 0;
	updateFiltersInterval = 32;

	filters.clear();
	for (int i = 0; i < channel * numFiltersPerChannel; ++i) {
		Filter* filter;
		filters.add(filter = new Filter());
	}

	for (int i = 0; i < channel; ++i)
		filteredOutputs.add(0.0f);
}

void PhaserProcessor::updateFilters(double centerFrequency)
{
	double discreteFrequency = twoPi * centerFrequency * inverseSampleRate;

	for (int i = 0; i < filters.size(); ++i)
		filters[i]->updateCoefficients(discreteFrequency);
}
