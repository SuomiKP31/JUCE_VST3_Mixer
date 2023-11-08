/*
  ==============================================================================

    FreqEffectButton.h
    Created: 7 Nov 2023 9:31:53pm
    Author:  fangz

  ==============================================================================
*/

#pragma once
#include "../BaseGUIComp.h"
#include "../DSP/FreqEffectProcessor.h"

class FreqEffectButton : public TriggerButton {
public:
    void TakeAction(TriggerAction& action) override;
    FreqEffectProcessor* processor;
};