/*
  ==============================================================================

    BitCrusherProcessor.cpp
	Created: 7 Nov 2023 1:23:12am
    Author:  KP31

  ==============================================================================
*/

#include "BitCrusherProcessor.h"

void BitCrusherProcessor::process(juce::AudioBuffer<float>& buffer, double sampleRate)
{
	if (bypass)
	{
		return;
	}
	auto numInputChannels = buffer.getNumChannels();
	auto numSamples = buffer.getNumSamples();
	
	for (int channel = 0; channel < numInputChannels; ++channel) {
		float* channelData = buffer.getWritePointer(channel);
		
		for (int sample = 0; sample < numSamples; ++sample)
		{
			// Quantify
			float totalQLevels = powf(2, bitDepth);
			float val = channelData[sample];
			float remainder = fmodf(val, 1 / totalQLevels);
			// This may seems confusing but we're just mapping audio signals by steps really
			// each step is 1/totalQLevels
			// mapped audio signals have reduced bit depth so they'll sound distorted
			channelData[sample] -= remainder;

			// Downsample
			if(rateDivide > 1)
			{
				int preserveNeeded = sample % rateDivide;  //The remainder of index to the last sample we want to preserve
				// Preserve one of several sample, replace the others, so that we artificially lower the sample rate
				if(preserveNeeded != 0)
				{
					channelData[sample] = channelData[sample - preserveNeeded];
				}
			}
		}
	}

}


BitCrusherProcessor::BitCrusherProcessor(int bitDepth, int rateDivide)
	: bitDepth(bitDepth), rateDivide(rateDivide)
{
	// Member Initializer List
}
