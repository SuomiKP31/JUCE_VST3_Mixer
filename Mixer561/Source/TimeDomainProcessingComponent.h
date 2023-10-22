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

//==============================================================================
/*
*/


class LabeledTextInputField : public juce::TextEditor
{
public:
    juce::String label;
    juce::Label labelComp;
    LengthAndCharacterRestriction lacr{ 5, "0123456789" };

    double getNumber();
    
};

class TriggerButton : public juce::TextButton
{
public:
	enum TriggerAction
	{
		Activate,
		Deactivate,
		None
	};

	juce::Atomic<bool> activating;

	virtual void OnStateChanged();
	virtual void TakeAction(TriggerAction &action) = 0;
	TriggerAction JudgeButtonAction(ButtonState currentState);

protected:
	ButtonState cachedState = buttonNormal;
};

// TODO: Put trigger buttons here

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


private:
    std::vector<juce::Component*> getComponent();
	void setShortcuts();
	void initButtons();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeDomainProcessingComponent)
};


