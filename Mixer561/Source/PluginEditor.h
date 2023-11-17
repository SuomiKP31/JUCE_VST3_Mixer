/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TimeDomainProcessingComponent.h"
#include "LaunchpadComponent.h"

//==============================================================================
/**
*/
static void MultiplyMagnitude(double& mag, Filter& filter, double& sample_rate, double& freq);

template <int Index, typename ChainType>
static void ChainMagnitudeMultiply(double& mag, ChainType& cut, double& sample_rate, double& freq);

static inline juce::String GetFreqText(float val, juce::String suffix)
{
    bool addK = false;


    if (val > 999.f)
    {
        val /= 1000.f;
        addK = true;
    }

    juce::String str = juce::String(val, addK ? 2 : 0);


    if (suffix.isNotEmpty())
    {
        str << " ";
        if (addK) str << "k";

        str << suffix;
    }
    return str;
};


struct LookAndFeel : juce::LookAndFeel_V4
{

    void drawRotarySlider(juce::Graphics&,
        int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
        juce::Slider&) override;

};

struct RotarySliderWithLabel : juce::Slider
{
    RotarySliderWithLabel(juce::RangedAudioParameter& rap, const juce::String& unitSuffix) : juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag,
        juce::Slider::TextEntryBoxPosition::NoTextBox),
        param(&rap),
        suffix(unitSuffix)
    {
        setLookAndFeel(&lnf);
    }

    ~RotarySliderWithLabel()
    {
        setLookAndFeel(nullptr);
    }
    struct LabelPos
    {
        float pos;
        juce::String label;
    };

    void paint(juce::Graphics& g) override;
    juce::Rectangle<int> getSliderBounds() const;
    int getTextHeight() const { return 14; }
    juce::String getDisplayString() const;

    juce::Array<LabelPos> labels;
private:

    LookAndFeel lnf;

    juce::RangedAudioParameter* param;
    juce::String suffix;
};

static void AddLabel(RotarySliderWithLabel& rswl, juce::String l_lbl, juce::String r_lbl);

struct ResponseCurveComponent : juce::Component, juce::AudioParameterFloat::Listener,
    juce::Timer
{
    ResponseCurveComponent(Mixer561AudioProcessor&);
    ~ResponseCurveComponent() override;
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    void timerCallback() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    Mixer561AudioProcessor& audioProcessor;

    MonoChain monoChain;
    juce::Atomic<bool> parameterChanged{ false };

    juce::Image background;
    juce::Rectangle<int> GetRenderArea();
    juce::Rectangle<int> GetAnalysisArea();

    void PaintResponseCurve(juce::Graphics& g);
    void PaintAnalysisGrid();

    void UpdateGuiChain();
};

class Mixer561AudioProcessorEditor : public juce::AudioProcessorEditor
{
    struct KnobLabel
    {
        KnobLabel(float p, juce::String l)
        {
            nmlpos = p;
            lbl = l;
        }
        float nmlpos;
        juce::String lbl;
    };

public:
    Mixer561AudioProcessorEditor(Mixer561AudioProcessor&);
    ~Mixer561AudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    juce::Image logo;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Mixer561AudioProcessor& audioProcessor;

    RotarySliderWithLabel peakFreqSlider,
        peakGainSlider,
        peakQualitySlider,
        lowCutFreqSlider,
        highCutFreqSlider,
        lowCutSlopeSlider,
        highCutSlopeSlider;

    ResponseCurveComponent responseCurveComponent;
    TimeDomainProcessingComponent timeDomainComponent;
    LaunchpadComponent launchpadComponent;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment  peakFreqAttachment,
        peakGainAttachment,
        peakQualityAttachment,
        lowCutFreqAttachment,
        highCutFreqAttachment,
        lowCutSlopeAttachment,
        highCutSlopeAttachment;


    std::vector<juce::Component*> GetComps();

    juce::Rectangle<int> imageBound;
    juce::Rectangle<int> knobLabelBound;
    juce::Array<KnobLabel> labels;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mixer561AudioProcessorEditor)
};

