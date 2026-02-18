#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SpringSlider : public juce::Slider,
                     private juce::Timer
{
public:
    SpringSlider()
    {
        setSliderStyle (juce::Slider::RotaryVerticalDrag);
        setTextBoxStyle (juce::Slider::TextBoxBelow, false, 48, 18);
        setVelocityBasedMode (false);
        setMouseDragSensitivity (140);
    }

    void mouseUp (const juce::MouseEvent& e) override
    {
        startTimerHz (60); 
        juce::Slider::mouseUp (e);
    }

private:
    void timerCallback() override
    {
        double v = getValue();
        double target = juce::jlimit (getMinimum(), getMaximum(), v);

        double newValue = v + (target - v) * 0.35;

        if (std::abs (newValue - v) < 0.0005)
        {
            stopTimer();
            return;
        }

        setValue (newValue, juce::dontSendNotification);
    }
};


class SlapEnhancerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    SlapEnhancerAudioProcessorEditor (SlapEnhancerAudioProcessor&);
    ~SlapEnhancerAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SlapEnhancerAudioProcessor& processor;

    SpringSlider slapKnob, biteKnob, punchKnob, toneKnob, outKnob;
    juce::Label  slapLabel, biteLabel, punchLabel, toneLabel, outLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        slapAttach, biteAttach, punchAttach, toneAttach, outAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlapEnhancerAudioProcessorEditor)
};

