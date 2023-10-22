/*
  ==============================================================================

    TimeDomainProcessingComponent.cpp
    Created: 14 Mar 2022 11:33:25am
    Author:  Roarlisf/KP31

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TimeDomainProcessingComponent.h"


double LabeledTextInputField::getNumber()
{
    auto s = getText().toStdString();
    if(s.length()==0)
    {
        return 120;
    }
    return std::stof(s);
}

//-----------------TriggerButton

void TriggerButton::OnStateChanged()
{
	TriggerAction action;
	action = JudgeButtonAction(getState());
	TakeAction(action);

}


TriggerButton::TriggerAction TriggerButton::JudgeButtonAction(ButtonState currentState)
{
	// DBG(juce::String::formatted("cache %d state %d", cachedState, currentState));
	if (cachedState == buttonNormal)
	{
		cachedState = currentState;
		if (currentState == buttonOver)
		{
			return None;
		}
		if (currentState == buttonDown)
		{
			return Activate;
		}
		return None;
	}
	if (cachedState == buttonOver)
	{
		cachedState = currentState;
		if (currentState == buttonNormal)
		{
			return None;
		}
		if (currentState == buttonDown)
		{
			return Activate;
		}
		return None;
	}
	if (cachedState == buttonDown)
	{
		cachedState = currentState;
		if (currentState == buttonDown)
		{
			return None;
		}

		return Deactivate;
	}
	return None;
}

// TODO: Trigger button actions

TimeDomainProcessingComponent::TimeDomainProcessingComponent(Mixer561AudioProcessor&p) : audioProcessor(p)
{

    bpmInput.setInputFilter(&bpmInput.lacr, false);

  
    for(auto* comp : getComponent())
    {
        addAndMakeVisible(comp);
    }

	initButtons();

	setShortcuts();
}

TimeDomainProcessingComponent::~TimeDomainProcessingComponent()
{


}

void TimeDomainProcessingComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void TimeDomainProcessingComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto bound = getLocalBounds();
	auto bpmBound = bound.removeFromLeft(bound.getWidth() * 0.33);
	bpmInput.label = "BPM";
    bpmInput.labelComp.setBounds(bpmBound.removeFromTop(bpmBound.getHeight() * 0.33));
    bpmInput.labelComp.setText(bpmInput.label, juce::dontSendNotification);
    bpmInput.setBounds(bpmBound);

	// TODO: Button GUI
}


std::vector<juce::Component*> TimeDomainProcessingComponent::getComponent()
{
    return {
        &bpmInput,
        &bpmInput.labelComp,
    };
}

void TimeDomainProcessingComponent::setShortcuts()
{
	
}

void TimeDomainProcessingComponent::initButtons()
{
	// Init buttons and bind shortcuts here
}
