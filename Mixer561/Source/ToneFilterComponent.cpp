/*
  ==============================================================================

    ToneFilterComponent.cpp
    Created: 14 Mar 2025 10:48:50am
    Author:  fangz

  ==============================================================================
*/

#include "ToneFilterComponent.h"

ToneFilterComponent::ToneFilterComponent(Mixer561AudioProcessor& p) : audioProcessor(p)
{
    for (auto* comp : getComponent())
    {
        addAndMakeVisible(comp);
    }

    mixingStrengthSld.setRange(0, 1);
    originTrackAttenuationSld.setRange(0, 1);
}

ToneFilterComponent::~ToneFilterComponent()
{
}

void ToneFilterComponent::paint(juce::Graphics& g)
{
    

    auto bound = getLocalBounds();
    auto b1 = bound.removeFromTop(0.15f * bound.getHeight());

    
    //g.drawRect(b1, 1);
    g.setColour(juce::Colours::blueviolet);
    g.fillRect(b1);

    g.setColour(juce::Colours::black);
    g.fillRect(bound);

    g.setColour(juce::Colours::grey);
    g.drawRect(b1, 2.f);
    g.drawRect(bound, 2.f);
    //g.fillAll(juce::Colours::black);   // clear the background
    
}

void ToneFilterComponent::resized()
{
    auto bound = getLocalBounds();
    auto controllerBound = bound.removeFromTop(0.15f * bound.getHeight());

    float controllerStripWidth = controllerBound.getWidth();

    auto toggleBound = controllerBound.removeFromLeft(.2f * controllerStripWidth);
    auto mixStrengthSliderBound = controllerBound.removeFromLeft(0.4f * controllerStripWidth);

    auto text1Bound = mixStrengthSliderBound.removeFromTop(mixStrengthSliderBound.getHeight() * 0.33);
    auto text2Bound = controllerBound.removeFromTop(controllerBound.getHeight() * 0.33);

    bypassToggle.setBounds(toggleBound);
    bypassToggle.setButtonText("ToneFilter Bypass");
    
    mixingStrengthSld.setBounds(mixStrengthSliderBound);
    originTrackAttenuationSld.setBounds(controllerBound);
    
    mixLabel.setText("Harmonic Strength", juce::NotificationType::dontSendNotification);
    mixLabel.setBounds(text1Bound);
    attenuationLabel.setText("Original Track Attenuation", juce::NotificationType::dontSendNotification);
    attenuationLabel.setBounds(text2Bound);
}

std::vector<juce::Component*> ToneFilterComponent::getComponent()
{
    return {
        &mixingStrengthSld,
        &originTrackAttenuationSld,
        &bypassToggle,
        &mixLabel,
        &attenuationLabel
    };
}
