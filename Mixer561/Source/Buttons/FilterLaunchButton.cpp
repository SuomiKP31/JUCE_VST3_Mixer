/*
  ==============================================================================

    FilterLaunchButton.cpp
    Created: 16 Nov 2023 9:51:39pm
    Author:  KP31

  ==============================================================================
*/

#include "FilterLaunchButton.h"

void FilterLaunchButton::TakeAction(TriggerAction& action)
{
	switch (action)
	{
		case Activate: {
			ApplySlam();
			break;
		}
		case Deactivate: {
			break;
		}
		case None: break;
		default: break;
	}
}

void FilterLaunchButton::DefineAction(GUISlamControl::SlamActionType actionType, GUISlamControl::SlamControlType controlType)
{
    action = actionType;
    control = controlType;
}

void FilterLaunchButton::ApplySlam()
{
	bool playSlam = true;
	playSlam = audioProcessor->ProcessSlam(action, control);
	if (control == GUISlamControl::SlamControlType::All)
	{
		playSlam = false;
	}
	if (playSlam) {
		audioProcessor->PlayNextSlam();
	}
	
}
