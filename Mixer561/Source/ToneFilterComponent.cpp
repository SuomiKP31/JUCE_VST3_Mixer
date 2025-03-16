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
}

ToneFilterComponent::~ToneFilterComponent()
{
}

void ToneFilterComponent::paint(juce::Graphics& g)
{
    

    auto bound = getLocalBounds();

    g.setColour(juce::Colours::black);
    g.fillRect(bound);

    g.setColour(juce::Colours::grey);
    g.drawRect(bound, 2.f);
    
}

void ToneFilterComponent::resized()
{

}

std::vector<juce::Component*> ToneFilterComponent::getComponent()
{
    return {

    };
}
