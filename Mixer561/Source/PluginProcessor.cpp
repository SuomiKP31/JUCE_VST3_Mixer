/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Mixer561AudioProcessor::Mixer561AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    bitcrusherProcessor = BitCrusherProcessor(8, 10, true);
}

Mixer561AudioProcessor::~Mixer561AudioProcessor()
{
}

//==============================================================================
const juce::String Mixer561AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Mixer561AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Mixer561AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Mixer561AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Mixer561AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Mixer561AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Mixer561AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Mixer561AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Mixer561AudioProcessor::getProgramName (int index)
{
    return {};
}

void Mixer561AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Mixer561AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;

    spec.numChannels = 1;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = sampleRate;

    leftChain.prepare(spec);
    rightChain.prepare(spec);

    // TODO: Init effectors here
    flangerProcessor.setupDelayBuffer(getTotalNumInputChannels(), getSampleRate());
    phaserProcessor.setupFilters(getTotalNumInputChannels(), getSampleRate());
    tapestopProcessor.setupTapeBuffer(getTotalNumInputChannels(), getSampleRate());

    UpdateFilters();
}

void Mixer561AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Mixer561AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Mixer561AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // TODO: Resize effector DSP buffers here if needed
    repeatProcessor.resizeRepeatProcessorByChannelNum(totalNumInputChannels);
    gateProcessor.resizeGateProcessorByChannelNum(totalNumInputChannels);


    UpdateFilters();

    juce::dsp::AudioBlock<float> block(buffer);

    auto left_block = block.getSingleChannelBlock(0);
    auto right_block = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> left_context(left_block);
    juce::dsp::ProcessContextReplacing<float> right_context(right_block);


    auto sampleRate = getSampleRate();

    // Effectors run first
    repeatProcessor.process(buffer, sampleRate);
    gateProcessor.process(buffer, sampleRate);
    flangerProcessor.process(buffer, sampleRate);
    phaserProcessor.process(buffer, sampleRate);
    bitcrusherProcessor.process(buffer, sampleRate);
    tapestopProcessor.process(buffer, sampleRate);

    // Filters should run last
    leftChain.process(left_context);
    rightChain.process(right_context);

}

//==============================================================================
bool Mixer561AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Mixer561AudioProcessor::createEditor()
{
    return new Mixer561AudioProcessorEditor (*this);
}

//==============================================================================
// For parameters save/load
void Mixer561AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void Mixer561AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);
        UpdateFilters();
    }
}

//==============================================================================
// Free functions
ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings setting;

    setting.highCutFreq = apvts.getRawParameterValue("HighCut Freq")->load();
    setting.lowCutFreq = apvts.getRawParameterValue("LowCut Freq")->load();
    setting.peakFreq = apvts.getRawParameterValue("Peak Freq")->load();
    setting.peakGainInDecibels = apvts.getRawParameterValue("Peak Gain")->load();
    setting.peakQuality = apvts.getRawParameterValue("Peak Quality")->load();
    setting.lowCutSlope = apvts.getRawParameterValue("LowCut Slope")->load();
    setting.highCutSlope = apvts.getRawParameterValue("HighCut Slope")->load();

    return setting;
}

Coefficient makePeakFilter(const ChainSettings& chain_settings, double sampleRate)
{
    return juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, chain_settings.peakFreq,
        chain_settings.peakQuality, juce::Decibels::decibelsToGain(chain_settings.peakGainInDecibels));
}

void UpdateCoefficients(Coefficient& old, const Coefficient& replacement)
{
    *old = *replacement;
}

//==============================================================================
// Member functions
juce::AudioProcessorValueTreeState::ParameterLayout Mixer561AudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("LowCut Freq", "HPF",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
        20.f));  // Low Cut Filter = High Pass Filter HPF
    layout.add(std::make_unique<juce::AudioParameterFloat>("HighCut Freq", "LPF",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
        20000.f));  // High Cut Filter = Low Pass Filter LPF
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak Freq", "Peak Freq",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
        750.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak Gain", "Peak Gain",
        juce::NormalisableRange<float>(-24.f, 24.f, .5f, 1),
        0.0f));    // Gain is represented in decibels
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak Quality", "Peak Quality",
        juce::NormalisableRange<float>(0.1f, 10.f, .05f, 1),
        1.0f));    // Bandwidth of Peak

    juce::StringArray string_array;
    for (int i = 0; i < 4; i++)
    {
        juce::String str;
        str << 12 + i * 12;
        str << " db/Oct";
        string_array.add(str);
    }

    layout.add(std::make_unique<juce::AudioParameterChoice>("LowCut Slope", "LowCut(HP) Slope", string_array, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("HighCut Slope", "HighCut(LP) Slope", string_array, 0));

    return layout;
}

void Mixer561AudioProcessor::UpdatePeakFilter(const ChainSettings& chain_settings)
{
    auto peak_coef = makePeakFilter(chain_settings, getSampleRate());

    *leftChain.get<Peak>().coefficients = *peak_coef;
    *rightChain.get<Peak>().coefficients = *peak_coef;
}

void Mixer561AudioProcessor::UpdateFilters()
{
    auto chain_settings = getChainSettings(apvts);
    UpdatePeakFilter(chain_settings);

    UpdateLowCutFilters(chain_settings);

    UpdateHighCutFilters(chain_settings);
}

void Mixer561AudioProcessor::UpdateLowCutFilters(ChainSettings& chain_settings)
{
    auto cut_coef = makeLowCutCoefficient(chain_settings, getSampleRate());

    auto& lefthpf = leftChain.get<LowCut>();
    auto& righthpf = rightChain.get<LowCut>();
    UpdateCutFilter(lefthpf, cut_coef, chain_settings.lowCutSlope);
    UpdateCutFilter(righthpf, cut_coef, chain_settings.lowCutSlope);
}

void Mixer561AudioProcessor::UpdateHighCutFilters(ChainSettings& chain_settings)
{
    auto cut_hcoef = makeHighCutCoefficient(chain_settings, getSampleRate());

    auto& leftlpf = leftChain.get<HighCut>();
    auto& rightlpf = rightChain.get<HighCut>();
    UpdateCutFilter(leftlpf, cut_hcoef, chain_settings.highCutSlope);
    UpdateCutFilter(rightlpf, cut_hcoef, chain_settings.highCutSlope);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Mixer561AudioProcessor();
}
