/*
  ==============================================================================

    Gate.h
    Created: 5 Nov 2023 3:30:31pm
    Author:  KP31

  ==============================================================================
*/
#include <JuceHeader.h>
#include "../AnimatorByCount.h"
#pragma once

class Gate
{
public:
	void setInterval(int _interval);
	void setParameter(double sampleRate, double bpm);

	template <typename T>
	void process(T & val);

	template <typename T>
	void getGateVal(T & v, int & attackSample, int & releaseSample, int & divCount);


private:
	int count = 0;
	AnimatorByCount animator;

	int bufferSize;
	double sampleRate;

	int interval = 0;  // In SDVX the Gate-i FX triggers every i-th note time just like the repeat FX
	int preserveInterval = 0;

	double attackTime = 0.35; // Trigger time the noise gate (percentage of the interval)
	double releaseTime = 0.35; // Release time
	int curDivSize = 0;

	double gating = 0.05; // Minimum value of gating
};

template <typename T>
void Gate::process(T & val)
{
	if (interval != 0)
	{
		int divSize = (double)bufferSize / (double)interval;  // how many samples in each repeat interval
		int divCount = count % divSize;  // count how many samples passed in the outer loop and get an index

		int attackSample = juce::roundToInt(attackTime * divSize);
		int releaseSample = divSize - juce::roundToInt(releaseTime * divSize);
		curDivSize = divSize;
		// DBG(juce::String::formatted("aSample %d, rSample %d", attackSample, releaseSample));
		getGateVal(val, attackSample, releaseSample, divCount);
	}
	

	animator.update();
	if (animator.getIsProgress()) {
		double v = animator.getValue();
		val *= v;
		// fadeout complete -> update interval
		if (v <= 0.0 && animator.getNormalizedValue() >= 1.0) {
			animator.start(100, 0.0, 1.0);
			interval = preserveInterval;
		}
	}

	count++;
}

template <typename T>
void Gate::getGateVal(T & v, int & attackSample, int & releaseSample, int & divCount)
{
	// Map the volume multiplier from 0-1 based on where the sample index(divCount) sits
	T volMod = 0;
	double progress = 0; // Normalized value of progress
	if (divCount < attackSample)
	{
		// Lower volume in the process of attack
		progress = static_cast<double>(attackSample - divCount) / static_cast<double>(attackSample);
		volMod = juce::jmap(progress, gating, 1.0+gating);
	}
	else if (divCount > releaseSample)
	{
		const double rSampleAmount = curDivSize - releaseSample;
		progress = static_cast<double>(divCount - releaseSample) / rSampleAmount;
		volMod = juce::jmap(progress, gating, 1.0+gating);
	}
	else
	{
		volMod = gating;
	}
	v *= volMod;
}

