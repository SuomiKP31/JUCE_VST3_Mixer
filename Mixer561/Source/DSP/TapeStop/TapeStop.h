/*
  ==============================================================================

    TapeStop.h
	Created: 7 Nov 2023 8:52:11am
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TapeStop
{
public:
	

	int passedSample = 0; // How many samples passed from the start
	int sampleLingerLength = 6000;  //  How many samples we want this effect to last before muting the output
	float sampleIdx = 0.f;

	template <typename T>
	void process(T & val, float* bufferedData)
	{
		float sampleIncrementRate = 1.f - static_cast<float>(passedSample) / static_cast<float>(sampleLingerLength);
		if (sampleIncrementRate <= 0) // Momentum depleted
		{
			val = 0.f; // Mute
			return;
		}
		bufferedData[passedSample] = val;

		int slowedSampleIdx = static_cast<int>(floorf(sampleIdx));
		val = bufferedData[slowedSampleIdx];

		sampleIdx += sampleIncrementRate;
		passedSample += 1;
	}

	inline void resetParam(int lingerLength)
	{
		passedSample = 0;
		sampleIdx = 0.f;
		sampleLingerLength = lingerLength;
	}
};