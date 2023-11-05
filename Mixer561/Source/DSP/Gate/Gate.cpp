/*
  ==============================================================================

    Gate.cpp
    Created: 5 Nov 2023 3:30:31pm
    Author:  KP31

  ==============================================================================
*/

#include "Gate.h"

void Gate::setInterval(int _interval)
{
	// Interval is the how long we want to repeat the samples, typically expressed by note length
		// e.g. 8th, 12th, 16th, etc.
		// if we pass 0 to it, that means this effect is bypassed
	if (preserveInterval != _interval) {

		preserveInterval = _interval;


		// fade animator is started when interval value changes
		animator.start(100, 1.0, 0.0);
	}
}

void Gate::setParameter(double sampleRate, double bpm)
{
	// set the buffer parameter when bpm changes

	bufferSize = sampleRate * (60.0 / bpm * 4.0); // the sample count of a 4-quarter note measure
	this->sampleRate = sampleRate;
}

