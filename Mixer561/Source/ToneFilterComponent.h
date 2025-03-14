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


class ToneFilterComponent : public juce::Component
{
public:
    ToneFilterComponent(Mixer561AudioProcessor&);
    ~ToneFilterComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    Mixer561AudioProcessor& audioProcessor;

    //LabeledTextInputField bpmInput;
    juce::Slider mixingStrength;
    juce::ToggleButton bypass;

    float* toneRMS; // Just pass the array here...

    // There are two thresholds. The maximum energy we allow (we'll likely apply a fixed gain to the info we get)
    // And the mute energy which, if not reached by a tone, we ignore it. These will be set by sliders and applied in the processor
    float muteThreshold;
    float clampThreshold;


private:
    std::vector<juce::Component*> getComponent();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToneFilterComponent)
};