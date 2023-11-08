/*
  ==============================================================================

    TapeStopProcessor.h
	Created: 7 Nov 2023 9:59:53am
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include "../FreqEffectProcessor.h"
#include "../TapeStop/TapeStop.h"

class TapeStopProcessor : public FreqEffectProcessor
{
public:
	void process(juce::AudioBuffer<float>& buffer, double sampleRate) override;
	void setBypass(bool bypassFlag)override;
	void setupTapeBuffer(int channel, double sampleRate);

	juce::AudioSampleBuffer tapeBuffer;
	bool bypass = true;

	float sampleIdx = 0.f; // use this float to simulate a momentum attenuation of the tape
	double sampleRate = 48000;

	int sampleLingerLength = 6000;  //  How many samples we want this effect to last before muting the output
	float stopTime = 0.3f;

	std::vector<TapeStop> tapeMono;
};