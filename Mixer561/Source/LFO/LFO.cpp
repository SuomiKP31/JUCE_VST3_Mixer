/*
  ==============================================================================

    LFO.cpp
    Created: 5 Nov 2023 11:31:28am
    Author:  KP31

  ==============================================================================
*/

#include "LFO.h"

float LFO::lfo(float phase, LFOWaveform waveform)
{
	float out = 0;
	float twoPi = juce::MathConstants<float>::twoPi;
	switch (waveform)
	{
	case SineWave:
		out = 0.5f + 0.5f * sinf(twoPi * phase); // 0.5(1+sin(2pi*phi) is to map the value to [0,1]
		break;
	case Triangle:
		if (phase < 0.25f)
			out = 0.5f + 2.0f * phase;
		else if (phase < 0.75f)
			out = 1.0f - 2.0f * (phase - 0.25f);
		else
			out = 2.0f * (phase - 0.75f);
		break;
	}
	return out;
}
