# SlapEnhancer

A real-time audio effect plugin built using C++ and the JUCE framework.

## Overview

SlapEnhancer is an audio processing plugin designed to enhance transient punch,
harmonic content, and brightness in bass-heavy material.

## Architecture

- `PluginProcessor` handles audio processing and parameter management.
- `PluginEditor` handles GUI layout and control binding.
- Parameters are managed using `AudioProcessorValueTreeState`.
- Audio processing occurs inside `processBlock()`.

## DSP Signal Flow

Input
→ Transient Section
→ Saturation Stage
→ Brightness Control
→ Output Gain

## Real-Time Considerations

- No dynamic memory allocation inside `processBlock()`
- Audio thread and UI thread are separated
- Processing performed per-sample inside the audio buffer loop
- Parameters accessed through APVTS

## Technologies Used

- C++
- JUCE Framework
- Audio Units (AU)

## Future Improvements

- Add parameter smoothing
- Add oversampling for saturation
- Modularize DSP stages
- Add performance profiling

## Build

This project was developed using the JUCE framework.

To build:
1. Open the project in Projucer.
2. Generate IDE project files.
3. Build using Xcode (macOS).

Future versions will migrate to a CMake-based workflow.

