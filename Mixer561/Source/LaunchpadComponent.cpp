/*
  ==============================================================================

    LaunchpadComponent.cpp
    Created: 17 Nov 2023 11:43:21am
    Author:  fangz

  ==============================================================================
*/

#include "LaunchpadComponent.h"

LaunchpadComponent::LaunchpadComponent(Mixer561AudioProcessor& p) : audioProcessor(p)
{
    for (auto* comp : getComponent())
    {
        addAndMakeVisible(comp);
    }
    initButtons();

    setShortcuts();
}

LaunchpadComponent::~LaunchpadComponent()
{
}

void LaunchpadComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::aquamarine);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void LaunchpadComponent::resized()
{
}

std::vector<juce::Component*> LaunchpadComponent::getComponent()
{
    return std::vector<juce::Component*>();
}

void LaunchpadComponent::setShortcuts()
{
}

void LaunchpadComponent::initButtons()
{
}
