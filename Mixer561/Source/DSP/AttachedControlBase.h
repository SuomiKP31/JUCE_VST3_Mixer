/*
  ==============================================================================

    AttachedControlBase.h
    Created: 31 Mar 2021 10:08:32pm
    Author:  migizo
    Refined and re-commented(in English) by RoarlisF/KP31

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;

struct AttachedControlBase : public AudioProcessorValueTreeState::Listener,
    public AsyncUpdater
{
    AttachedControlBase(AudioProcessorValueTreeState& s, const String& p)
            : state(s), paramID(p), lastValue(0)
    {
        state.addParameterListener(paramID, this);
    }

    virtual void removeListener()
    {
        state.removeParameterListener(paramID, this);
        paramID = String();
    }

    virtual void replaceListener(String newParamID)
    {
        state.removeParameterListener(paramID, this);
        paramID = newParamID;
        state.addParameterListener(paramID, this);
        sendInitialUpdate();
    }

    AudioProcessorValueTreeState::Parameter* getParameter() {
        return (AudioProcessorValueTreeState::Parameter*)state.getParameter(paramID);
    }

    void setNewUnnormalisedValue(float newUnnormalisedValue)
    {
        if (AudioProcessorParameter* p = state.getParameter(paramID))
        {
            const float newValue = state.getParameterRange(paramID)
                .convertTo0to1(newUnnormalisedValue);

            if (p->getValue() != newValue)
                p->setValueNotifyingHost(newValue);
        }
    }

    float getUnnormalisedValue()
    {
        RangedAudioParameter* p = state.getParameter(paramID);
        if (p) return p->getValue();
        
        // Parameter with given paramID does not exist!
        jassert(false);
        return 0;
    }


    void sendInitialUpdate()
    {
        float* v = (float *)state.getRawParameterValue(paramID);
        if (v) parameterChanged(paramID, *v);
    }

    void parameterChanged(const String&, float newValue) override
    {
        lastValue = newValue;

        if (MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            setValue(newValue);
        }
        else
        {
            triggerAsyncUpdate();
        }
    }

    void beginParameterChange()
    {
        if (AudioProcessorParameter* p = state.getParameter(paramID))
            p->beginChangeGesture();
    }

    void endParameterChange()
    {
        if (AudioProcessorParameter* p = state.getParameter(paramID))
            p->endChangeGesture();
    }

    void handleAsyncUpdate() override
    {
        setValue(lastValue);
    }

    virtual void setValue(float) = 0;

    AudioProcessorValueTreeState& state;
    String paramID;
    float lastValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AttachedControlBase)
};