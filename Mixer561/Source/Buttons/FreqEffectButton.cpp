/*
  ==============================================================================

    FreqEffectButton.cpp
    Created: 7 Nov 2023 9:31:53pm
    Author:  fangz

  ==============================================================================
*/

#include "FreqEffectButton.h"

void FreqEffectButton::TakeAction(TriggerAction& action)
{
	switch (action)
	{
	case Activate: {
		processor->setBypass(false);
		break;
	}
	case Deactivate: {
		processor->setBypass(true);
		break;
	}
	case None: break;
	default: break;
	}
}
