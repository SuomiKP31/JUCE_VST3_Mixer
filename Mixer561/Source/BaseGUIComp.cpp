/*
  ==============================================================================

    BaseGUIComp.cpp
    Created: 5 Nov 2023 5:03:04pm
    Author:  KP31

  ==============================================================================
*/

#include "BaseGUIComp.h"

double LabeledTextInputField::getNumber()
{
	auto s = getText().toStdString();
	if (s.length() == 0)
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
	//DBG(juce::String::formatted("cache %d state %d", cachedState, currentState));
	if (cachedState == buttonNormal)
	{
		cachedState = currentState;
		if (currentState == buttonOver)
		{
			return None;
		}
		if (currentState == buttonDown)
		{
			//DBG("Activate");
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
			//DBG("Activate");
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
		//DBG("Deactivate");
		return Deactivate;
		
	}
	return None;
}