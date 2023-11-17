/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


void RotarySliderWithLabel::paint(juce::Graphics& g)
{
    using namespace juce;

    auto startAng = degreesToRadians(225.f);
    auto endAng = degreesToRadians(135.f) + MathConstants<float>::twoPi;

    auto range = getRange();

    auto sliderBounds = getSliderBounds();

    /*g.setColour(Colours::red);
    g.drawRect(getLocalBounds());
    g.setColour(Colours::white);
    g.drawRect(sliderBounds);*/

    getLookAndFeel().drawRotarySlider(g, sliderBounds.getX(), sliderBounds.getY(), sliderBounds.getWidth(), sliderBounds.getHeight()
        , jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
        startAng, endAng, *this);

    auto center = sliderBounds.toFloat().getCentre();
    auto radius = sliderBounds.getWidth() * 0.5f;

    g.setColour(Colour(39u, 206u, 224u));
    g.setFont(getTextHeight());

    auto numChoices = labels.size();

    for (int i = 0; i < numChoices; ++i)
    {
        auto pos = labels[i].pos;
        jassert(0.f <= pos);
        jassert(pos <= 1.f);

        auto ang = jmap(pos, 0.f, 1.f, startAng, endAng);

        auto c = center.getPointOnCircumference(radius + getTextHeight() * 0.75f + 1, ang); // Get corner pos, move it a little bit outwards

        Rectangle<float> r;
        auto str = labels[i].label;
        r.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());

        r.setY(r.getY() + getTextHeight()); // Move downwards a little. (JUCE Anchor is the top left corner)
        r.setCentre(c);
        g.drawFittedText(str, r.toNearestInt(), Justification::centred, 1);
    }
}

juce::Rectangle<int> RotarySliderWithLabel::getSliderBounds() const
{
    using namespace juce;
    auto bounds = getLocalBounds();

    auto size = jmin(bounds.getWidth(), bounds.getHeight());

    size -= getTextHeight() * 2;

    Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), 0);
    r.setY(2);


    return r;
}

juce::String RotarySliderWithLabel::getDisplayString() const
{
    if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(param))
        return choiceParam->getCurrentChoiceName();

    juce::String str;
    bool addK = false;

    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(param))
    {
        float val = getValue();

        str = GetFreqText(val, suffix);
    }
    else
    {
        jassertfalse;
    }
    return str;
}

void LookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    using namespace juce;
    auto bounds = Rectangle<float>(x, y, width, height);

    g.setColour(Colour(0u, 53u, 128u));
    g.fillEllipse(bounds);

    g.setColour(Colour(0u, 108u, 224u));
    g.drawEllipse(bounds, 1.f);

    if (auto* rswl = dynamic_cast<RotarySliderWithLabel*>(&slider))
    {
        // Draw Center rod with a path
        auto center = bounds.getCentre();
        Path p;

        Rectangle<float> r;
        r.setLeft(center.getX() - 2);
        r.setRight(center.getX() + 2);
        r.setTop(bounds.getY());
        r.setBottom(center.getY() - rswl->getTextHeight() * 1.5);
        p.addRoundedRectangle(r, 2.f);
        jassert(rotaryStartAngle < rotaryEndAngle);
        auto sliderRad = jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);

        p.applyTransform(AffineTransform().rotated(sliderRad, center.getX(), center.getY()));
        g.fillPath(p);

        g.setFont(rswl->getTextHeight());
        auto text = rswl->getDisplayString();
        auto strWidth = g.getCurrentFont().getStringWidth(text);

        r.setSize(strWidth + 4, rswl->getTextHeight() + 2);
        r.setCentre(center);

        g.setColour(Colours::black);
        g.fillRect(r);

        g.setColour(Colours::white);
        g.drawFittedText(text, r.toNearestInt(), Justification::centred, 1);
    }





}
//==============================================================================
ResponseCurveComponent::ResponseCurveComponent(Mixer561AudioProcessor& p) : audioProcessor(p)
{
    const auto& params = audioProcessor.getParameters();
    for (const auto param : params)
    {
        param->addListener(this);
    }

    UpdateGuiChain();
    startTimerHz(30);
}

ResponseCurveComponent::~ResponseCurveComponent()
{
    const auto& params = audioProcessor.getParameters();
    for (const auto param : params)
    {
        param->removeListener(this);
    }
}

void ResponseCurveComponent::parameterValueChanged(int parameterIndex, float newValue)
{
    parameterChanged.set(true);
}

