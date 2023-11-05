/*
  ==============================================================================

    Repeat.h
    Created: 5 Nov 2023 3:45:31pm
    Author:  KP31
    Reference: migizo/Repeat on Github

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../AnimatorByCount.h"
#include <cmath>
class Repeat {
public:
    void setInterval(int _interval);
    
    //------------------------------------------------------------------------
    void setParameter(double sampleRate, double bpm);
    //------------------------------------------------------------------------
    template <typename T>
    void process(T& val);
    
    //------------------------------------------------------------------------
    void clear();
    
    //------------------------------------------------------------------------

private:
    int count = 0;
    std::vector<double> buffer;
    AnimatorByCount animator;
    
    int maxBufferSize;
    int bufferSize;
    bool enablePush = false;
    int interval = 0;
    int preserveInterval = 0;

};
