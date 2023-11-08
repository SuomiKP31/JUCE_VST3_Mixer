/*
  ==============================================================================

    TimeDomainProcessingComponent.h
    Created: 14 Mar 2022 11:33:25am
    Author:  Roarlisf/KP31

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Buttons/GateButton.h"
#include "Buttons/RepeatButton.h"
#include "Buttons/FreqEffectButton.h"

//==============================================================================
/*
*/


class TimeDomainProcessingComponent  : public juce::Component
{
public:
    TimeDomainProcessingComponent(Mixer561AudioProcessor&);
    ~TimeDomainProcessingComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    Mixer561AudioProcessor& audioProcessor;

    LabeledTextInputField bpmInput;
    
    // TODO: Init trigger buttons
    RepeatButton re8, re12, re16, re32;
    GateButton ga8, ga16, ga24, ga32;
    FreqEffectButton fl1, ph1, bc1, tp1;

private:
    std::vector<juce::Component*> getComponent();
	void setShortcuts();
	void initButtons();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeDomainProcessingComponent)
};


