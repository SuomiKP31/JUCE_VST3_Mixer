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
    startTimerHz(60); // 60 frames per second (from juce::Timer)
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

   

    // tone Energy Distribution
    g.setColour(juce::Colours::green);
    for (size_t i = 0; i < toneBounds.size(); i++)
    {
        float e = juce::jlimit(0.f, 1.f, juce::jmap(audioProcessor.toneRMS[i], 0.f, 0.1f, 0.f, 1.f));

        juce::Rectangle<int> amplitude(toneBounds[i]);
        amplitude.expand(-2, -2);
        amplitude.setHeight(amplitude.getHeight() * e);
        g.fillRect(amplitude);
    }

    // Frame
    g.setColour(juce::Colours::white);
    for (size_t i = 0; i < toneBounds.size(); i++)
    {
        g.drawRect(toneBounds[i], 1.f);
        g.drawFittedText(juce::String(audioProcessor.toneRMS[i], 5), toneBounds[i], juce::Justification::centred, 1);

        juce::Rectangle<int> amplitude(toneBounds[i]);
        amplitude.expand(-1, -1);

    }
    // Key Labels
    if (toneBounds.size() == 48) {
        const char* noteNames[] = {
            "A3", "A#3", "B3", "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4",
            "A4", "A#4", "B4", "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5",
            "A5", "A#5", "B5", "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6",
            "A6", "A#6", "B6", "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7"
        };

        for (int i = 0; i < 48; ++i) {
            g.drawFittedText(juce::String(noteNames[i]), toneBounds[i], juce::Justification::bottomLeft, 1);
        }
    }


}

void ToneFilterComponent::resized()
{
    auto bound = getLocalBounds();
    toneBounds.clear();

    int toneBlockWidth = bound.getWidth() / 12;
    int toneBlockHeight = bound.getHeight() / 4;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 12; j++) {
            juce::Rectangle<int> toneBound(j * toneBlockWidth, i * toneBlockHeight, toneBlockWidth, toneBlockHeight);
            toneBounds.push_back(toneBound);
        }
    }
}

std::vector<juce::Component*> ToneFilterComponent::getComponent()
{
    return {

    };
}

void ToneFilterComponent::timerCallback() {
    repaint();
}
