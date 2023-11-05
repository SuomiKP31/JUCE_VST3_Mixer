/*
  ==============================================================================

    AnimatorByCount.h
    Created: 12 Mar 2021 10:33:15pm
    Author:  migizo
	Refined and re-commented(in English) by RoarlisF/KP31

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AnimatorByCount
{
public:
//    << how to use >>
//    animator->update();
//    if (animator->getIsProgress())
//    {
//        // get the output value and multiply it by your sample
//    }
    void start(const int requiredCount, float startValue, float endValue)
    {
		// Fires the fading process
        count = 0;
        isProgress = true;
        isInitialized = true;
        this->requiredCount = requiredCount;
        this->startValue = startValue;
        this->endValue = endValue;
        value = startValue;
    }
    void update()
    {
		// This should be called for each sample
		// It increments count by one and output a linear interpolated value between count and requiredCount,
		// remapped to startValue and endValue
		// you can use this value to fade in/out your sample volume
        if (count >= requiredCount) isProgress = false;

        if (! isProgress)return;
        int countMax = requiredCount - 1;
        normalizedValue = (float)juce::jmin(count, countMax) / (float)countMax;
        value = juce::jmap(normalizedValue, 0.0, 1.0, startValue, endValue);
        count++;
    }
    
    // getter
    bool getIsProgress(){return isProgress;}
    bool getIsInitialized(){return isInitialized;}
    float getValue() {return value;}
    float getNormalizedValue() {return normalizedValue;}
    int getRequiredCount() {return requiredCount;}
private:
    int count = 0;
    int requiredCount;
    double startValue = 0.0;
    double endValue = 0.0;
    float value;
    double normalizedValue = 0;
    bool isProgress = false;
    bool isInitialized = false;
};