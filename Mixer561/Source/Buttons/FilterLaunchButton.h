/*
  ==============================================================================

    FilterLaunchButton.h
    Created: 16 Nov 2023 9:51:39pm
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include "../BaseGUIComp.h"
#include "../PluginProcessor.h"

// GUI Action Definition
namespace GUISlamControl {
    enum SlamControlType
    {
        LPF,
        Peak,
        HPF,
        All
    };
    enum SlamActionType
    {
        Add,
        Substract,
        Reset
    };
}


// This button is a launchpad button that create a sudden change in freq filter and plays a slam sound
class FilterLaunchButton : public TriggerButton {
public:
    virtual void TakeAction(TriggerAction& action) override;
    virtual void DefineAction(GUISlamControl::SlamActionType actionType, GUISlamControl::SlamControlType controlType);
    virtual void ApplySlam();
    Mixer561AudioProcessor* audioProcessor;

private:
    GUISlamControl::SlamControlType control;
    GUISlamControl::SlamActionType action;
};