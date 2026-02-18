#include "PluginEditor.h"

SlapEnhancerAudioProcessorEditor::SlapEnhancerAudioProcessorEditor (SlapEnhancerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (540, 300);

    auto setupKnob = [&](juce::Slider& s)
    {
        s.setColour (juce::Slider::rotarySliderFillColourId,
                     juce::Colour (0xffff7aa2)); // toy pink
        s.setColour (juce::Slider::rotarySliderOutlineColourId,
                     juce::Colours::transparentBlack);
        s.setColour (juce::Slider::thumbColourId,
                     juce::Colours::white);
        s.setColour (juce::Slider::textBoxTextColourId,
                     juce::Colours::white);
        s.setColour (juce::Slider::textBoxOutlineColourId,
                     juce::Colours::transparentBlack);
        addAndMakeVisible (s);
    };

    auto setupLabel = [&](juce::Label& l, const juce::String& t)
    {
        l.setText (t, juce::dontSendNotification);
        l.setJustificationType (juce::Justification::centred);
        l.setColour (juce::Label::textColourId, juce::Colours::white);
        l.setFont (juce::Font (12.0f, juce::Font::bold));
        addAndMakeVisible (l);
    };

    setupKnob (slapKnob);   setupLabel (slapLabel,  "SLAP");
    setupKnob (biteKnob);   setupLabel (biteLabel,  "BITE");
    setupKnob (punchKnob);  setupLabel (punchLabel, "PUNCH");
    setupKnob (toneKnob);   setupLabel (toneLabel,  "BRIGHT");
    setupKnob (outKnob);    setupLabel (outLabel,   "LEVEL");

    slapAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,"SLAP",slapKnob);
    biteAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,"BITE",biteKnob);
    punchAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,"PUNCH",punchKnob);
    toneAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,"TONE",toneKnob);
    outAttach   = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,"OUTPUT",outKnob);
}

SlapEnhancerAudioProcessorEditor::~SlapEnhancerAudioProcessorEditor() {}

void SlapEnhancerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff5b8cff));

    auto panel = juce::Rectangle<float> (20.0f, 60.0f,
                                         getWidth() - 40.0f, 180.0f);

    g.setColour (juce::Colour (0xff8fb6ff)); 
    g.fillRoundedRectangle (panel, 20.0f);

    g.setColour (juce::Colour (0x40ffffff));
    g.fillRoundedRectangle (panel.removeFromTop (35.0f), 18.0f);


    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (22.0f, juce::Font::bold));
    g.drawText ("SLAP ENHANCER",
                0, 18, getWidth(), 30,
                juce::Justification::centred);
}

void SlapEnhancerAudioProcessorEditor::resized()
{
    const int knobSize = 68;
    const int labelH   = 18;
    const int yKnob    = 95;

    const int sideMargin = 40;
    const int totalKnobs = 5;

    const int usableWidth = getWidth() - sideMargin * 2;
    const int spacing =
        (usableWidth - (knobSize * totalKnobs)) / (totalKnobs - 1);

    int x = sideMargin;

    auto place = [&](juce::Slider& s, juce::Label& l)
    {
        s.setBounds (x, yKnob, knobSize, knobSize);
        l.setBounds (x, yKnob + knobSize + 6, knobSize, labelH);
        x += knobSize + spacing;
    };

    place (slapKnob,  slapLabel);
    place (biteKnob,  biteLabel);
    place (punchKnob, punchLabel);
    place (toneKnob,  toneLabel);
    place (outKnob,   outLabel);
}
