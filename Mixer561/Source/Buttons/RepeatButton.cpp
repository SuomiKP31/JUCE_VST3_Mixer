/*
  ==============================================================================

    RepeatButton.cpp
    Created: 5 Nov 2023 4:46:48pm
    Author:  KP31

  ==============================================================================
*/

#include "RepeatButton.h"

void RepeatButton::TakeAction(TriggerAction& action)
{
	switch (action)
	{
	case Activate: {
		repeatProcessor->setBpm(bpmInput->getNumber());
		//DBG(bpmInput->getNumber());
		repeatProcessor->setInterval(denominator);
		break;
	}
	case Deactivate: {
		repeatProcessor->setInterval(0);
		break;
	}
	case None: break;
	default: break;
	}
}
