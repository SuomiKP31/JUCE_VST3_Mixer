/*
  ==============================================================================

    GateButton.h
    Created: 5 Nov 2023 4:47:09pm
    Author:  KP31

  ==============================================================================
*/

#pragma once
#include "../BaseGUIComp.h"
#include "../DSP/Gate/GateProcessor.h"

class GateButton : public TriggerButton
{
public:
	int denominator;
	LabeledTextInputField* bpmInput;
	GateProcessor* gateProcessor;

	void TakeAction(TriggerAction& action) override;

};