/*
  ==============================================================================

    RepeatProcessor.cpp
    Created: 5 Nov 2023 3:56:14pm
    Author:  KP31

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RepeatProcessor.h"

//==============================================================================
RepeatProcessor::RepeatProcessor()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

RepeatProcessor::~RepeatProcessor()
{
}

void RepeatProcessor::resizeRepeatProcessorByChannelNum(int channel)
{
    if (repeat.size() != channel) {
        repeat.resize(channel);
        for (int i = 0; i < repeat.size(); i++) {
            repeat[i].clear();
        }
    }
}

void RepeatProcessor::process(juce::AudioBuffer<float>& buffer, double sampleRate)
{
    auto bufferSize = buffer.getNumSamples();
    for (int channel = 0; channel < repeat.size(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        repeat[channel].setParameter(sampleRate, bpm);

        for (int i = 0; i < bufferSize; i++) {
            repeat[channel].process(channelData[i]);

        }
    }
}

void RepeatProcessor::setInterval(int denominator)
{
    
    for (int i = 0; i < repeat.size(); i++) {
        if (denominator == 0) repeat[i].setInterval(0);
        else repeat[i].setInterval(denominator);

    }
    //DBG(juce::String::formatted("set denominator from %d to %d",rpInterval, denominator));
    rpInterval = denominator;
}

void RepeatProcessor::setBpm(double b)
{
    bpm = b;
}


