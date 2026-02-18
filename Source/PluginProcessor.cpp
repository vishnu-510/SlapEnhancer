#include "PluginProcessor.h"
#include "PluginEditor.h"

SlapEnhancerAudioProcessor::SlapEnhancerAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

SlapEnhancerAudioProcessor::~SlapEnhancerAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout
SlapEnhancerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat>("SLAP",   "Slap",   0.0f, 1.0f, 0.5f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("BITE",   "Bite",   0.0f, 1.0f, 0.5f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("PUNCH",  "Punch",  0.0f, 1.0f, 0.5f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("TONE",   "Tone",  -1.0f, 1.0f, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        "OUTPUT", "Output",
        juce::NormalisableRange<float> (-24.0f, 6.0f), 0.0f));

    return { params.begin(), params.end() };
}

void SlapEnhancerAudioProcessor::prepareToPlay (double, int) {}
void SlapEnhancerAudioProcessor::releaseResources() {}

bool SlapEnhancerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainInputChannelSet()  == juce::AudioChannelSet::stereo()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void SlapEnhancerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                               juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    float slap  = parameters.getRawParameterValue("SLAP")->load();
    float bite  = parameters.getRawParameterValue("BITE")->load();
    float punch = parameters.getRawParameterValue("PUNCH")->load();
    float tone  = parameters.getRawParameterValue("TONE")->load();
    float outDb = parameters.getRawParameterValue("OUTPUT")->load();

    for (int ch = 0; ch < numChannels; ++ch)
    {
        float* data = buffer.getWritePointer (ch);

        for (int i = 1; i < numSamples; ++i)
        {
            float delta = data[i] - data[i - 1];
            data[i] += delta * slap * 2.5f;
        }
    }

    for (int ch = 0; ch < numChannels; ++ch)
    {
        float* data = buffer.getWritePointer (ch);

        for (int i = 0; i < numSamples; ++i)
            data[i] = std::tanh (data[i] * (1.0f + bite * 4.0f));
    }

    float punchGain = 1.0f + punch * 0.6f;
    buffer.applyGain (punchGain);

    float toneGain = 1.0f + tone * 0.3f;
    buffer.applyGain (toneGain);

    float linearGain = juce::Decibels::decibelsToGain (outDb);
    buffer.applyGain (linearGain);
}


juce::AudioProcessorEditor* SlapEnhancerAudioProcessor::createEditor()
{
    return new SlapEnhancerAudioProcessorEditor (*this);
}

bool SlapEnhancerAudioProcessor::hasEditor() const { return true; }

const juce::String SlapEnhancerAudioProcessor::getName() const { return "Slap Enhancer"; }
bool SlapEnhancerAudioProcessor::acceptsMidi() const { return false; }
bool SlapEnhancerAudioProcessor::producesMidi() const { return false; }
bool SlapEnhancerAudioProcessor::isMidiEffect() const { return false; }
double SlapEnhancerAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int  SlapEnhancerAudioProcessor::getNumPrograms() { return 1; }
int  SlapEnhancerAudioProcessor::getCurrentProgram() { return 0; }
void SlapEnhancerAudioProcessor::setCurrentProgram (int) {}
const juce::String SlapEnhancerAudioProcessor::getProgramName (int) { return {}; }
void SlapEnhancerAudioProcessor::changeProgramName (int, const juce::String&) {}

void SlapEnhancerAudioProcessor::getStateInformation (juce::MemoryBlock&) {}
void SlapEnhancerAudioProcessor::setStateInformation (const void*, int) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SlapEnhancerAudioProcessor();
}
