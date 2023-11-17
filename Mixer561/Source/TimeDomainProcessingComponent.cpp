/*
  ==============================================================================

    TimeDomainProcessingComponent.cpp
    Created: 14 Mar 2022 11:33:25am
    Author:  Roarlisf/KP31

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TimeDomainProcessingComponent.h"


// TODO: Trigger button actions

TimeDomainProcessingComponent::TimeDomainProcessingComponent(Mixer561AudioProcessor&p) : audioProcessor(p)
{

    bpmInput.setInputFilter(&bpmInput.lacr, false);

  
    for(auto* comp : getComponent())
    {
        addAndMakeVisible(comp);
    }

	initButtons();

	setShortcuts();
}

TimeDomainProcessingComponent::~TimeDomainProcessingComponent()
{


}

void TimeDomainProcessingComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::white);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void TimeDomainProcessingComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto bound = getLocalBounds();
	auto hb = 0.01 * bound.getHeight();
	auto wb = 0.01 * bound.getWidth();
	bound.removeFromTop(hb);
	bound.removeFromBottom(hb);
	bound.removeFromLeft(wb);
	bound.removeFromRight(wb);

	auto bpmBound = bound.removeFromLeft(bound.getWidth() * 0.33);
	bpmInput.label = "BPM";
    bpmInput.labelComp.setBounds(bpmBound.removeFromTop(bpmBound.getHeight() * 0.33));
    bpmInput.labelComp.setText(bpmInput.label, juce::dontSendNotification);
    bpmInput.setBounds(bpmBound);

	// TODO: Button GUI
	auto buttonW = bound.getWidth() * 0.25;
	auto repeatBound = bound.removeFromLeft(buttonW);
	auto repeatH = repeatBound.getHeight() * 0.25;

	auto gateBound = bound.removeFromLeft(buttonW);
	auto gateH = gateBound.getHeight() * 0.25;

	std::vector tbtn{ &re8, & re12, & re16, & re32 };
	for (int i = 0; i < 4; i++)
	{
		tbtn[i]->setBounds(repeatBound.removeFromTop(repeatH));

	}
	std::vector gbtn = { &ga4, &ga8, &ga12, &ga16 };
	for (int i = 0; i < 4; i++)
	{
		gbtn[i]->setBounds(gateBound.removeFromTop(gateH));
	}
	auto flanBound = bound.removeFromLeft(buttonW);
	auto flanH = flanBound.getHeight() * 0.25;

	fl1.setBounds(flanBound.removeFromTop(flanH));
	ph1.setBounds(flanBound.removeFromTop(flanH));
	bc1.setBounds(flanBound.removeFromTop(flanH));
	tp1.setBounds(flanBound.removeFromTop(flanH));
}


std::vector<juce::Component*> TimeDomainProcessingComponent::getComponent()
{
    return {
        &bpmInput,
        &bpmInput.labelComp,
		& re8,& re12,& re16,& re32,
		& ga4,& ga8,& ga12,& ga16,
		& fl1,& bc1,& ph1,& tp1 
    };
}

void TimeDomainProcessingComponent::setShortcuts()
{
	re8.addShortcut(juce::KeyPress('q', juce::ModifierKeys::shiftModifier, 0));
	re12.addShortcut(juce::KeyPress('w', juce::ModifierKeys::shiftModifier, 0));
	re16.addShortcut(juce::KeyPress('e', juce::ModifierKeys::shiftModifier, 0));
	re32.addShortcut(juce::KeyPress('r', juce::ModifierKeys::shiftModifier, 0));

	ga4.addShortcut(juce::KeyPress('a', juce::ModifierKeys::shiftModifier, 0));
	ga8.addShortcut(juce::KeyPress('s', juce::ModifierKeys::shiftModifier, 0));
	ga12.addShortcut(juce::KeyPress('d', juce::ModifierKeys::shiftModifier, 0));
	ga16.addShortcut(juce::KeyPress('f', juce::ModifierKeys::shiftModifier, 0));

	fl1.addShortcut(juce::KeyPress('z', juce::ModifierKeys::shiftModifier, 0));
	ph1.addShortcut(juce::KeyPress('x', juce::ModifierKeys::shiftModifier, 0));
	bc1.addShortcut(juce::KeyPress('c', juce::ModifierKeys::shiftModifier, 0));
	tp1.addShortcut(juce::KeyPress('v', juce::ModifierKeys::shiftModifier, 0));
}

void TimeDomainProcessingComponent::initButtons()
{
	// Init buttons and bind shortcuts here
	//=== REPEAT *4 ===
	std::vector tbtn{ &re8, & re12, & re16, & re32 };
	std::vector suffix{ 8, 12, 16, 32 };
	for (int i = 0; i < 4; i++)
	{
		char buttonText[20];
		sprintf(buttonText, "RE%d", suffix[i]);
		auto& btn = tbtn[i];
		btn->setButtonText(buttonText);
		btn->onStateChange = [btn] {btn->OnStateChanged(); };
		btn->denominator = suffix[i];
		btn->repeatProcessor = &audioProcessor.repeatProcessor;
		btn->bpmInput = &bpmInput;
	}

	std::vector gbtn{ &ga4, & ga8, & ga12, & ga16 }; // GA24+ sounds really bad
	suffix.assign({ 4, 8, 12, 16 });

	//=== GATE *4 ===
	for (int i = 0; i < 4; i++)
	{
		char buttonText[20];
		sprintf(buttonText, "GA%d", suffix[i]);
		auto& btn = gbtn[i];
		btn->setButtonText(buttonText);
		btn->onStateChange = [btn] {btn->OnStateChanged(); };
		btn->denominator = suffix[i];
		btn->gateProcessor = &audioProcessor.gateProcessor;
		btn->bpmInput = &bpmInput;
	}

	//=== FREQ *4 ===
	fl1.onStateChange = [btn = &fl1] {btn->OnStateChanged();  };
	fl1.setButtonText("Flanger");
	fl1.processor = &audioProcessor.flangerProcessor;

	ph1.onStateChange = [btn = &ph1] {btn->OnStateChanged(); };
	ph1.setButtonText("Phaser");
	ph1.processor = &audioProcessor.phaserProcessor;

	bc1.onStateChange = [btn = &bc1] {btn->OnStateChanged();  };
	bc1.setButtonText("BitCrusher");
	bc1.processor = &audioProcessor.bitcrusherProcessor;

	tp1.onStateChange = [btn = &tp1] {btn->OnStateChanged();  };
	tp1.setButtonText("TapeStop");
	tp1.processor = &audioProcessor.tapestopProcessor;
}
