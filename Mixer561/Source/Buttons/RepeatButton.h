/*
  ==============================================================================

    RepeatButton.h
    Created: 5 Nov 2023 4:46:48pm
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include "../BaseGUIComp.h"
#include "../DSP/Repeat/RepeatProcessor.h"

class RepeatButton : public TriggerButton
{
public:
    int denominator;
    LabeledTextInputField* bpmInput;
    RepeatProcessor* repeatProcessor;

    void TakeAction(TriggerAction& action) override;

};