void ResponseCurveComponent::timerCallback()
{
    if (parameterChanged.compareAndSetBool(false, true))
    {
        //  Update Monochains
        UpdateGuiChain();
        //  Signal Repaint
        repaint();
    }
}

void ResponseCurveComponent::UpdateGuiChain()
{
    auto sampleRate = audioProcessor.getSampleRate();

    auto chainSettings = getChainSettings(audioProcessor.apvts);
    auto peak_coef = makePeakFilter(chainSettings, sampleRate);
    UpdateCoefficients(monoChain.get<Peak>().coefficients, peak_coef);

    auto cut_l_coef = makeLowCutCoefficient(chainSettings, sampleRate);
    auto cut_h_coef = makeHighCutCoefficient(chainSettings, sampleRate);

    UpdateCutFilter(monoChain.get<LowCut>(), cut_l_coef, chainSettings.lowCutSlope);
    UpdateCutFilter(monoChain.get<HighCut>(), cut_h_coef, chainSettings.highCutSlope);

}


juce::Rectangle<int> ResponseCurveComponent::GetRenderArea()
{
    auto bounds = getLocalBounds();

    /*bounds.reduce(JUCE_LIVE_CONSTANT(5),
        JUCE_LIVE_CONSTANT(5));*/
        // bounds.reduce(12, 8);
    bounds.removeFromTop(15);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(20);
    bounds.removeFromRight(20);
    return bounds;
}

juce::Rectangle<int> ResponseCurveComponent::GetAnalysisArea()
{
    auto bounds = GetRenderArea();
    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);

    return bounds;
}

void ResponseCurveComponent::PaintResponseCurve(juce::Graphics& g)
{
    using namespace juce;
    // (Our component is opaque, so we must completely fill the background with a solid colour)

    auto responseArea = GetAnalysisArea();

    auto w = responseArea.getWidth();
    auto sampleRate = audioProcessor.getSampleRate();

    auto& lowCut = monoChain.get<LowCut>();
    auto& highCut = monoChain.get<HighCut>();

    std::vector<double> magnitudes;
    magnitudes.resize(w);  // One for each pixel

    for (int i = 0; i < w; i++)
    {
        double mag = 1.f;
        double freq = mapToLog10(double(i) / double(w), 20.0, 20000.0);
        ChainMagnitudeMultiply<Peak, MonoChain>(mag, monoChain, sampleRate, freq);

        ChainMagnitudeMultiply<0, CutFilter>(mag, lowCut, sampleRate, freq);
        ChainMagnitudeMultiply<1, CutFilter>(mag, lowCut, sampleRate, freq);
        ChainMagnitudeMultiply<2, CutFilter>(mag, lowCut, sampleRate, freq);
        ChainMagnitudeMultiply<3, CutFilter>(mag, lowCut, sampleRate, freq);

        ChainMagnitudeMultiply<0, CutFilter>(mag, highCut, sampleRate, freq);
        ChainMagnitudeMultiply<1, CutFilter>(mag, highCut, sampleRate, freq);
        ChainMagnitudeMultiply<2, CutFilter>(mag, highCut, sampleRate, freq);
        ChainMagnitudeMultiply<3, CutFilter>(mag, highCut, sampleRate, freq);

        magnitudes[i] = Decibels::gainToDecibels(mag);
    }
    Path responseCurve;

    const double outputMin = responseArea.getBottom();
    const double outputMax = responseArea.getY();

    auto map = [outputMin, outputMax](double input)
    {
        // Map gains in decibels to screen coords inside the response area
        return jmap(input, -24.0, 24.0, outputMin, outputMax);
    };

    int x = responseArea.getX();
    responseCurve.startNewSubPath(x, map(magnitudes.front()));

    for (size_t i = 1; i < w; i++)
    {
        responseCurve.lineTo(x + i, map(magnitudes[i]));
    }

    g.setColour(Colours::azure);
    g.drawRoundedRectangle(GetRenderArea().toFloat(), 4.f, 1.f);
    g.setColour(Colours::white);
    g.strokePath(responseCurve, PathStrokeType(2.f));
}

