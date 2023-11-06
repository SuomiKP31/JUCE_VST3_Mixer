/*
  ==============================================================================

    RepeatProcessor.h
    Created: 5 Nov 2023 3:56:14pm
    Author:  KP31

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Repeat.h"

//==============================================================================
/*
*/
class RepeatProcessor
{
public:
    RepeatProcessor();
    ~RepeatProcessor();

    void resizeRepeatProcessorByChannelNum(int channel);
    void process(juce::AudioBuffer<float>& buffer, double sampleRate);

    void setInterval(int denominator);
    void setBpm(double b);
private:
    std::vector<Repeat> repeat;
    double bpm = 120;
    int rpInterval = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RepeatProcessor)
};
