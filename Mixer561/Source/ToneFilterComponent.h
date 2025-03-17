/*
  ==============================================================================

    ToneFilterComponent.h
    Created: 14 Mar 2025 10:48:50am
    Author:  fangz

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "PluginProcessor.h"


class ToneFilterComponent : public juce::Component, private juce::Timer
{
public:
    ToneFilterComponent(Mixer561AudioProcessor&);
    ~ToneFilterComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    Mixer561AudioProcessor& audioProcessor;

    std::vector<juce::Rectangle<int>> toneBounds;

private:
    std::vector<juce::Component*> getComponent();
    void timerCallback();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToneFilterComponent)
};