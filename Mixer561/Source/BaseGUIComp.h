/*
  ==============================================================================

    BaseGUIComp.h
    Created: 5 Nov 2023 5:03:04pm
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

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
	virtual void TakeAction(TriggerAction& action) = 0;
	TriggerAction JudgeButtonAction(ButtonState currentState);

protected:
	ButtonState cachedState = buttonNormal;
};