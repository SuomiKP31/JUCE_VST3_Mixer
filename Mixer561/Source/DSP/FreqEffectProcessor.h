/*
  ==============================================================================

    FreqEffectProcessor.h
    Created: 6 Nov 2023 8:50:35pm
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FreqEffectProcessor
{
	// A Frequency-domain effect does not modify the time axis. i.e. Input Samples = Output Samples
public:
	FreqEffectProcessor() = default;
	virtual void process(juce::AudioBuffer<float>& buffer, double sampleRate);
	virtual void setBypass(bool bypassFlag);

protected:
	bool bypass = true;

	
};

