/*
  ==============================================================================

    Repeat.cpp
    Created: 5 Nov 2023 3:45:31pm
    Author:  KP31

  ==============================================================================
*/

#include "Repeat.h"

void Repeat::setInterval(int _interval) {
    // Interval is the how long we want to repeat the samples, typically expressed by note length
        // e.g. 8th, 12th, 16th, etc.
        // if we pass 0 to it, that means this effect is bypassed
    if (preserveInterval != _interval) {
        if (this->interval == 0) {
            clear();
            enablePush = true;  // starts to get samples once the interval changes from 0 to something else
        }
        preserveInterval = _interval;


        // fade animator is started when interval value changes
        animator.start(100, 1.0, 0.0);
    }
}

void Repeat::setParameter(double sampleRate, double bpm) {

        // set the buffer parameter when bpm changes
        maxBufferSize = sampleRate * 60.0 / 1.0/*bpm*/ * 4.0; // bpm=1
        bufferSize = sampleRate * (60.0 / bpm * 4.0); // the sample count of a 4-quarter note measure

        buffer.reserve(2 * maxBufferSize); // We want to prevent re-allocation in audio thread
        // Still... using std::vector is a pretty bad idea and this fx will occasionally fail.
        // You should really use AudioSampleBuffer provided by juce if you want to recreate this effect.
    }


void Repeat::clear()
{
    count = 0;
    buffer.clear();
}