void ResponseCurveComponent::PaintAnalysisGrid()
{
    using namespace juce;
    background = Image(Image::PixelFormat::RGB, getWidth(), getHeight(), true);  // This is static, so only need to draw once
    Graphics g(background);
    g.setColour(Colours::dimgrey);

    Array<float> freqs{
        20, 30, 40, 50, 100, 200, 300, 500, 1000, 2000, 3000, 5000, 10000, 20000
    };

    Array<float> gains{
        -24, -12, 0, 12, 24
    };

    // Grid Lines
    auto area = GetAnalysisArea();
    auto left = area.getX();
    auto right = area.getRight();
    auto top = area.getY();
    auto bottom = area.getBottom();
    auto width = area.getWidth();

    Array<float> xs;

    for (auto const f : freqs)
    {
        auto normX = mapFromLog10(f, 20.f, 20000.f);
        auto x = left + width * normX;
        g.drawVerticalLine(x, top, bottom);
        xs.add(x);
    }


    for (auto const gain : gains)
    {
        g.setColour(gain == 0 ? Colours::darkcyan : Colours::dimgrey);
        auto y = jmap(gain, -24.f, 24.f, float(getHeight()), 0.f);

        g.drawHorizontalLine(y, left, right);
    }

    // Grid Labels
    g.setColour(Colours::lightgrey);
    const int fontHeight = 10;
    g.setFont(fontHeight);

    for (int i = 0; i < freqs.size(); i++)
    {
        juce::String str = GetFreqText(freqs[i], "Hz");
        auto x = xs[i];

        auto textWidth = g.getCurrentFont().getStringWidth(str);

        Rectangle<int> r;
        r.setSize(textWidth, fontHeight);
        r.setCentre(x, 0);
        r.setY(1);

        g.drawFittedText(str, r, Justification::centred, 1);
    }

    for (auto gain : gains)
    {
        auto y = jmap(gain, -24.f, 24.f, float(bottom), float(top));

        String str;
        if (gain > 0) str << "+";
        str << gain;

        auto textWidth = g.getCurrentFont().getStringWidth(str);
        // Right: Gain labels
        Rectangle<int> r;
        r.setSize(textWidth, fontHeight);
        r.setX(getWidth() - textWidth);
        r.setCentre(r.getCentreX(), y);

        g.setColour(gain == 0 ? Colours::lightblue : Colours::lightgrey);
        g.drawFittedText(str, r, Justification::centred, 1);
        // Left db labels
        str.clear();
        str << roundToInt(gain - 24.f);
        r.setX(1);
        r.setSize(textWidth, fontHeight);
        r.setCentre(r.getCentreX(), y);
        g.setColour(Colours::lightgrey);
        g.drawFittedText(str, r, Justification::centred, 1);
    }
}

void ResponseCurveComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.drawImage(background, getLocalBounds().toFloat());
    PaintResponseCurve(g);

}

void ResponseCurveComponent::resized()
{
    PaintAnalysisGrid();
}


