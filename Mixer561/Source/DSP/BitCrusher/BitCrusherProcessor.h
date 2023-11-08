/*
  ==============================================================================

    BitCrusherProcessor.h
	Created: 7 Nov 2023 1:23:12am
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include "../FreqEffectProcessor.h"

class BitCrusherProcessor : public FreqEffectProcessor
{
public:
	int bitDepth = 8; // Reduce the bit depth to ...
	int rateDivide = 10; // Reduce sample rate by ... times

	void process(juce::AudioBuffer<float>& buffer, double sampleRate) override;

	BitCrusherProcessor() = default;
	BitCrusherProcessor(int bitDepth, int rateDivide);
};
