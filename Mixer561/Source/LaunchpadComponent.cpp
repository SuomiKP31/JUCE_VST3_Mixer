/*
  ==============================================================================

    LaunchpadComponent.cpp
    Created: 17 Nov 2023 11:43:21am
    Author:  KP31

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
    auto bound = getLocalBounds();
    bound.removeFromLeft(1);
    bound.removeFromRight(1);

    auto labelHeight = 0.03 * bound.getHeight(); // 3 labels = 0.09
    auto buttonHeight = 0.09 * bound.getHeight(); // 10 Buttons = 0.9


    ResetAll.setBounds(bound.removeFromTop(buttonHeight));

    auto labelBound = bound.removeFromTop(labelHeight);
    Hlabel.setBounds(labelBound);

    std::vector hbtn = { &HPFAdd, &HPFReset, &HPFSubtract };
    auto HPFBound = bound.removeFromTop(buttonHeight * 3.1);
    for (int i = 0; i < 3; i++) {
        hbtn[i]->setBounds(HPFBound.removeFromTop(buttonHeight));
    }

    labelBound = bound.removeFromTop(labelHeight);
    Llabel.setBounds(labelBound);

    std::vector tbtn {&LPFAdd, & LPFReset, & LPFSubtract};
    auto LPFBound = bound.removeFromTop(buttonHeight * 3.1);
    for (int i = 0; i < 3; i++) {
        tbtn[i]->setBounds(LPFBound.removeFromTop(buttonHeight));
    }

    labelBound = bound.removeFromTop(labelHeight);
    Plabel.setBounds(labelBound);


    std::vector pbtn = {&PeakAdd, & PeakReset, & PeakSubstract};
    auto PeakBound = bound.removeFromTop(buttonHeight * 3.1);
    for (int i = 0; i < 3; i++) {
        pbtn[i]->setBounds(PeakBound.removeFromTop(buttonHeight));
    }


}

std::vector<juce::Component*> LaunchpadComponent::getComponent()
{
    return { &ResetAll,&Plabel, &PeakAdd, &PeakReset, &PeakSubstract, &Llabel, &LPFAdd, &LPFReset, &LPFSubtract, &Hlabel, &HPFAdd, &HPFReset, &HPFSubtract };
}

void LaunchpadComponent::setShortcuts()
{
}

void LaunchpadComponent::initButtons()
{
    Plabel.setText("Peak", juce::dontSendNotification);
    Hlabel.setText("HPF", juce::dontSendNotification);
    Llabel.setText("LPF", juce::dontSendNotification);

    std::vector tbtn {&ResetAll, & PeakAdd, & PeakReset, & PeakSubstract, & LPFAdd, & LPFReset, & LPFSubtract, & HPFAdd, & HPFReset, & HPFSubtract};
    std::vector btnText {"R", ">>", "Peak-R", "<<", ">>", "LPF-R", "<<", ">>", "HPF-R", "<<"};


    for (int i = 0; i < tbtn.size(); i++) {
        auto& btn = tbtn[i];
        btn->audioProcessor = &audioProcessor;
        btn->setButtonText(btnText[i]);
        btn->onStateChange = [btn] {btn->OnStateChanged(); };
    }
}


