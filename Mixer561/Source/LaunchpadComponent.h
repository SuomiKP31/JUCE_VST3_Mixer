/*
  ==============================================================================

    LaunchpadComponent.h
    Created: 17 Nov 2023 11:43:21am
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Buttons/FilterLaunchButton.h"

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

    juce::Label Hlabel, Llabel, Plabel;
    FilterLaunchButton ResetAll, PeakAdd, PeakReset, PeakSubstract, LPFAdd, LPFReset, LPFSubtract, HPFAdd, HPFReset, HPFSubtract;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LaunchpadComponent)
};