//==============================================================================
Mixer561AudioProcessorEditor::Mixer561AudioProcessorEditor(Mixer561AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    peakFreqSlider(*audioProcessor.apvts.getParameter("Peak Freq"), "Hz"),
    peakGainSlider(*audioProcessor.apvts.getParameter("Peak Gain"), "dB"),
    peakQualitySlider(*audioProcessor.apvts.getParameter("Peak Quality"), ""),
    lowCutFreqSlider(*audioProcessor.apvts.getParameter("LowCut Freq"), "Hz"),
    highCutFreqSlider(*audioProcessor.apvts.getParameter("HighCut Freq"), "Hz"),
    lowCutSlopeSlider(*audioProcessor.apvts.getParameter("LowCut Slope"), "db/Oct"),
    highCutSlopeSlider(*audioProcessor.apvts.getParameter("HighCut Slope"), "db/Oct"),
    responseCurveComponent(p),
    timeDomainComponent(p),
    launchpadComponent(p),
    peakFreqAttachment(audioProcessor.apvts, "Peak Freq", peakFreqSlider),
    peakGainAttachment(audioProcessor.apvts, "Peak Gain", peakGainSlider),
    peakQualityAttachment(audioProcessor.apvts, "Peak Quality", peakQualitySlider),
    lowCutFreqAttachment(audioProcessor.apvts, "LowCut Freq", lowCutFreqSlider),
    highCutFreqAttachment(audioProcessor.apvts, "HighCut Freq", highCutFreqSlider),
    lowCutSlopeAttachment(audioProcessor.apvts, "LowCut Slope", lowCutSlopeSlider),
    highCutSlopeAttachment(audioProcessor.apvts, "HighCut Slope", highCutSlopeSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    RotarySliderWithLabel* rswlWithFreq[]
    {
        &peakFreqSlider,
        &lowCutFreqSlider, &highCutFreqSlider
    };

    RotarySliderWithLabel* rswlWithdbOct[]
    {
        &highCutSlopeSlider, &lowCutSlopeSlider
    };
    AddLabel(peakGainSlider, "-24dB", "24dB");
    AddLabel(peakQualitySlider, "0.1", "10");
    for (const auto rswl : rswlWithFreq)
    {
        AddLabel(*rswl, "20Hz", "20kHz");
    }
    for (const auto rswl : rswlWithdbOct)
    {
        AddLabel(*rswl, "12 db/Oct", "48 db/Oct");
    }


    for (auto* comp : GetComps())
    {
        addAndMakeVisible(comp);
    }


    setSize(1000, 960);

}

Mixer561AudioProcessorEditor::~Mixer561AudioProcessorEditor()
{

}

//==============================================================================
void Mixer561AudioProcessorEditor::paint(juce::Graphics& g)
{
    g.drawImageWithin(logo, imageBound.getX(),
        imageBound.getY(), imageBound.getWidth(), imageBound.getHeight(),
        juce::RectanglePlacement());

    auto xmin = 0;
    auto xmax = knobLabelBound.getWidth();

    g.setColour(juce::Colours::lightsteelblue);
    g.setFont(20);
    auto centerY = knobLabelBound.getCentreY();
    for (auto rpos : labels)
    {
        auto mapped_x = juce::jmap<float>(rpos.nmlpos, xmin, xmax);
        auto width = g.getCurrentFont().getStringWidth(rpos.lbl);

        juce::Rectangle<float> r;
        r.setSize(width, 30);
        r.setCentre(mapped_x, 0);
        r.setY(centerY);

        g.drawFittedText(rpos.lbl, r.toNearestInt(), juce::Justification::centred, 1);
    }
}

void Mixer561AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bound = getLocalBounds();
    auto responseArea = bound.removeFromTop(bound.getHeight() * 0.25);
    auto launchpadArea = bound.removeFromRight(bound.getWidth() * 0.20);
    auto timeDomainArea = bound.removeFromBottom(bound.getHeight() * 0.33);
    

    responseCurveComponent.setBounds(responseArea);
    timeDomainComponent.setBounds(timeDomainArea);
    launchpadComponent.setBounds(launchpadArea);
   

    knobLabelBound = bound.removeFromTop(bound.getHeight() * 0.1);

    auto lowcutArea = bound.removeFromLeft(bound.getWidth() * 0.33);
    auto highcutArea = bound.removeFromRight(bound.getWidth() * 0.5);

    lowCutFreqSlider.setBounds(lowcutArea.removeFromTop(lowcutArea.getHeight() * 0.5));
    lowCutSlopeSlider.setBounds(lowcutArea);
    highCutFreqSlider.setBounds(highcutArea.removeFromTop(highcutArea.getHeight() * 0.5));
    highCutSlopeSlider.setBounds(highcutArea);

    imageBound = bound.removeFromTop(bound.getHeight() * 0.25);
    peakFreqSlider.setBounds(bound.removeFromTop(bound.getHeight() * 0.33));
    peakGainSlider.setBounds(bound.removeFromTop(bound.getHeight() * 0.5));
    peakQualitySlider.setBounds(bound);

    logo = juce::ImageFileFormat::loadFrom(BinaryData::_561Mixer_png, BinaryData::_561Mixer_pngSize);

    labels.clear();
    labels.add(KnobLabel(0.17f, "HPF"));
    labels.add(KnobLabel(0.5f, "Peak"));
    labels.add(KnobLabel(0.83f, "LPF"));

}





std::vector<juce::Component*> Mixer561AudioProcessorEditor::GetComps()
{
    return {
        &peakFreqSlider,
        &peakGainSlider,
        &peakQualitySlider,
        &lowCutFreqSlider,
        &highCutFreqSlider,
        &lowCutSlopeSlider,
        &highCutSlopeSlider,
        &responseCurveComponent,
        &timeDomainComponent,
        &launchpadComponent
    };
}

static void MultiplyMagnitude(double& mag, Filter& filter, double& sample_rate, double& freq)
{
    mag *= filter.coefficients->getMagnitudeForFrequency(freq, sample_rate);
}

template <int Index, typename ChainType>
static void ChainMagnitudeMultiply(double& mag, ChainType& cut, double& sample_rate, double& freq)
{
    if (!cut.template isBypassed<Index>())
    {
        MultiplyMagnitude(mag, cut.template get<Index>(), sample_rate, freq);
    }
}

static void AddLabel(RotarySliderWithLabel& rswl, juce::String l_lbl, juce::String r_lbl)
{
    rswl.labels.add({ 0.f, l_lbl });
    rswl.labels.add({ 1.f, r_lbl });
}

