/*
  ==============================================================================

    GateProcessor.cpp
    Created: 5 Nov 2023 5:19:55pm
    Author:  KP31

  ==============================================================================
*/

#include "GateProcessor.h"

GateProcessor::GateProcessor()
{
	
}

GateProcessor::~GateProcessor()
{
}

void GateProcessor::resizeGateProcessorByChannelNum(int channel)
{
	if (gate.size() != channel) {
		gate.resize(channel);
	}
}

void GateProcessor::process(juce::AudioBuffer<float>& buffer, double sampleRate)
{
	auto bufferSize = buffer.getNumSamples();
	for (int channel = 0; channel < gate.size(); ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		gate[channel].setParameter(sampleRate, bpm);

		for (int i = 0; i < bufferSize; i++) {
			gate[channel].process(channelData[i]);

		}
	}
}

void GateProcessor::setInterval(int denominator)
{
	for (int i = 0; i < gate.size(); i++) {
		if (denominator == 0) gate[i].setInterval(0);
		else gate[i].setInterval(denominator);

	}
	//DBG(juce::String::formatted("set denominator from %d to %d",rpInterval, denominator));
	rpInterval = denominator;
}

void GateProcessor::setBpm(double b)
{
	bpm = b;
}
