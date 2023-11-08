/*
  ==============================================================================

    FlangerProcessor.cpp
    Created: 7 Nov 2023 11:31:52am
    Author:  KP31
    https://github.com/juandagilc/Audio-Effects/blob/master/Flanger/Source/PluginProcessor.cpp

  ==============================================================================
*/

#include "FlangerProcessor.h"

FlangerProcessor::FlangerProcessor()
{
}

FlangerProcessor::~FlangerProcessor()
{
}

void FlangerProcessor::process(juce::AudioBuffer<float>& buffer, double sampleRate)
{
	if(bypass)
	{
		return;
	}
	auto numInputChannels = buffer.getNumChannels();
	auto numSamples = buffer.getNumSamples();

	int localWritePosition;
	float phase;
	float phaseMain;

	for (int channel = 0; channel < numInputChannels; ++channel) {
		float* channelData = buffer.getWritePointer(channel);
		float* delayData = delayBuffer.getWritePointer(channel);
		localWritePosition = delayWritePosition;
		phase = lfoPhase;
		/*if (channel != 0)
			phase = fmodf(phase + 0.25f, 1.0f); // Stereo offset the phase of following channels*/

		for (int sample = 0; sample < numSamples; ++sample) {
			const float in = channelData[sample];
			float out = 0.0f;

			float localDelayTime =
				(delayTime + width * LFO::lfo(phase, lfoWaveform)) * sampleRate;

			float readPosition =
				fmodf((float)localWritePosition - localDelayTime + (float)delayBufferSamples, delayBufferSamples);
			int localReadPosition = floorf(readPosition);

			switch (interpolation) {
			case interpolationNearestNeighbour: {
				float closestSample = delayData[localReadPosition % delayBufferSamples];
				out = closestSample;
				break;
			}
			case interpolationLinear: {
				float fraction = readPosition - (float)localReadPosition;
				float delayed0 = delayData[(localReadPosition + 0)];
				float delayed1 = delayData[(localReadPosition + 1) % delayBufferSamples];
				out = delayed0 + fraction * (delayed1 - delayed0);
				break;
			}
			case interpolationCubic: {
				float fraction = readPosition - (float)localReadPosition;
				float fractionSqrt = fraction * fraction;
				float fractionCube = fractionSqrt * fraction;

				float sample0 = delayData[(localReadPosition - 1 + delayBufferSamples) % delayBufferSamples];
				float sample1 = delayData[(localReadPosition + 0)];
				float sample2 = delayData[(localReadPosition + 1) % delayBufferSamples];
				float sample3 = delayData[(localReadPosition + 2) % delayBufferSamples];

				float a0 = -0.5f * sample0 + 1.5f * sample1 - 1.5f * sample2 + 0.5f * sample3;
				float a1 = sample0 - 2.5f * sample1 + 2.0f * sample2 - 0.5f * sample3;
				float a2 = -0.5f * sample0 + 0.5f * sample2;
				float a3 = sample1;
				out = a0 * fractionCube + a1 * fractionSqrt + a2 * fraction + a3;
				break;
			}
			}

			channelData[sample] = in + out * depth;
			delayData[localWritePosition] = in + out * feedback;

			if (++localWritePosition >= delayBufferSamples)
				localWritePosition -= delayBufferSamples;

			phase += lfoFrequency * inverseSampleRate;
			if (phase >= 1.0f)
				phase -= 1.0f;
		}

		if (channel == 0)
			phaseMain = phase;
	}

	delayWritePosition = localWritePosition;
	lfoPhase = phaseMain;

	//======================================

	/*
	for (int channel = numInputChannels; channel < numOutputChannels; ++channel)
		buffer.clear(channel, 0, numSamples);
	*/
	
}

void FlangerProcessor::setBypass(bool bypassFlag)
{
	bypass = bypassFlag;
	
	lfoPhase = 0;
}

void FlangerProcessor::setupDelayBuffer(int channel, double sampleRate)
{
	// Initialize before activate
	this->sampleRate = sampleRate;
	inverseSampleRate = 1 / (float)sampleRate;
	delayBufferSamples = (int)(2 * delayTime *(float)sampleRate) + 1;
	if (delayBufferSamples < 1)
		delayBufferSamples = 1;

	delayBuffer.setSize(channel, delayBufferSamples);
	delayBuffer.clear();
	lfoPhase = 0;

}
