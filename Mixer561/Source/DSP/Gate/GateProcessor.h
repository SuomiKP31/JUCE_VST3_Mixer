/*
  ==============================================================================

    GateProcessor.h
    Created: 5 Nov 2023 5:19:55pm
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Gate.h"

//==============================================================================
/*
*/
class GateProcessor
{
public:
	GateProcessor();
	~GateProcessor();

	void resizeGateProcessorByChannelNum(int channel);
	void process(juce::AudioBuffer<float>& buffer, double sampleRate);

	void setInterval(int denominator);
	void setBpm(double b);
private:
	std::vector<Gate> gate;
	double bpm = 120;
	int rpInterval = 0;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GateProcessor)
};
