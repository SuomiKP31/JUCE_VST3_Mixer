/*
  ==============================================================================

    GateButton.cpp
    Created: 5 Nov 2023 4:47:09pm
    Author:  KP31

  ==============================================================================
*/

#include "GateButton.h"

void GateButton::TakeAction(TriggerAction& action)
{
	switch (action)
	{
	case Activate: {
		gateProcessor->setBpm(bpmInput->getNumber());
		//DBG(bpmInput->getNumber());
		gateProcessor->setInterval(denominator);
		break;
	}
	case Deactivate: {
		gateProcessor->setInterval(0);
		break;
	}
	case None: break;
	default: break;
	}
}
