/*
  ==============================================================================

    FilterLaunchButton.h
    Created: 16 Nov 2023 9:51:39pm
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include "../BaseGUIComp.h"
#include "../PluginEditor.h"

// This button is a launchpad button that create a sudden change in freq filter and plays a slam sound
class FilterLaunchButton : public TriggerButton {
public:
    virtual void TakeAction(TriggerAction& action) override;
    void PlaySlam();
protected:
    double freqBand[];

};