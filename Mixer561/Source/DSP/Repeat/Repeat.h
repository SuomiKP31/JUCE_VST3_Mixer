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

// Templates should be implemented in headers!
template <typename T>
void Repeat::process(T& val) {
    // write buffer. The input val here is typically one sample in the streaming audio buffer
    if (enablePush) {
        // if the buffer is not full, push it in buffer
        if (buffer.size() < maxBufferSize) {
            buffer.push_back(val);
        }
        // otherwise we stop pushing
        if (buffer.size() == maxBufferSize) enablePush = false;
    }

    if (interval != 0) {
        // read the buffer and write buffered value to val, thus we play some previous samples repeatedly
        int divSize = (double)bufferSize / (double)interval;  // how many samples in each repeat interval
        int divCount = count % divSize;  // count how many samples passed in the outer loop (in RepeatProcessor.h) and get an index
        const int fadeSize = 100;
        val = buffer[divCount];  // write to the streaming buffer

        // Fade in, at the head of each repeated interval
        if (divCount < fadeSize) {
            val *= juce::jmap((double)(divCount), 0.0, (double)(fadeSize - 1), 0.0, 1.0);
        }
        // Fade out at tail, similarly
        else if (divCount > (divSize - fadeSize)) {
            val *= juce::jmap((double)(divCount), (double)(divSize - fadeSize), (double)(divSize - 1), 1.0, 0.0);
        }
    }

    // Handle fade-out in the process of changing the interval parameter (see setInterval function)
    // see AnimatorByCount.h
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