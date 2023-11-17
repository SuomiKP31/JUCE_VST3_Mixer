/*
  ==============================================================================

    LaunchpadComponent.h
    Created: 17 Nov 2023 11:43:21am
    Author:  fangz

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class LaunchpadComponent : public juce::Component {
public:
    LaunchpadComponent(Mixer561AudioProcessor&);
    ~LaunchpadComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    Mixer561AudioProcessor& audioProcessor;

private:
    std::vector<juce::Component*> getComponent();
    void setShortcuts();
    void initButtons();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LaunchpadComponent)
};