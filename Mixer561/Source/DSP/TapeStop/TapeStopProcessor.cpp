/*
  ==============================================================================

    TapeStopProcessor.cpp
    Created: 7 Nov 2023 9:59:53am
    Author:  KP31

  ==============================================================================
*/

#include "TapeStopProcessor.h"

void TapeStopProcessor::setupTapeBuffer(int channel, double sampleRate)
{
	this->sampleRate = sampleRate;
	sampleLingerLength = juce::roundToInt(stopTime * (float)sampleRate);
	tapeBuffer.setSize(channel, 2*sampleLingerLength+1);
	tapeMono.resize(channel);
}

void TapeStopProcessor::setBypass(bool bypassFlag)
{
	bypass = bypassFlag;
	if(!bypass)
	{
		tapeBuffer.clear();
		for (auto &t : tapeMono)
		{
			t.resetParam(sampleLingerLength);
		}
	}
}


void TapeStopProcessor::process(juce::AudioBuffer<float>& buffer, double sampleRate)
{
	if (bypass)
	{
		return;
	}
	auto numInputChannels = buffer.getNumChannels();
	auto numSamples = buffer.getNumSamples();

	for (int channel = 0; channel < numInputChannels; ++channel) {
		float* channelData = buffer.getWritePointer(channel);
		float* bufferedData = tapeBuffer.getWritePointer(channel);

		//passedSample = 0;
		// Separate channels... we need two counter for both channels
		for (int sample = 0; sample < numSamples; ++sample)
		{
			tapeMono[channel].process(channelData[sample], bufferedData);
			
		}

	}
}

