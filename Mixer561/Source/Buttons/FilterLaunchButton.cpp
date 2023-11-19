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
    // Send action bind to this button to component, and let it interact with processor.
}

void FilterLaunchButton::DefineAction(GUISlamControl::SlamActionType actionType, GUISlamControl::SlamControlType controlType)
{
    action = actionType;
    control = controlType;
}
