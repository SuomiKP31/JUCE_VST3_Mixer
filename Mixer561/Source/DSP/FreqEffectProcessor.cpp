#include "FreqEffectProcessor.h"

/*
  ==============================================================================

    FreqEffectProcessor.cpp
    Created: 6 Nov 2023 8:50:35pm
    Author:  KP31

  ==============================================================================
*/

void FreqEffectProcessor::process(juce::AudioBuffer<float>& buffer, double sampleRate)
{
	// Do nothing
}

void FreqEffectProcessor::setBypass(bool bypassFlag)
{
	bypass = bypassFlag;
}
